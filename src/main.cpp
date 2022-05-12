#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    // wait for monitor open
    delay(500);
}

void loop() {
    // no-op - ESP32
}
