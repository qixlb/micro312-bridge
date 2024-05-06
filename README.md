thanks to
https://github.com/openfuck/buttshock-arduino
https://github.com/bkifft/MK-312WS/

Everything still work in progress.
Main idea is to use OSC for control Micro312 via bt. OSC is a reliable protocol I really like, very fast and responsive. Used by a variety of multimedia software.

Second goal is use usb host capabilities of esp32 for control channel parameters using MIDI controller like APC MINI by AKAI.

Third goal is web configurator and interface, but honestly I'm more focused to bridge situation.

to do list and random ideas:
- [ ] complete parameter control for waves in mk312 library
- [ ] move status led to neopixel option
- [ ] complete pairing mode
- [ ] usb host support for midi surface
- [ ] internal esp32 user presets to store in FS and recall from MIDI pads
- [ ] finish touchosc mk2 interface
- [ ] switch from OSC to MQTT for xtoys interface
- [ ] update mk312 library to use stream lib so can switch from bluetooth to HardwareSerial comm
- [ ] easy 3 button interaction for switching modes, pairing, etc...
- [ ] add SPI LCD for info and setup
- [ ] optimize mk312 for faster control and less unnecessary data read

feel free to contribute, share, edit :)

