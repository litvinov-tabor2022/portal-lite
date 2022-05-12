#include <mutex>
#include <Wire.h>
#include "Display.h"
#include "HwLocks.h"
#include "Tasker.h"
#include "debugging.h"

bool Display::begin() {
    {
        std::lock_guard<std::mutex> lg(HwLocks::I2C);

        if (!this->display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            return false;
        }

        Debug.println("Display initialized");

        display.clearDisplay();
        display.setTextSize(3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 10);
        display.println(F("Cekej..."));
        display.display();
    }

    return true;
}

void Display::draw() {
    std::lock_guard<std::mutex> lg(HwLocks::I2C);
    display.clearDisplay();

    display.setCursor(10, 10);
    display.println(displayed);

    display.display();
}



