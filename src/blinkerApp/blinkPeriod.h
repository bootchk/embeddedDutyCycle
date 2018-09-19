

/*
 * Knows count of blinks in blink periods.
 *
 * We schedule the blinking task, not and end of period task.
 */


class BlinkPeriod {
    static int counter;
    static bool _isEvening;

public:
    static void initForEveningBlinking() {
        // Evening blink for 4 hours every 10 seconds
        counter = 6 * 60 * 4;
        _isEvening = true;
    }

    static void initForMorningBlinking() {
            // Evening blink for 2 hours every 10 seconds
            counter = 6 * 60 * 2;
            _isEvening = false;
        }

    static bool isOver() { return counter <= 0; }

    static bool isEvening() { return _isEvening; }

    static bool blink() { counter--; }
};
