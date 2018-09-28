
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
//static int bar=1;
int bar=1;

// Normal
// text "zed" appears in .map, and storage seems to be in TI.persistent segment at 0xC402
#pragma PERSISTENT
int zed=2;




int mainaaa() {
    foo = 1;
    bar = 2;
    zed = 3;
    return 0;
}
