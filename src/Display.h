#ifndef PORTAL_LITE_DISPLAY_H
#define PORTAL_LITE_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <utility>
#include "types.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

class Display {
public:
    bool begin();

    void show(String text, u8 x, u8 y, u8 size, bool textScroll = false) {
        displayed = std::move(text);
        displayedSize = size;
        textX = x;
        textY = y;
        this->textScroll = textScroll;

        redraw = true;
    }

private:
    void draw();

    bool redraw = false;
    String displayed = String("OK");
    u8 displayedSize = 1;
    u8 textX = 0, textY = 0;
    bool textScroll = false;

    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
};


#endif //PORTAL_LITE_DISPLAY_H
