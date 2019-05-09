// msp430Drivers
#include <alarmClock/time/timeTypes.h>  // Duration


/*
 * Function or static method that is the task.
 */
typedef void (*TaskMethodPtr) (void);


typedef Duration (*MomentMethodPtr) (void);

#ifdef OLD
///typedef void TaskMethod(void);

// Pointer to it
///typedef TaskMethod* TaskMethodPtr;
#endif




#ifdef OLD
/*
 * All implementation is here.
 * Initialized by member initialization list.
 */
class Task {
public:
    TaskMethod* method;

    void execute() { (*method) (); }

    Task(TaskMethod* aMethod) : method(aMethod) {}
};
#endif
