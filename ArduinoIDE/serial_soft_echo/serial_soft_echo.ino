
#include <Arduino.h>


void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }
    Serial.write("ja bin da\n\r");
}

void loop() {
    constexpr size_t bufferSize = 128;
    static char buffer[bufferSize];
    static size_t index = 0;

    while (Serial.available() > 0) {
        char incomingByte = (char)Serial.read();

        // End of line -> send collected bytes (if any) and reset
        if (incomingByte == '\n' || incomingByte == '\r') {
            if (index > 0) {
                Serial.write(buffer, index);
                index = 0;
            }
            // ignore bare newline characters otherwise
            continue;
        }

        // Store byte and check for buffer full
        buffer[index++] = incomingByte;
        if (index >= bufferSize) {
            Serial.write(buffer, bufferSize);
            index = 0;
        }
    }

    // Do not automatically flush partial data here; wait for newline or buffer full
}