

#include <src/mainObject.h>

#include "testMain.h"

#include "PMM/powerMgtModule.h"
#include "duty.h"
#include "app.h"


void Main::onResetPreamble() {
    TestMain::initGpio();

    PMM::unlockLPM5();
}

void Main::onWakeFromLPM() {
    TestMain::blinkLED();
}

void Main::onColdReset() {
    // require external RTC connected
    Duty::onPowerOnReset();

    // assert Duty is ready for setAlarm

    //App::onPowerOnReset();
    // assert app in initial state
    // assert GPIO in sleeping configuration
}

void Main::configureWakeupSource() {
    TestMain::configureWakeupSource();
}
