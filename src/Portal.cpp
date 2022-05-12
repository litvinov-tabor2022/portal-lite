#include "Portal.h"

bool Portal::begin(PortalFramework *pFramework) {
    if (!display.begin()) {
        Debug.println("Could not initialize display!");
        return false;
    }

    return true;
}
