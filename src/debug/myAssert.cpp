
#include "myAssert.h"

#include "fatal.h"
#include "debug.h"


//void fail(unsigned int line, char* file ) {

void fail(unsigned int line ) {
    Debug::persistLineNumber(line);
    Fatal::warbleRedLEDForever();
}
