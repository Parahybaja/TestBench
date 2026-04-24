#pragma once
#include <Arduino.h>

/***********************************************************************************************************************
 * @file LcdModule.h
 * @addtogroup LcdModule
 * @brief Interface do modulo de display LCD I2C.
 * @details Wire remapeado para PIN_LCD_SDA / PIN_LCD_SCL (barra superior do ESP32).
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * PROTOTIPOS PUBLICOS
 **********************************************************************************************************************/

void LcdModule_Init();
void LcdModule_ShowMessage(const char* line0, const char* line1);
void LcdModule_SetRecordingIndicator(bool active);
