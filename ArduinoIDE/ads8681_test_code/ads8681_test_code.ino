#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include "ADS8681.h"
#define baud 9600
#define ADS8681_CS_PIN 2
ADS8681 adc(ADS8681_CS_PIN);

void setup() {
  pinMode(16, INPUT);
  Serial.begin(baud);
  uint32_t range_sel_reg = 0;
  range_sel_reg |= (uint32_t)
        ADS8681_RANGE_SEL_UP_1_25_VREF
        & ADS8681_RANGE_SEL_MASK
        << ADS8681_RANGE_SEL_SHIFT;
  adc.spiSend(
        ADS868X_SPI_COMMAND_WRITE_FULL,
        ADS868X_REGISTER_ADDRESS_RANGE_SEL,
        range_sel_reg);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t ret = adc.adcRead();
  //Serial.printf("%#08x \t", ret);
  //Serial.printf("%d \t", ret);
  //Serial.printf("%lf \t", (((4.096*0.625)/65536)*ret)-((4.096*0.625)/65536));
  double middle = (((4.096*1.25)/65536)*ret);
  for(int i = 0; i < 256; i++)
  {
    ret = adc.adcRead();
    middle = (((4.096*1.25)/65536)*ret) + middle;
  }
  Serial.printf("%f \n", middle/256);
  delay(100);
}
