#include "Portal.h"
#include "Constants.h"

bool Portal::begin(PortalFramework *pFramework) {
    this->pFramework = pFramework;

    if (!display.begin()) {
        Debug.println("Could not initialize display!");
        return false;
    }

    displayCurrentItem();

    pinMode(PIN_BUTTON_LEFT, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_RIGHT, INPUT_PULLDOWN);

    Core1.loopEvery("buttonsCheck", 20, [this] {
        reactToButtons();
    });

    return true;
}

void Portal::buttonLeftAction() {
    if (pFramework->synchronizationMode.isStarted()) return; // ignore when sync. mode active

    itemSelector.switchPrev();
    displayCurrentItem();
}

void Portal::buttonRightAction() {
    if (pFramework->synchronizationMode.isStarted()) return; // ignore when sync. mode active

    itemSelector.switchNext();
    displayCurrentItem();
}

void Portal::buttonRightLongAction() {
    display.show("Cekej!", 20, 10, 3);

    if (!pFramework->synchronizationMode.isStarted()) {
        Debug.println("Starting synchronization mode...");
        if (!pFramework->synchronizationMode.start()) {
            Debug.println("Synchronization mode couldn't be started!");
            return;
        }
        display.show("Serv. mod", 0, 10, 2, true);
    } else {
        Debug.println("Stopping synchronization mode...");
        if (!pFramework->synchronizationMode.stop()) {
            Debug.println("Synchronization mode couldn't be stopped!");
            return;
        }
        displayCurrentItem();
    }
}

void Portal::reactToButtons() {
    const bool buttonIsPressedLeft = (digitalRead(PIN_BUTTON_LEFT) == HIGH);
    const bool buttonIsPressedRight = (digitalRead(PIN_BUTTON_RIGHT) == HIGH);

    const u32 now = millis();

    u16 leftPressedFor = 0;
    u16 rightPressedFor = 0;

    if (buttonIsPressedLeft) {
        leftPressedSince = leftPressedSince > 0 ? leftPressedSince : now;
        const u16 pressedFor = leftPressedSince > 0 ? now - leftPressedSince : 0;
        if (pressedFor >= BUTTON_THRESHOLD_LONG) {
            leftPressedFor = pressedFor;
        }
    } else {
        leftPressedFor = leftPressedSince > 0 && !leftReacted ? now - leftPressedSince : 0;
        leftPressedSince = 0;
        leftReacted = false;
    }

    if (buttonIsPressedRight) {
        rightPressedSince = rightPressedSince > 0 ? rightPressedSince : now;
        const u16 pressedFor = rightPressedSince > 0 ? now - rightPressedSince : 0;
        if (pressedFor >= BUTTON_THRESHOLD_LONG) {
            rightPressedFor = pressedFor;
        }
    } else {
        rightPressedFor = rightPressedSince > 0 && !rightReacted ? now - rightPressedSince : 0;
        rightPressedSince = 0;
        rightReacted = false;
    }

//    Serial.printf("LeftPressedFor = %d    RightPressedFor = %d\n", leftPressedFor, rightPressedFor);

    if (leftPressedFor > 0 && !leftReacted) {
        leftReacted = true;
        buttonLeftAction();
        return;
    }

    if (rightPressedFor > BUTTON_THRESHOLD_LONG && !rightReacted) {
        rightReacted = true;
        buttonRightLongAction();
        return;
    }

    if (rightPressedFor > 0 && !rightReacted) {
        rightReacted = true;
        buttonRightAction();
        return;
    }
}

void Portal::displayCurrentItem() {
    display.show(itemSelector.current()->name, 45, 3, 4);
}
