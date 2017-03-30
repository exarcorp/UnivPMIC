/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


/******************************************************************************
XRP7704 Command Library

The I2C Slave will supports only one mode of communication:
    (1) Standard
        commands will be 8 bits long and data
        transmitted will also be 8 bits long.
******************************************************************************/

#include "Arduino.h"
#include "PowerXR.h"
#include <Wire.h>

#define SET_CH_EN_CONFIG 0x97 // SET_CH_EN_CONFIG should be programmed last during chip programming
#define TOTAL_NUM_OF_RUNTIME_RECS 16 // number of records(lines) in the runtime hex file, excluding the End of File record
#define TOTAL_NUM_OF_NVM_RECS 29 // number of records(lines) in the NVM hex file, excluding the End of File record  

//<<default constructor>>
Xrp7704::Xrp7704(){
    /*nothing to construct*/
}

//<<constructor>>
Xrp7704::Xrp7704(int i2c_addr){
    _i2c_addr = i2c_addr;
}

//<<destructor>>
Xrp7704::~Xrp7704() {    
    /*nothing to destruct*/    
}

byte Xrp7704::standard_read(byte address){
    byte data = 0xFF;
  
    // send register address
    Wire.beginTransmission(_i2c_addr);
    Wire.write(address);
    Wire.endTransmission();
  
    // request one byte
    if (Wire.requestFrom(_i2c_addr, 1) == 1){
        data = Wire.read();
    }  
    return data;
}

void Xrp7704::standard_write(byte address, byte data){
    // send command/register address
    Wire.beginTransmission(_i2c_addr);
    Wire.write(address);

    // send data byte
    Wire.write(data);
    Wire.endTransmission();
}

byte Xrp7704::otp_read(byte address){
    byte data = 0xFF;

    // send command/register address
    Wire.beginTransmission(_i2c_addr);
    Wire.write(0xAE);
    Wire.write(address);
    Wire.endTransmission();

    // request one byte
    if (Wire.requestFrom(_i2c_addr, 1) == 1) {
        data = Wire.read();
    }
  
    return data;
}

void Xrp7704::otp_write(byte address, byte data){
    // send command/register address
    Wire.beginTransmission(_i2c_addr);
    Wire.write(0xAF);
    Wire.write(address);

    // send data byte
    Wire.write(data);
    Wire.endTransmission();
}



/*-----------------------------------------------------------------------------
    void program_flash(byte[]);
    function to download the NVM image (one record at a time)
    each record sent as data[]
    data[0] - no of data bytes
    data[1] - Upper byte of starting address - Always 0xAF in this case
    data[2] - Lower byte of starting address
    data[3] onwards - data bytes that are part of the record

    Reset the chip after complete programming for the changes to take affect
-----------------------------------------------------------------------------*/
byte Xrp7704::program_flash(byte data[]){
    static byte num_recs_downloaded = 0; // to keep track of number of records(lines) from the NVM hex file downloaded so far
    byte num_bytes_in_rec = 0; 
    byte starting_addr_rec = 0x00; // the lower byte of the starting address in the record - the register address we start programming at
    byte reg_addr = 0x00;
    byte reg_data = 0x00;
    byte readback_val = 0x00;
    byte flash_prog_fail_flag = 0;

    // read the bytes coming in from the NVM hex file
    // this section is written assuming the number of bytes and starting address are sent before the actual data bytes in each record
    // each record sent as data[]
    // data[0] - no of data bytes
    // data[1] - Upper byte of starting address - Always 0xAF in this case
    // data[2] - Lower byte of starting address
    // data[3] onwards - data bytes that are part of the record
    num_bytes_in_rec = data[0];
    starting_addr_rec = (data[2] & 0xFF);
    for(byte i = 0; i < num_bytes_in_rec; i++){
        reg_addr = starting_addr_rec + i;
        reg_data = data[3+i];
        otp_write(reg_addr, reg_data);
        delay(100);
        readback_val = otp_read(reg_addr);
        delay(50);
        if(readback_val != reg_data){
            Serial.println("Error, data not written to NVM correctly");
            Serial.print("Written Value: ");
            Serial.println(reg_data);
            Serial.print("Read Back Value: ");
            Serial.println(readback_val);
            Serial.print("Address: ");
            Serial.println(reg_addr);
            // flash_prog_fail_flag = 1;
            // break; 
        }
    }

    if(flash_prog_fail_flag){
        Serial.println("Error, data not written to NVM correctly");
        Serial.print("Written Value: ");
        Serial.println(reg_data);
        Serial.print("Read Back Value: ");
        Serial.println(readback_val);
        Serial.print("Address: ");
        Serial.println(reg_addr);
        return 1;
    }
    else{
        num_recs_downloaded++;
        if(num_recs_downloaded == TOTAL_NUM_OF_NVM_RECS){
            Serial.println("NVM image downloaded successfully");
            Serial.println("Reset the chip for the changes to take affect");

        }
    }
    return 0;

}

