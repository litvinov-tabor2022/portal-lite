#ifndef PORTAL_LITE_ITEMSELECTOR_H
#define PORTAL_LITE_ITEMSELECTOR_H

#include "PortalFramework.h"

typedef struct {
    String name;
    i8 bonusPointsDelta;
} SelectedOp;

class ItemSelector {
public:
    void begin();

    void switchNext() {
        curr = (curr + 1) % max;
    }

    void switchPrev() {
        curr--;
        if (curr < 0) curr = max + curr; // yes, it's '+'
    }

    SelectedOp *current() {
        return &ops[curr];
    }

private:
    int curr = 0; // the overflow must be allowed
    static const u8 max = 10;

    SelectedOp ops[max] = {
            SelectedOp{.name="+1", .bonusPointsDelta=1},
            SelectedOp{.name="+2", .bonusPointsDelta=2},
            SelectedOp{.name="+3", .bonusPointsDelta=3},
            SelectedOp{.name="+4", .bonusPointsDelta=4},
            SelectedOp{.name="+5", .bonusPointsDelta=5},
            SelectedOp{.name="+6", .bonusPointsDelta=6},
            SelectedOp{.name="+7", .bonusPointsDelta=7},
            SelectedOp{.name="+8", .bonusPointsDelta=8},
            SelectedOp{.name="+9", .bonusPointsDelta=9},
            SelectedOp{.name="+10", .bonusPointsDelta=10},
    };
};


#endif //PORTAL_LITE_ITEMSELECTOR_H
