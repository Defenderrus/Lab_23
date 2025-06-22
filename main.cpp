#include <windows.h>
#include "tests/test.hpp"


int main(void) {
    SetConsoleOutputCP(65001);
    run_tests();
    return 0;
}
