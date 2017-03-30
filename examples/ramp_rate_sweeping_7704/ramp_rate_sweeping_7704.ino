/*
 * Sweeping through startup and shutdown ramp rates - XRP7704
 * This example assumes only channel 1 is being used in the system.
 * If other channels are being used, bit masking and bit-wise operations should be used as required, see ANP-31 Appendix A register map for details.
 */
 
#include <Wire.h>
#include "PowerXR.h"

// registers to define startup and shutdown delay time and ramp increment/decrement time for channel 1
#define SS_RISE_LB_CH1 0x23 
#define SS_RISE_HB_CH1 0x24
#define PD_FALL_LB_CH1 0x21
#define PD_FALL_HB_CH1 0x22

#define ENABLE_CH 0xA6 // to enable or disable channels
#define CH_EN_CONFIG 0x97 // to configure "ENABLE" on a channel 

Xrp7704 chip(0x00); // initializing instance of Xrp7704 class with i2c address 0x00

void setup()
{  
  Wire.begin();
  Serial.begin(9600);
}
  
void loop()
{
  delay(5000);
  chip.standard_write(CH_EN_CONFIG, 0); // to configure channel 1 to be I2C controlled channel
  
  // ramping up example
  Serial.println("Ramping up");
  // for zero delay and 1V Vout total rise time of 5.1 ms, SS_RISE_HB_CH1[7:2] = 6'h0
  // and {SS_RISE_HB_CH1[1:0], SS_RISE_LB_CH1[7:0]} = 10'h0FF
  // refer to ANP-31 Appendix A register map for calculation details
  chip.standard_write(SS_RISE_HB_CH1, 0x00); // for zero delay
  chip.standard_write(SS_RISE_LB_CH1, 0xFF); // for a 1V rise time of 5.1ms
  chip.standard_write(ENABLE_CH, 0x01); // enable channel 1
  delay(2000); // waiting 2 sec before disabling the channel  
  
  // ramping down example
  Serial.println("Ramping down");
  // for 500 us delay and 1V Vout total fall time of 40 us, PD_FALL_HB_CH1[7:2] = 6'h2
  // and {PD_FALL_HB_CH1[1:0], PD_FALL_LB_CH1[7:0]} = 10'h002
  // refer to ANP-31 Appendix A register map for calculation details
  chip.standard_write(PD_FALL_HB_CH1, 0x08); // for 500 us delay
  chip.standard_write(PD_FALL_LB_CH1, 0x02); // for a 1V fall time of 40us
  chip.standard_write(ENABLE_CH, 0x00); // disable channel
}

