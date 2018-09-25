

/*
 * Knows count of blinks in blink periods.
 *
 * We schedule the blinking task, not an end of period task.
 *
 * Blink period comprises two subperiods, evening and morning, separated by time.
 *
 * Sequence of calls is:
 * initForEveningBlinking,
 * advance, isOver, ..., advance, isOver=>true,
 * isEvening()=>false
 * initForMorningBlinking
 * advance, isOver, ..., advance, isOver=>true,
 */


namespace {

/*
 * Blink period must persist through low power sleep
 */
#pragma PERSISTENT
static int counter = 0;

// State variable, evening and morning blink subperiods
#pragma PERSISTENT
static bool _isEvening = false;

#pragma PERSISTENT
static bool _isActive = false;

}



class BlinkPeriod {


public:
    static void initForEveningBlinking() {
        // Evening blink for 4 hours every 10 seconds
        counter = 6 * 60 * 4;
        _isEvening = true;
        _isActive = true;
    }

    static void initForMorningBlinking() {
            // Evening blink for 2 hours every 10 seconds
            counter = 6 * 60 * 2;
            _isEvening = false;
            _isActive = true;
        }

    static bool isActive() { return _isActive; }

    static bool isOver() {
        bool result;
        result = (counter <= 0);
        // active if not over
        _isActive = not result;
        return result;
    }

    static bool isEvening() { return _isEvening; }

    static void advance() { counter--; }
};
