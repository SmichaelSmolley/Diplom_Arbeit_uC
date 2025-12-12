/**dummy code
*es können zwei buffer verwendet werden bei hohen datenraten
*dma befüült buffer A, cpu bearbeitet buffer B, dma befüllt buffer B, cpu bearbeiutet A
*/
#include <SPI.h>

#define SAMPLE_BUFFER_SIZE 16
#define SPI_CS 5
uint8_t dmabuffer[SAMPLE_BUFFER_SIZE*3]; // 16 samples speichern und 3 byte pro sample
uint8_t dummybuffer[SAMPLE_BUFFER_SIZE*3] = {0};
volatile bool dmaDone = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(18,19,23); // SCLK, MISO, MOSI, CS
  initDMA(dmabuffer, SAMPLE_BUFFER_SIZE);
  dmaTransaction.tx_buffer = dummybuffer;
  initTimer(1000);
}


void IRAM_ATTR onTimer(){ // IRAM_ATTR sagt das es im internen ram abgelegt wird welches schneller ist
  startSPIDMATransfer();
}

void IRAM_ATTR startSPIDMATransfer(){
  digitalWrite(SPI_CS,LOW);
  spi_device_transmit_dma(spiHandle, &dmaTrandaction);
}

void IRAM_ATTR onDMATransfcompleate(){
  dmaDone = true;
  digitalWrite(SPI_CS,HIGH);
}

void process(){
  for(int i{0}, i < SAMPLE_BUFFER_SIZE; i++){
    uint32_t sample = ((dmabuffer[i*3] << 16) | (dmabuffer[i*3+1] << 8) | dmabuffer[i*3+2]); 

    /**
    *i = 0
dmabuffer[0]   → erstes Byte des Sample 0 (MSB)
dmabuffer[1]   → zweites Byte des Sample 0
dmabuffer[2]   → drittes Byte des Sample 0 (LSB)
    *i = 1
dmabuffer[3] → erstes Byte Sample 1
dmabuffer[4] → zweites Byte Sample 1
dmabuffer[5] → drittes Byte Sample 1
    *
    */
    Serial.println(sample);
  }
  dmaDone = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bufferReady())
  {
    process();
  }
}
