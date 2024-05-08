// based on https://github.com/Rubberfate/mk312com/blob/master/mk312/constants.py
#ifndef MK312_H
#define MK312_H
#pragma once




//Memory addresses
#define ADDRESS_R15 0x400f
#define ADDRESS_ADC_POWER 0x4062
#define ADDRESS_ADC_BATTERY 0x4063
#define ADDRESS_LEVELA 0x4064
#define ADDRESS_LEVELB 0x4065
#define ADDRESS_PUSH_BUTTON 0x4068
#define ADDRESS_COMMAND_1 0x4070
#define ADDRESS_COMMAND_2 0x4071
#define ADDRESS_MA_MIN_VALUE 0x4086
#define ADDRESS_MA_MAX_VALUE 0x4087
#define ADDRESS_CURRENT_MODE 0x407b
#define ADDRESS_LCD_WRITE_PARAMETER_1 0x4180
#define ADDRESS_LCD_WRITE_PARAMETER_2 0x4181
#define ADDRESS_POWER_LEVEL 0x41f4
#define ADDRESS_BATTERY_LEVEL 0x4203
#define ADDRESS_LEVELMA 0x420D
#define ADDRESS_KEY 0x4213

#define ADDRESS_RAMP_LEVEL 0x41f8
#define ADDRESS_RAMP_TIME 0x41f9

//Modulators addresses
#define ADDRESS_levela 0x40A5 
#define ADDRESS_freqa 0x40AE 
#define ADDRESS_widtha 0x40B7 
#define ADDRESS_gatea 0x4090
#define ADDRESS_levelb 0x41A5 
#define ADDRESS_freqb 0x41AE 
#define ADDRESS_widthb 0x41b7 
#define ADDRESS_gateb 0x4190 


// EEPROM addresses
#define EEPROM_ADDRESS_POWER_LEVEL 0x8009
#define EEPROM_ADDRESS_FAVORITE_MODE 0x800C

// Commands
#define COMMAND_START_FAVORITE_MODULE 0x00
#define COMMAND_EXIT_MENU 0x04
#define COMMAND_SHOW_MAIN_MENU 0x0a
#define COMMAND_NEW_MODE 0x12
#define COMMAND_WRITE_STRING_TO_LCD 0x15
#define COMMAND_NO_COMMAND 0xff

#define COMMAND_START_RAMP 0x21

// Modes
#define MODE_POWERON 0x00
#define MODE_UNKNOWN 0x01
#define MODE_WAVES 0x76c
#define MODE_STROKE 0x77
#define MODE_CLIMB 0x78
#define MODE_COMBO 0x79
#define MODE_INTENSE 0x7a
#define MODE_RYTHM 0x7b
#define MODE_AUDIO1 0x7c
#define MODE_AUDIO2 0x7d
#define MODE_AUDIO3 0x7e
#define MODE_SPLIT 0x7f
#define MODE_RANDOM1 0x80
#define MODE_RANDOM2 0x81
#define MODE_TOGGLE 0x82
#define MODE_ORGASM 0x83
#define MODE_TORMENT 0x84
#define MODE_PHASE1 0x85
#define MODE_PHASE2 0x86
#define MODE_PHASE3 0x87
#define MODE_USER1 0x88
#define MODE_USER2 0x89
#define MODE_USER3 0x8a
#define MODE_USER4 0x8b
#define MODE_USER5 0x8c
#define MODE_USER6 0x8d
#define MODE_USER7 0x8e

// Power Level
#define POWERLEVEL_LOW 0x01
#define POWERLEVEL_NORMAL 0x02
#define POWERLEVEL_HIGH 0x03

// Register 15 Bits
#define REGISTER_15_ADCDISABLE 0

// Buttons
#define BUTTON_MENU 0x80
#define BUTTON_OK 0x20
#define BUTTON_RIGHT 0x40
#define BUTTON_LEFT 0x10


//deprecated void init_mk312();
void mk312_sync();
void mk312_inc_a();
void mk312_inc_b();
void mk312_inc_ma();
void mk312_dec_a();
void mk312_dec_b();
void mk312_dec_ma();
void mk312_set_a(int percent);
void mk312_set_b(int percent);
void mk312_set_ma(int percent);
void mk312_set_mode(byte newmode);
void mk312_set_la(int percent);
void mk312_set_fa(int percent);
void mk312_set_wa(int percent);
void mk312_set_ga(int percent);
void mk312_set_lb(int percent);
void mk312_set_fb(int percent);
void mk312_set_wb(int percent);
void mk312_set_gb(int percent);
void mk312_set_powerlevel(byte newmode);



int mk312_get_a();
int mk312_get_b();
int mk312_get_ma();
byte mk312_get_mode();

void mk312_all_off();

void mk312_enable_adc();
void mk312_disable_adc();
bool mk312_get_adc_disabled();
byte mk312_get_ramp_level();
byte mk312_get_ramp_time();
void mk312_set_ramp_level();
void mk312_set_ramp_time();
void mk312_ramp_start();
int mk312_get_battery_level();
void init_mk312_easy();
//void mk312_bruteforce_ramp();

#endif //MK312_H

//other addr from venerate
#define ETMEM_knobmamin 0x4086
#define ETMEM_knobmamax 0x4087
#define ETMEM_timeona 0x4098
#define ETMEM_timeoffa 0x4099
#define ETMEM_timeopta 0x409A 
#define ETMEM_ramp 0x4006 // 0-255 also ramp at 400b, 409c, 419c
#define ETMEM_levelmina 0x40A6
#define ETMEM_levelmaxa 0x40A7
#define ETMEM_levelratea 0x40A8
#define ETMEM_levelopta 0x40AB
#define ETMEM_freqmaxa 0x40AF
#define ETMEM_freqmina 0x40B0
#define ETMEM_freqopta 0x40B5
#define ETMEM_widthmina 0x40B8
#define ETMEM_widthmaxa 0x40B9
#define ETMEM_widthopta 0x40BE
#define ETMEM_timeonb 0x4198
#define ETMEM_timeoffb 0x4199
#define ETMEM_timeoptb 0x419A
#define ETMEM_levelminb 0x41A6
#define ETMEM_levelmaxb 0x41A7
#define ETMEM_levelrateb 0x41A8
#define ETMEM_leveloptb 0x41AB
#define ETMEM_freqmaxb 0x41AF
#define ETMEM_freqminb 0x41B0
#define ETMEM_freqoptb 0x41B5
#define ETMEM_widthminb 0x41B8
#define ETMEM_widthmaxb 0x41B9
#define ETMEM_widthoptb 0x41BE
#define ETMEM_powerlevel 0x41F4
#define ETMEM_ramptime 0x41F9
#define ETMEM_knobmaset 0x420d
