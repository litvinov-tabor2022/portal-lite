#ifndef PORTAL_LITE_PORTAL_H
#define PORTAL_LITE_PORTAL_H

#include "Display.h"
#include "PortalFramework.h"
#include "ItemSelector.h"

class Portal {
public:
    bool begin(PortalFramework *pFramework);
private:
    Display display;
    ItemSelector itemSelector;
};


#endif //PORTAL_LITE_PORTAL_H
