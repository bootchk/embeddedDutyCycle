
#include "baz.h"


// Error?
// storage is allocated in .bss segment (in RAM)
// Compiler gives: warning, must be initialized
#pragma PERSISTENT
// WRONG: int foo;
int foo=0;


// Wierd?
// the text "bar" does not appear in the .map, although storage seems to be allocated in TI.persistent segment anonymously at 0xC400
// static vars do not appear in .map
#pragma PERSISTENT
static int bar=1;
// int bar=1;

// Normal
// text "zed" appears in .map, and storage seems to be in TI.persistent segment at 0xC402
#pragma PERSISTENT
int zed=2;

// text baz not appear in map without linker flag.
// with linker flag, baz appears in map at location 2300, i.e. in RAM
namespace {
#pragma PERSISTENT
static int baz = 0;
}


int mainbbb() {
    Baz::init();
    foo = 1;
    bar = 2;
    zed = 3;
    baz = 1;
    return 0;
}
