/*
 * Dynamic changes of the OCP levels based on regulated voltage levels - XRP7714.
 */

#include <Wire.h>
#include "PowerXR.h"

#define ENABLE 1
#define DISABLE 0

#define VOUT_TARGET_CH1 0x20
#define VIOUT_MAX_CH1 0x25
#define OVVP_REG 0xA2

float ripple = 0;
float set_val = 3.3; // set voltage to 3.3V
float load_current = 0;
int viout_max_val = 0;


Xrp7714 chip(0x00);

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
  chip.standard_write(OVVP_REG, 0x00); // set OVP to max(600mv above target voltage)
}

void loop()
{
  delay(5000);
  // setting voltage for channel 1 to 3.3V
  chip.standard_write(VOUT_TARGET_CH1, 0x42);
  // calculating ripple value based on channel 1 values from XRP7714EVB config
  ripple = calc_ripple(12.0, set_val, 300e3, 4.89e-6); // fundamental frequency value in Hz, inductor in H
  //calculate current based on the fact that power is constant at 10W
  load_current = 10/set_val; // P=VI
  viout_max_val = calc_viout_max(load_current, ripple, 30e-3)/0.005;
  // Maximum VIout allowed is viout_max_val with OCP warning limit set at 10 mV below VIout max
  chip.standard_write(VIOUT_MAX_CH1, viout_max_val); // bit[7:6] define the voltage level below Viout max allowed, 0x00 in this case for 10mV
}

// function to calculate the ripple current value
// values specific to each channel
float calc_ripple(float vin, float vout, float freq,  float inductor)
{
  return ((vout*(vin-vout))/(vin*freq*inductor));
}

float calc_viout_max(float imax, float ripple, float low_fet_r)
{
  return (imax + ripple/2) * (low_fet_r * 1.5);
}

