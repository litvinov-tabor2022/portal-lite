#ifndef PORTAL_LITE_PORTAL_H
#define PORTAL_LITE_PORTAL_H

#include "Display.h"
#include "PortalFramework.h"

class Portal {
public:
    bool begin(PortalFramework *pFramework);
private:
    Display display;
};


#endif //PORTAL_LITE_PORTAL_H
