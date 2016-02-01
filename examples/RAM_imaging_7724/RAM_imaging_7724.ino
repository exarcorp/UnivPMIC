/*
 * Programming RAM image in XRP7724
 * This program gives an example of programming one record(line) from the runtime hex file into the RAM
 * The program can be expanded to download the entire RAM image by reading the runtime hex file one record
 * at a time and using download_ram() function for each record.
 */
 
#include <Wire.h>
#include "PowerXR.h"

byte record_data[19] = {0};
byte ret_val = -1;
  
Xrp7724 chip(0x64);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  chip.invalidate_flash();
}

void loop()
{
  if(ret_val == -1){ // to limit the execution to just the first time
    // creating a sample record
    record_data[0] = 0x10; // number of data bytes
    record_data[1] = 0x80; // upper byte of starting address
    record_data[2] = 0x72; // lower byte of starting address
    record_data[3] = 0x21; // assigning some data bytes
    record_data[4] = 0x22;
    record_data[5] = 0x23;
    record_data[6] = 0x24;
    ret_val = chip.download_ram(record_data); // downloading the sample record in RAM
    if(!ret_val)
      Serial.println("Record downloaded successfuly in RAM");
  }
  
}
