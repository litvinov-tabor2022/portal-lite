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
    pinMode(PIN_BUZZER, OUTPUT);

    Core1.loopEvery("buttonsCheck", 20, [this] {
        reactToButtons();
    });

    pFramework->addErrorCallback([this](const String *err) {
        this->displayMsgLong(*err, true);
    });

    pFramework->addOnConnectCallback([this](PlayerData playerData, bool isReload) {
        if (!isReload) {
            Debug.printf("Connected: player ID %d\n", playerData.user_id);

            if (this->pFramework->synchronizationMode.isStarted()) {
                Debug.println("Won't write to tag - I'm in synchronization mode");
                return;
            }

            const u32 now = millis();
            if (now - lastWrite < WRITE_MIN_DELAY) {
                Debug.println("Won't write to tag - it was too fast");
                return;
            }

            lastWrite = now;

            if (!this->updateTag(playerData)) {
                Debug.println("Could not write to tag!!");
                this->displayMsgLong("Chyba zapisu!!!", false);
            } else {
                this->displayMsgShort("OK!", false);
            }
        } else {
            Debug.println("Tag successfully reloaded");
        }
    });

    pFramework->addOnDisconnectCallback([] {
        Debug.println("Tag disconnected");
    });

    beep(BEEP_SHORT);
    return true;
}

bool Portal::updateTag(PlayerData playerData) {
    const i8 delta = itemSelector.current()->bonusPointsDelta;
    Debug.printf("Updating tag - writing delta %d bonus points\n", delta);

    playerData.bonus_points += delta;

    const Transaction t = Transaction{
            .time = pFramework->getCurrentTime(),
            .device_id = pFramework->getDeviceConfig().deviceId,
            .user_id= static_cast<u8>(playerData.user_id),
            .bonus_points = delta,
    };

    if (!(pFramework->writePlayerData(playerData) && pFramework->storage.appendTransaction(t))) {
        Debug.println("Can't write to the tag or commit log!");
        return false;
    }

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

void Portal::displayMsgLong(const String &msg, bool scroll) {
    displayMsg(msg, 2000, scroll);
    beep(BEEP_LONG);
}

void Portal::displayMsgShort(const String &msg, bool scroll) {
    displayMsg(msg, 600, scroll);
    beep(BEEP_SHORT);
}

void Portal::displayMsg(const String &msg, const u16 timeout, bool scroll) {
    display.show(msg, 0, 0, 3, scroll);
    Core0.once("show-value-back", [timeout, this]() {
        Tasker::sleep(timeout);
        this->displayCurrentItem();
    });
}

void Portal::beep(const u16 timeout) {
    digitalWrite(PIN_BUZZER, HIGH);
    Tasker::sleep(timeout);
    digitalWrite(PIN_BUZZER, LOW);
}
