
#pragma once

void fail(unsigned int line);


/*
 * Custom assert macros
 */


// general assertion
#define myAssert(expr) \
    if (!(expr)) \
        fail(__LINE__)


// postcondition assertion
#define ensure(expr) \
    if (!(expr)) \
        fail(__LINE__)



// traditionally would be fail(__FILE__, __LINE__)
