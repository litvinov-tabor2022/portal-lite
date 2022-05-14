#include <Arduino.h>
#include "PortalFramework.h"
#include "Portal.h"

PortalFramework framework;
Portal portal;

void setup() {
    Serial.begin(115200);

    // wait for monitor open
    delay(500);

    std::optional<std::string> frameworkInitMessage = framework.begin();
    if (!frameworkInitMessage->empty()) {
        Debug.printf("Could not initialize framework! Err: %s\n", frameworkInitMessage.value().c_str());
        return;
    }

    if (!portal.begin(&framework)) {
        Debug.println("Could not initialize portal!");
        return;
    }

    Debug.println("Initialized!");
}

void loop() {
    // no-op - ESP32
}
