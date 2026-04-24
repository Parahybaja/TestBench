/***********************************************************************************************************************
 * @file main.cpp
 * @brief Ponto de entrada do firmware — delega toda a logica ao CoreModule.
 **********************************************************************************************************************/

#include <Arduino.h>
#include "CoreModule.h"

void setup() {
    CoreModule_Init();
}

void loop() {
    CoreModule_Update();
}
