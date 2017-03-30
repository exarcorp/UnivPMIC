/*
 * Programming RAM image in XRP7704
 * This program gives an example of programming one record(line) from the runtime hex file into the RAM
 * The program can be expanded to download the entire RAM image by reading the runtime hex file one record
 * at a time and using download_ram() function for each record.
 */
 
#include <Wire.h>
#include "PowerXR.h"

#define SET_CH_EN_CONFIG 0x97
#define SET_STBLDO_EN_CONFIG 0x98
#define SET_TEST_IGNORE_FAULT 0xAA

byte record_data[19] = {0};
byte ret_val = -1;
  
Xrp7704 chip(0x00);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  
  // Turn off the regulating Supplies before reprogramming
  chip.standard_write(SET_CH_EN_CONFIG, 0x00);
  chip.standard_write(SET_STBLDO_EN_CONFIG, 0x00);
  chip.standard_write(SET_TEST_IGNORE_FAULT, 0x1F); // program fault ignores first
}

void loop()
{
  if(ret_val == -1){ // to limit the execution to just the first time
    // creating a sample record
    record_data[0] = 0x03; // number of data bytes
    record_data[1] = 0x00; // upper byte of starting address - always 0x00 in this case
    record_data[2] = 0x0D; // lower byte of starting address
    record_data[3] = 0x00; // assigning some data bytes
    record_data[4] = 0x04;
    record_data[5] = 0x00;
    record_data[6] = 0xDD;
    ret_val = chip.download_ram(record_data); // downloading the sample record in RAM
    if(!ret_val)
      Serial.println("Record downloaded successfuly in RAM");
  }
  
}
