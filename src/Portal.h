#ifndef PORTAL_LITE_PORTAL_H
#define PORTAL_LITE_PORTAL_H

#include "Display.h"
#include "PortalFramework.h"
#include "ItemSelector.h"

class Portal {
public:
    bool begin(PortalFramework *pFramework);

private:
    void reactToButtons();

    void buttonLeftAction();

    void buttonRightAction();

    void buttonRightLongAction();

    void displayCurrentItem();

    PortalFramework *pFramework;
    Display display;
    ItemSelector itemSelector;

    u16 rightPressedSince = 0, leftPressedSince = 0;
    bool leftReacted = false, rightReacted = false;
};


#endif //PORTAL_LITE_PORTAL_H
