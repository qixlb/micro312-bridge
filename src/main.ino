// #include "userconfig.h"
#include <Arduino.h>
#include "BluetoothSerialHandler.h"
#include "BluetoothSerial.h"
#include <map>
#include <string>
#include <functional>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <WiFiManager.h>
#include "mk312.h"
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

BluetoothSerial SerialBT;
WiFiManager wifiManager;
WiFiUDP Udp;
OSCErrorCode error;

// WiFi
const unsigned int outPort = 9999;   // set this as incoming port on control surface
const unsigned int localPort = 8888; // set this as outgoing port on control surface
const int retry_limit = 10;
IPAddress espIP(0, 0, 0, 0);
IPAddress broadcastIP(0, 0, 0, 0);
#define WIFI_SSID "esp"     // fallback ssid
#define WIFI_PSK "12345678" // fallback psk

// Bluetooth
bool pair_new = false; // TODO implement pairing new device. now using static address for faster testing
uint8_t btaddr[6] = {0xa6, 0x20, 0x1b, 0x5a, 0x4c, 0x41};
BTAddress myaddr = btaddr;
static bool btScanAsync = true;
static bool btScanSync = true;
esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE; // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
esp_spp_role_t role = ESP_SPP_ROLE_MASTER; // or ESP_SPP_ROLE_MASTER
#define BT_DISCOVER_TIME 10000

bool hb; //variable for pulsing
#define LED 2 //status led
int hb_rate = 1000;

void init_bt()
{
  if (!SerialBT.begin("test", true))
  {
    Serial.println("SerialBT failed!");
    abort();
  }
  if (pair_new)
  {
    Serial.println("Starting discoverAsync...");
    BTScanResults *btDeviceList = SerialBT.getScanResults(); // maybe accessing from different threads!
    if (SerialBT.discoverAsync([](BTAdvertisedDevice *pDevice)
                               {
      // BTAdvertisedDeviceSet*set = reinterpret_cast<BTAdvertisedDeviceSet*>(pDevice);
      //btDeviceList[pDevice->getAddress()] = * set;
      Serial.printf(">>>>>>>>>>>Found a new device asynchronously: %s\n", pDevice->toString().c_str()); }))
    {
      delay(BT_DISCOVER_TIME);
      Serial.print("Stopping discoverAsync... ");
      SerialBT.discoverAsyncStop();
      Serial.println("discoverAsync stopped");
      delay(5000);
      if (btDeviceList->getCount() > 0)
      {
        BTAddress addr;
        int channel = 0;
        Serial.println("Found devices:");
        for (int i = 0; i < btDeviceList->getCount(); i++)
        {
          BTAdvertisedDevice *device = btDeviceList->getDevice(i);
          Serial.printf(" ----- %s  %s %d\n", device->getAddress().toString().c_str(), device->getName().c_str(), device->getRSSI());
          std::map<int, std::string> channels = SerialBT.getChannels(device->getAddress());
          Serial.printf("scanned for services, found %d\n", channels.size());
          for (auto const &entry : channels)
          {
            Serial.printf("     channel %d (%s)\n", entry.first, entry.second.c_str());
          }
          if (channels.size() > 0)
          {
            addr = device->getAddress();
            channel = channels.begin()->first;
          }
        }
        if (addr)
        {
          Serial.printf("connecting to %s - %d\n", addr.toString().c_str(), channel);
          SerialBT.connect(addr, channel, sec_mask, role);
        }
      }
      else
      {
        Serial.println("Didn't find any devices");
      }
    }
    else
    {
      Serial.println("Error on discoverAsync f.e. not workin after a \"connect\"");
    }
  }
  else
  {
    bt_connect_known();
  }
}

void bt_connect_known()
{
  while (!SerialBT.connected())
  {
    SerialBT.connect(myaddr, 2, sec_mask, role);
    delay(1000);
  }
  hb_rate = 1000;
}

