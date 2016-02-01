/*
 * Dynamic changes of the OCP levels based on regulated voltage levels - XRP7724.
 */
 
#include <Wire.h>
#include "PowerXR.h"

#define ENABLE 1
#define DISABLE 0

#define STA_OCPW_THRES_CH0 0xC01B
#define STA_OCPF_THRES_CH0 0xC01C

byte ch_num = 0; // 0 = channel 1
float f_pll = 100e6;
float ripple = 0;
float set_val = 5.5; // set voltage to 5.5V
float load_current = 0;
float valley_current = 0; // valley_current, ILvalley = VIL/RdsOn
float ocpf_val = 0;
float ocpw_val = 0;


Xrp7724 chip(0x28);


void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  chip.power_enable_supply(ch_num, ENABLE);
}


void loop()
{
  delay(5000);
  // setting voltage for channel 1 to 5.5V
  // this is assuming we are in the <=5.5V(10mV steps) prescaler range, we do not support dynamic prescaler value change
  chip.power_set_voltage_ch(ch_num, set_val);
  // calculating ripple value based on channel 1 values from EVB-DEMO1 config
  ripple = calc_ripple(12.0, set_val, 2, 247.6e3, 4.2e-6); // fundamental frequency value in Hz, inductor in H
  //calculate current based on the fact that power is constant at 10W
  load_current = 10/set_val; // P=VI
  valley_current = load_current - ripple/2;
  ocpf_val = 1.3 * valley_current; // ocpf is 30 % higher than max output current
  ocpw_val = 0.85 * ocpf_val; // ocpw is 85% of ocpf
  // assigning the respective registers
  if(ocpf_val > 0x7E){
     chip.register_write(STA_OCPF_THRES_CH0, 0x7E); // OCP fault handling threshold being clipped
  }
  else{
      chip.register_write(STA_OCPF_THRES_CH0, ocpf_val);
  }

  if(ocpw_val > 0x7E){
     chip.register_write(STA_OCPW_THRES_CH0, 0x7E); // OCP warning being clipped
  }
  else{
     chip.register_write(STA_OCPW_THRES_CH0, ocpw_val);
  }    
  
}

// function to calculate the ripple current value
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
