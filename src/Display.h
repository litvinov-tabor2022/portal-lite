#ifndef PORTAL_LITE_DISPLAY_H
#define PORTAL_LITE_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

class Display {
public:
    bool begin();

private:
    void draw();

    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
};


#endif //PORTAL_LITE_DISPLAY_H