void init_wifi()
{
  wifiManager.setConnectTimeout(retry_limit); // retry_limit in secondi
  wifiManager.autoConnect(WIFI_SSID, WIFI_PSK);
  Serial.println("Connected to WiFi!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
  espIP = WiFi.localIP();
  broadcastIP = espIP;
  broadcastIP[3] = 255;
}

void BluetoothReconnectTask(void *param)
{
  if (!SerialBT.begin("test", true))
  {
    Serial.println("Failed to initialize Bluetooth");
  }

  while (true)
  {
    if (!SerialBT.connected())
    {
      hb_rate = 250;
      Serial.println("Disconnected. Attempting to reconnect...");
      SerialBT.end();
      vTaskDelay(pdMS_TO_TICKS(5000));
      if (!SerialBT.begin("test", true))
      {
        Serial.println("Reconnect failed");
      }
      else
      {
        bt_connect_known();
        Serial.println("Reconnected successfully");
        init_mk312_easy();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void heartbeat(void *param)
{
  while (true)
  {
    sendHeartbeat(hb);
    digitalWrite(LED, hb);
    hb = !hb;
    vTaskDelay(pdMS_TO_TICKS(hb_rate));
  }
}

void tellMyIP(void *param)
{
  while (true)
  {
    OSCMessage msg = "/cfg/myip";
    msg.add(WiFi.localIP().toString().c_str());
    Udp.beginPacket(broadcastIP, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  init_wifi();
  // Serial2.begin(19200, SERIAL_8N1, 13, 15);
  xTaskCreate(heartbeat, "Heartbeat", 4096, NULL, 5, NULL);
  hb_rate = 250;
  init_bt();
  delay(1000);
  xTaskCreate(BluetoothReconnectTask, "BluetoothReconnectTask", 4096, NULL, 5, NULL);
  init_mk312_easy();
  Udp.begin(localPort);
  xTaskCreate(tellMyIP, "tellMyIP", 4096, NULL, 5, NULL);
}

void sendHeartbeat(bool b)
{
  OSCMessage heartbeat = "/heartbeat";
  int i = b;
  heartbeat.add(i);
  Udp.beginPacket(broadcastIP, outPort);
  heartbeat.send(Udp);
  Udp.endPacket();
  heartbeat.empty();
}

void handle_set_a(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0); // converting float to int because TouchOSC MK1 faders use Float values
  mk312_set_a(val);
}

void handle_set_b(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
  mk312_set_b(val);
}

void handle_set_ma(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
  mk312_set_ma(val);
}

void handle_set_mode(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
  mk312_set_mode(val);
}

void handle_set_adc(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
  if (val == 0)
  {
    mk312_disable_adc();
  }
  if (val == 1)
  {
    mk312_enable_adc();
  }
}
// following are work in progress
void handle_mod_la(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_fa(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_wa(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_ga(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_lb(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_fb(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_wb(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_mod_gb(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void handle_cfg_power_level(OSCMessage &msg)
{
  int val = (int)msg.getFloat(0);
}

void OSCMsgReceive()
{
  OSCMessage msg;
  int size;
  if ((size = Udp.parsePacket()) > 0)
  {
    while (size--)
      msg.fill(Udp.read());
    if (!msg.hasError())
    {
      // ignore brodcasts here...check ip
      if (Udp.remoteIP()[3] == 255)
        return; // ignore broadcasts
      // function group approach to optimize msg dispatch
      if (msg.match("/set/*"))
      {
        msg.dispatch("/set/a", handle_set_a);
        msg.dispatch("/set/b", handle_set_b);
        msg.dispatch("/set/ma", handle_set_ma);
        msg.dispatch("/set/mode", handle_set_mode);
        msg.dispatch("/set/adc", handle_set_adc);
      }
      if (msg.match("/mod/*"))
      {
        msg.dispatch("/mod/la", handle_mod_la); // Level
        msg.dispatch("/mod/fa", handle_mod_fa); // Frequency
        msg.dispatch("/mod/wa", handle_mod_wa); // Width
        msg.dispatch("/mod/ga", handle_mod_ga); // Gate
        msg.dispatch("/mod/lb", handle_mod_lb);
        msg.dispatch("/mod/fb", handle_mod_fb);
        msg.dispatch("/mod/wb", handle_mod_wb);
        msg.dispatch("/mod/gb", handle_mod_gb);
      }
      if (msg.match("/cfg/*"))
      {
        msg.dispatch("/cfg/pwr", handle_cfg_power_level);
      }
    }
  }
}

void loop()
{
  OSCMsgReceive();
}
