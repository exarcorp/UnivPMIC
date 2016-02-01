/*
 * XR Programmer - standalone programmer to program a hex file
 *
 * Copyright (c) 2014 Exar Corporation, Inc.
 *
 */

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
 
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "PowerXR.h"


char incomingByte = 0;
char carray[5] = {0};
word index = 0;
word EEsize = 1024; // size in bytes of Arduino Uno's EEPROM
word value = 0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean numbytesReceived = false; //to receive the number of entries first
byte hexfilePresent = 0; // magic value put in EEPROM to signify that a hex file has been loaded in - value of 13 is put in to show successful loading
word numbytesFile = 0; // no of bytes in hex file received and put in EEPROM
byte buttonPin = 12;   // pushbutton connected to digital pin 7
byte pgmErrorPin = 11;   // LED connected to digital pin 11
byte pgmDonePin = 10;   // LED connected to digital pin 10
byte *flash_values = NULL; // pointer to block of memory that stores array of flash values from hex file
byte pgm_flash_ret_val = -1; // variable to store return value of program_flash() function
Xrp7724 chip(0x28);


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  inputString.reserve(5);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW);
  pinMode(pgmDonePin, OUTPUT);
  digitalWrite(pgmDonePin, LOW);
  pinMode(pgmErrorPin, OUTPUT);
  digitalWrite(pgmErrorPin, LOW);
}

void loop()
{
  delay(10);
  // condition to check if a complete value received in serialEvent()
  if(stringComplete == true)
  {
    // just happens the first time, initial transaction to determine the number of bytes in the hex file
    if(numbytesReceived == false) 
    {
      inputString.toCharArray(carray, sizeof(carray));
      numbytesFile = atoi(carray);
      // putting the number of bytes in last location of EEPROM
      EEPROM.write(1022, highByte(numbytesFile));
      EEPROM.write(1023, lowByte(numbytesFile));
   
      numbytesReceived = true;
      stringComplete = false;
    }
    // happens for all values received other than the first (which is number of bytes)
    else
    {
      inputString.toCharArray(carray, sizeof(carray));
      //converting zero padded three character string to integer value
      value = atoi(carray);
      // storing data as one byte of integers in EEPROM
      EEPROM.write(index, lowByte(value)); 
      index++;
      stringComplete = false;
      if(index >= numbytesFile){
        if(flash_values != NULL){
          free(flash_values);
          flash_values = NULL;
        }
        // dynamically allocating memory according to number of bytes received
        flash_values = (byte *) malloc(numbytesFile * sizeof(byte)); 

        for(int j = 0; j < numbytesFile; j++){ 
          // sending the values read from EEPROM over the serial port, used for testing in XR Programmer
          Serial.write(EEPROM.read(j));
        }
        // for now, numbytesFile equal to 1024, it means we are programming the erase_hex file, not considered a proper hex file
        // if numbytesFile is not equal to 1024, it means genuine hex file programmed, hexfilePresent set to magic value 13 to show that all bytes of hex file properly stored in EEPROM
        if(numbytesFile != 1024){
          hexfilePresent = 13; 
          // writing flag value to EEPROM
          EEPROM.write(1021, hexfilePresent);
        }
      }
    }
  }
  
  // checking for button press
  if(digitalRead(buttonPin) == LOW) 
  {
    digitalWrite(pgmDonePin, LOW);
    digitalWrite(pgmErrorPin, LOW);
  
    Serial.println("Starting Programming Flash");
    
    // if hex file isn't programmed correctly
    if(EEPROM.read(1021) != 13){ // only 13 means successful loading
      Serial.println("Hex file hasn't been programmed correctly using XR programmer ");
      // both LED's high mean either hex file wasn't programmed or not enough memory
      digitalWrite(pgmDonePin, HIGH); 
      digitalWrite(pgmErrorPin, HIGH);
      delay(20);
    }
    // if hex file is programmed correctly
    else{
      // on resetting arduino, heap memory is cleared, so need to reallocate
      if(flash_values == NULL){
       // retreiving the number of bytes in hex file (stored at the last location of EEPROM)
       numbytesFile = ((EEPROM.read(1022)<<8)|EEPROM.read(1023));
       // dynamically allocating memory according to number of bytes received
       flash_values = (byte *) malloc(numbytesFile * sizeof(byte));
      }
        
      if(flash_values != NULL){
        for(int k = 0; k < numbytesFile; k++){
          // putting the bytes stored in EEPROM into an array that is passed to program_flash() in Xrp7724.cpp
          flash_values[k] =  EEPROM.read(k);
        }
        // blinking LED a couple of times to indicate that beginning to program
        digitalWrite(pgmDonePin, HIGH);
        delay(250);
        digitalWrite(pgmDonePin, LOW);
        delay(250);
        digitalWrite(pgmDonePin, HIGH);
        delay(250);
        digitalWrite(pgmDonePin, LOW);
        delay(250);
        digitalWrite(pgmDonePin, HIGH);
        delay(250);
        digitalWrite(pgmDonePin, LOW);
        // calling program_flash() in Xrp7724.cpp
        pgm_flash_ret_val = chip.program_flash(flash_values, numbytesFile);
        if(pgm_flash_ret_val == 0)
          digitalWrite(pgmDonePin, HIGH);
        else
          digitalWrite(pgmErrorPin, HIGH);
      }
      else{
        Serial.println("The Arduino couldn't allocate enough memory to store all bytes of the hex file ");
        // both LED's high mean either hex file wasn't programmed or not enough memory
        digitalWrite(pgmDonePin, HIGH); 
        digitalWrite(pgmErrorPin, HIGH);
      }
    }
    // to send a message to XR programmer to close the serial port, one programming cycle completed
    Serial.println("Done"); 
  }

}

// serialEvent() is called after loop() everytime
void serialEvent()
{
  delay(10);
  inputString = "";
  if (Serial.available())  
  {
    // just happens in the beginning to recevie the length of the hex file
    if(numbytesReceived == false) 
    {
      for(int i=0; i<4; i++)
      {
        if(Serial.available())
        {
          char inChar = (char)Serial.read(); 
          // add it to the inputString
          inputString += inChar; 
        }
      }
      // setting flag to indicate that a value has been received
      stringComplete = true;
    }
    // happens every time after the first to receive the hex file byte values one by one
    // each value is converted to a zero-padded three character string before being sent by the XR Programmer
    else
    {
      inputString = "";
      for(int i=0; i<3; i++)
      {
        if(Serial.available())
        {
          char inChar = (char)Serial.read(); 
          // add it to the inputString
          inputString += inChar;  
        }
      }
      // setting flag to indicate that a value has been received
      stringComplete = true;
    }
  }
}


