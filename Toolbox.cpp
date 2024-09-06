#include "Toolbox.h"

Toolbox::Toolbox() {
    debugMode = false;
}

Toolbox &Toolbox::getInstance() {
    static Toolbox instance;
    return instance;
}
