/*
 * Sweeping through startup and shutdown ramp rates - XRP7724
 * For the purposes of this code example, Channel 1 and 3 have been pre-configured to be in Group 1
 */
 
#include <Wire.h>
#include "PowerXR.h"

#define ENABLE 1
#define DISABLE 0

// register definitions
#define DYN_UP_SLOPE_CH0 0xC033 // registers to define ramp-up speed
#define DYN_UP_SLOPE_CH2 0xC233
#define PWRENSEQUPDATA_0_DELAY 0x8079 // registers to specify delay (ms) after enabling or disabling while sequencing
#define PWRENSEQDOWNDATA_0_DELAY 0x8085
#define DYN_DOWN_SLOPE_CH0 0xC034 // // registers to define ramp-down speed
#define DYN_DOWN_SLOPE_CH2 0xC234



byte group_num = 1; // sequencing group 1, channels 1 and 3 are pre-configured to be in Group 1

Xrp7724 chip(0x28);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  Serial.println(chip.standard_read(0x01)); // command GET_REV_ID
}
  
void loop()
{
  delay(1000);
  //ramping up example
  Serial.println("Ramping up");
  chip.register_write(DYN_UP_SLOPE_CH0, 5);  // assigning random value for ramp-up speed for channel 1 (channel 1 and 3 are in Group 1)
  delay(100);
  chip.register_write(PWRENSEQUPDATA_0_DELAY, 0); // Time to delay (ms) after enabling or disabling while sequencing
  delay(100);
  for(int i = 5; i < 10; i++){
    chip.register_write(DYN_UP_SLOPE_CH2, i*20); // changing ramp-up speed for channel 3
    delay(100);
    chip.power_enable_supply_group(group_num, ENABLE);
    delay(1000);
    chip.power_enable_supply_group(group_num, DISABLE);
    delay(1000);
  }  
  
  delay(2000);
  //ramping down example
  Serial.println("Ramping down");
  chip.register_write(DYN_DOWN_SLOPE_CH0, 5); // assigning random value for ramp-down speed for channel 1 (channel 1 and 3 are in Group 1)
  delay(100);
  chip.register_write(PWRENSEQDOWNDATA_0_DELAY, 0); 
  delay(100);
  for(int i = 5; i < 10; i++){
    chip.register_write(DYN_DOWN_SLOPE_CH2, i*20); // changing ramp-down speed for channel 3
    delay(100);
    chip.power_enable_supply_group(group_num, ENABLE);
    delay(1000);
    chip.power_enable_supply_group(group_num, DISABLE);
    delay(1000);
  }    
  
}

