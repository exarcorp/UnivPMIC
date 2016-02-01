#include <Wire.h>
#include "PowerXR.h"

Xrp7724 chip(0x28);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
}
  
void loop()
{
  delay(1000);
  //Serial.println( chip.get_chip_name() );
  
  delay(1000);
  chip.power_set_voltage_ch(3, 3.0);
  delay(1000);
  chip.power_enable_supply(0x00, 0x01);
  delay(1);
  chip.power_enable_supply(0x03, 0x01);
  
  delay(1000);
  Serial.print("ch1 ");
  Serial.println(chip.power_read_voltage_ch(0));
  Serial.print("ch4 ");
  Serial.println(chip.power_read_voltage_ch(3));
  Serial.print("vin ");
  Serial.println(chip.power_read_voltage_vin());
  Serial.print("curr4 ");
  Serial.println(chip.power_read_lowside_voltage_ch(3), 4);
  
  Serial.println("1------------------------");
  
  delay(2000);
  chip.power_enable_supply(0x00, 0x00);
  delay(1);
  chip.power_enable_supply(0x03, 0x00);;
  
  delay(1000);
  Serial.print("ch1 ");
  Serial.println(chip.power_read_voltage_ch(0));
  Serial.print("ch4 ");
  Serial.println(chip.power_read_voltage_ch(3));
  Serial.print("prescale4 ");
  Serial.println((chip.register_read(0xc33d)&0x30)>>4);
  Serial.println("2------------------------");

}
