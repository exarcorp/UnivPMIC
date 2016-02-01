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
XR77128 Command Library

The I2C Slave will support 2 modes of communication:
    (1) Standard
        Commands will be 8 bits long (with addresses less than 0x80) and data
        will be transmitted in word (16 bit) lengths.
    
    (2) Register
        Addresses represented in 16 bits but only transmit data in byte (8 bit)
        lengths. The High Byte of the register addresses will always be > 0x80.
    
The XRP7725 can support 4 types of commands:
    (1) System
        Monitor and manage the XRP7725, and interrupts and faults generated by
        it.
        
    (2) Power
        Control, monitor and adjust power parameters in a running
        system.
        
    (3) IO
        Control direction and polarity of GPIOs and PSIOs.
    
    (4) Flash
        Program customer configurations into NVM.
******************************************************************************/

#include "Arduino.h"
#include "PowerXR.h"
#include <Wire.h>

#define SET_VOLTAGE_CH_CONST 0.0125
#define PWR_SET_VOLTAGE_CH1 0x20
#define HW_DYN_PRESCALER_CH0 0xC03D 
        
//<<constructor>>
Xr77128::Xr77128(int i2c_addr){
    _i2c_addr = i2c_addr;
}

//<<destructor>>
Xr77128::~Xr77128(){    
    /*nothing to destruct*/    
}


void Xr77128::power_set_voltage_ch(byte channel, float set_value) {
    byte prescale_shift = (register_read(HW_DYN_PRESCALER_CH0 + (channel * 0x100)) & 0x30) >> 4;
    float prescale = get_prescale_voltage(channel);
    float vref = word( float(set_value) / (prescale * 5) + 0.000001 ) * (prescale * 5);
    word fine_adj = min(word(round((float(set_value) - vref) / prescale)), 0xF);
    word vref_value = min(word( (float(set_value)/SET_VOLTAGE_CH_CONST) + 0.000001 ), 0x1B8);
    standard_write((PWR_SET_VOLTAGE_CH1 + channel), (fine_adj << (12 + prescale_shift)) + vref_value);
}