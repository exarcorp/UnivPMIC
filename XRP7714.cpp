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
XRP7714 Command Library

The I2C Slave will supports only one mode of communication:
    (1) Standard
        commands will be 8 bits long and data
        transmitted will also be 8 bits long.
******************************************************************************/

#include "Arduino.h"
#include "PowerXR.h"
#include <Wire.h>


//<<constructor>>
Xrp7714::Xrp7714(int i2c_addr){
    _i2c_addr = i2c_addr;
}

//<<destructor>>
Xrp7714::~Xrp7714() {    
    /*nothing to destruct*/    
}