/*-----------------------------------------------------------------------------
    void download_ram(byte[]);
    function to download the RAM image (one record at a time)
    each record sent as data[]
    data[0] - no of data bytes
    data[1] - Upper byte of starting address - Always 0x00 in this case
    data[2] - Lower byte of starting address
    data[3] onwards - data bytes that are part of the record
-----------------------------------------------------------------------------*/
byte Xrp7704::download_ram(byte data[]){
    static byte num_recs_downloaded = 0; // to keep track of number of records(lines) from the runtime hex file downloaded so far
    byte random_register = 0x09;
    byte random_value = 0x42;
    byte num_bytes_in_rec = 0; 
    byte starting_addr_rec = 0x00; // the lower byte of the starting address in the record - the register address we start programming at
    byte reg_addr = 0x00;
    byte reg_data = 0x00;
    byte readback_val = 0x00;
    byte ram_dump_fail_flag = 0;
    byte saved_val = 0;

    // read the bytes coming in from the runtime hex file
    // this section is written assuming the number of bytes and starting address are sent before the actual data bytes in each record
    // each record sent as data[]
    // data[0] - no of data bytes
    // data[1] - Upper byte of starting address - Always 0x00 in this case
    // data[2] - Lower byte of starting address
    // data[3] onwards - data bytes that are part of the record
    num_bytes_in_rec = data[0];
    starting_addr_rec = (data[2] & 0xFF);
    for(byte i = 0; i < num_bytes_in_rec; i++){
        reg_addr = starting_addr_rec + i;
        reg_data = data[3+i];
        //only program SET_CH_EN_CONFIG at the end
        if(reg_addr != SET_CH_EN_CONFIG){
            standard_write(reg_addr, reg_data);
            readback_val = standard_read(reg_addr);
            if(readback_val != reg_data){
                ram_dump_fail_flag = 1;
                break; 
            }
        }
        else{ // save value to be programmed into SET_CH_EN_CONFIG later
            saved_val =  reg_data;
        } 
    }

    if(standard_read(random_register) != random_value){
        Serial.println("Error, Chip reset while downloading");
        return 1;
    }
    if(ram_dump_fail_flag){
        Serial.println("Error, data not written to RAM correctly");
        Serial.print("Written Value: ");
        Serial.println(reg_data);
        Serial.print("Read Back Value: ");
        Serial.println(readback_val);
        Serial.print("Address: ");
        Serial.println(reg_addr);
        return 1;
    }
    else{
        num_recs_downloaded++;
        if(num_recs_downloaded == TOTAL_NUM_OF_RUNTIME_RECS){
            standard_write(SET_CH_EN_CONFIG, saved_val); // enable channels after image has been downloaded
            Serial.println("RAM image downloaded successfully");
        }
    }
    return 0;
}
