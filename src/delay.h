

/*
 * Delays that may or may not be necessary.
 *
 */
/*
 * Delay might be needed for some unknown reason.
 * When I remove this delay, fails assert RTC::isReadable in alarmConfigure
 * !!! This delay, determined experimentally, seems necessary.
 * Originally it was on every reset.
 * It also seems to work only for coldstart.
 */

void delayForStartup();
