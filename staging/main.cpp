#include <iostream>

#include "PXApplication.hpp"

int main(int, char**)
{
    PXApplication& app = *PXApplication::instance();
    app.init();
    if (!app.run())
        PXApplication::destroyInstance();
    return 0;
}
