
#include "myAssert.h"

#include "testMain.h"

void fail(unsigned int line) {
    TestMain::warbleRedLEDForever();
}
