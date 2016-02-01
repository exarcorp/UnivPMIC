/*
 * Calculating current out of raw readings for channel 1 - XRP7725
 */
 
#include <Wire.h>
#include "PowerXR.h"

#define ENABLE 1
#define DISABLE 0

float f_pll = 100e6;
float ripple = 0;
float I_Load = 0;
byte ch_num = 0; // channel 1

Xrp7725 chip(0x28);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  chip.initialize_accumulator();
  chip.power_enable_supply(ch_num, ENABLE);
}
  
void loop()
{
  delay(1000);
  // calculating ripple value based on channel values from EVB-DEMO1 config
  ripple = calc_ripple(12.0, 1.8, 2, 247.6e3, 2.2e-6); // fundamental frequency value in Hz, inductor in H
  I_Load = chip.get_average_current_ch(ch_num, ripple, 10e-3, 256); // rdson value in Ohms
  Serial.print("The final load current value is: ");
  Serial.println(I_Load);
}


// function to calculate the ripple value that is passed as a parameter to get_current_ch()
// values specific to each channel
float calc_ripple(float vin, float vout, byte f_tier, float fund_freq,  float inductor)
{
  word crs = 0; // counter restart state
  crs = 1.0/( 9.71e-9*(fund_freq) );
  
  byte freq_mul = 0;
  
  if(f_tier == 2)
    freq_mul = 1;
  else if(f_tier == 4)
    freq_mul = 2;
    
  float f = 1.0/(crs+1)*pow(2, freq_mul)*f_pll;
  float x = ((vout/vin)*(vin-vout))/(inductor*f);
  return x;
  
}
