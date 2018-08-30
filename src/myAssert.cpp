
#include "myAssert.h"

#include "testMain.h"
#include "debug.h"



void fail(unsigned int line) {
    Debug::persistLineNumber(line);
    TestMain::warbleRedLEDForever();
}
