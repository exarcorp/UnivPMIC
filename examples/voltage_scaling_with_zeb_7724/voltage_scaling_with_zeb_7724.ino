/*
 * Voltage scaling with zero error bin example.
 */
 
#include <Wire.h>
#include "PowerXR.h"

#define ENABLE 1
#define DISABLE 0

byte ch_num = 0; // 0 = channel 1
float set_val = 3.1698; // 3.1698 3.1611 set voltage to 3.0V

Xrp7724 chip(0x28);


void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  chip.power_enable_supply(ch_num, DISABLE);
  delay(500);
  chip.power_enable_supply(ch_num, ENABLE);
  delay(2000);
  chip.standard_write(0x20, 16+152);
  delay(500);
}


void loop()
{

  delay(5000);
  // setting voltage for channel 1 to set_val
//  chip.power_set_voltage_ch(ch_num, set_val);
  chip.standard_write(0x20, 48+200);
//  chip.power_set_voltage_native_fine_adj_ch(ch_num, set_val);
  Serial.print("Voltage Set to : ");
  Serial.print(set_val);
  Serial.println(" V");
//  Serial.println(chip.get_fault_status());
}

