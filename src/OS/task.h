
// function or method that is the task
///typedef void TaskMethod(void);

// Pointer to it
///typedef TaskMethod* TaskMethodPtr;

// Alternative:
typedef void (*TaskMethodPtr) (void);



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
