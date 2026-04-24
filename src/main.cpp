#include <Arduino.h>
#include "CoreModule.h"

void setup() {
    CoreModule_Init();
}

void loop() {
    CoreModule_Update();
}
