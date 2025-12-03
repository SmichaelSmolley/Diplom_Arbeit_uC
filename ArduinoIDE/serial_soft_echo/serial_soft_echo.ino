#include <Arduino.h>

#define baud 9600 

void setup(){
    Serial.begin(9600);
    while(!Serial){;}
    Serial.printf("Config: %i, %ol", baud, Serial.getTimeout());
}

void loop(){
    static const size_t bufsize = 128;
    static size_t index = 0;
    static char msg[bufsize] = {0};

    while(Serial.available() > 0){
        char incomming_byte = (char)Serial.read();

        if(incomming_byte == '\n' || incomming_byte == '\r')
        {
            if(index > 0){
                Serial.write(msg,index);
                index = 0;
            }
            continue;
        }

        msg[index++] = incomming_byte;
        if(index >= bufsize){
            Serial.write(msg,bufsize);
            Serial.printf("\nBuffer overflow, data truncated\n");
            index = 0;
        }
    }
}