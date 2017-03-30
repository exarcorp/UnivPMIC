#include <Wire.h>
#include "PowerXR.h"

Xrp7724 chip(0x28); //assuming i2c_addr is 0x28
int ret_val = -1;
char ch = 0;

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Press 'x' to begin clear and erase cycles....");
}
  
void loop()
{
  delay(1000);
  ch = Serial.read(); // waiting for keyboard input 'x' to begin clear and erase cycles
  if(ch=='x'){
    for(int i=0; i<=6; i++){
      Serial.println("-------------------------");
      Serial.print("Clearing Page: ");
      Serial.println(i);
      Serial.println("-------------------------");
      ret_val = chip.clear_flash_page(i); // function based on ANP-38
      delay(50);
      if(ret_val == 0)
        Serial.println(" Page Successfully Cleared ");
      
      Serial.println("-------------------------");
      Serial.print("Erasing Page: ");
      Serial.println(i);
      Serial.println("-------------------------");
      for(int j=0; j<4; j++){ //num of cycles for erase
        Serial.print(" Cycle: ");
        Serial.println(j+1);
        ret_val = chip.erase_flash_page(i); // function based on ANP-38
        delay(50);
        if(ret_val == 0)
          Serial.println(" Page Successfully Erased ");
      }    
    }
    
    Serial.println("------- Finished Clear and Erase---------");
    delay(500);
  }
}
