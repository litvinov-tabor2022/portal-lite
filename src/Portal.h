#ifndef PORTAL_LITE_PORTAL_H
#define PORTAL_LITE_PORTAL_H

#include "Display.h"
#include "PortalFramework.h"
#include "ItemSelector.h"

class Portal {
public:
    bool begin(PortalFramework *pFramework);

    void beep(u16 timeout);

private:
    void reactToButtons();

    bool updateTag(PlayerData playerData);

    void buttonLeftAction();

    void buttonRightAction();

    void buttonRightLongAction();

    void displayCurrentItem();

    void displayMsgLong(const String& msg, bool scroll);

    void displayMsgShort(const String& msg, bool scroll);

    void displayMsg(const String &msg, u16 timeout, bool scroll);

    PortalFramework *pFramework;
    Display display;
    ItemSelector itemSelector;

    u32 rightPressedSince = 0, leftPressedSince = 0;
    u32 lastWrite = 0;
    bool leftReacted = false, rightReacted = false;
};


#endif //PORTAL_LITE_PORTAL_H
