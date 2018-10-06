
namespace {
#pragma PERSISTENT
static int baz2 = 0;
}

class Baz {
public:
    static void init() {
        baz2 = 1;
    }
};
