/*
 * Programming NVM in XRP7704
 * This program gives an example of programming one record(line) from the NVM hex file into the flash(NVM)
 * The program can be expanded to download the entire image by reading the NVM hex file one record
 * at a time and using program_flash() function for each record.
 * Reset the chip after complete programming for the changes to take affect.
 */
 
#include <Wire.h>
#include "PowerXR.h"

byte record_data[19] = {0};
byte ret_val = -1;
  
Xrp7704 chip(0x00);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  if(ret_val == -1){ // to limit the execution to just the first time
    // creating a sample record
    record_data[0] = 0x03; // number of data bytes
    record_data[1] = 0xAF; // upper byte of starting address - always 0xAF in this case
    record_data[2] = 0x1A; // lower byte of starting address
    record_data[3] = 0x00; // assigning some data bytes
    record_data[4] = 0x07;
    record_data[5] = 0x01;
    record_data[6] = 0x76;
    ret_val = chip.program_flash(record_data); // programming the sample record in NVM
    if(!ret_val)
      Serial.println("Record programmed successfuly in NVM");
  }
  
}
