#pragma once
#include <Arduino.h>

// =============================================================
// LCD MODULE
// Wire remapeado para PIN_LCD_SDA / PIN_LCD_SCL (barra superior).
// =============================================================

/**
 * Inicializa o barramento I2C e o LCD.
 * Utiliza PIN_LCD_SDA, PIN_LCD_SCL, LCD_I2C_ADDR, LCD_COLS e LCD_ROWS de PinConfig.h.
 */
void LcdModule_Init();

/**
 * Exibe uma mensagem de duas linhas imediatamente.
 * @param line0  Linha superior (até LCD_COLS caracteres).
 * @param line1  Linha inferior (até LCD_COLS caracteres).
 */
void LcdModule_ShowMessage(const char* line0, const char* line1);

/**
 * Exibe (ou apaga) o indicador de gravação na última coluna da linha superior.
 * @param active  true = losango visível (gravando OK), false = apagado (idle ou erro SD).
 */
void LcdModule_SetRecordingIndicator(bool active);
