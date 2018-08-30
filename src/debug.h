
#pragma once


class Debug {
public:
    /*
     * Leave a trail of breadcrumbs in FRAM
     */
    static void leaveCrumb(unsigned int);

    /*
     * Write assert line number to FRAM
     */
    static void persistLineNumber(unsigned int);
};
