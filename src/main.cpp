#include <Arduino.h>
#include "PortalFramework.h"
#include "Portal.h"
#include "Constants.h"

PortalFramework framework;
Portal portal;

void setup() {
    Serial.begin(115200);

    // wait for monitor open
    delay(500);

    std::optional<std::string> frameworkInitMessage = framework.begin();
    if (!frameworkInitMessage->empty()) {
        Debug.printf("Could not initialize framework! Err: %s\n", frameworkInitMessage.value().c_str());
        pinMode(PIN_BUZZER, OUTPUT); // to be able to beep...
        portal.beep(BEEP_LONG);
        return;
    }

    if (!portal.begin(&framework)) {
        Debug.println("Could not initialize portal!");
        portal.beep(BEEP_LONG);
        return;
    }

    Debug.println("Initialized!");
}

void loop() {
    // no-op - ESP32
}
