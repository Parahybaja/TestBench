/***********************************************************************************************************************
 * @file LcdModule.cpp
 * @addtogroup LcdModule
 * @brief Implementacao do modulo de display LCD I2C.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "LcdModule.h"
#include "PinConfig.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/***********************************************************************************************************************
 * VARIAVEIS LOCAIS
 **********************************************************************************************************************/

static LiquidCrystal_I2C _lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
static bool _recordingIndicator = false;

/// Losango com buraco — caracter personalizado CGRAM slot 0
static uint8_t charSDConnected[8] = {
    0x1C,
    0x12,
    0x11,
    0x11,
    0x11,
    0x1F,
    0x1F,
    0x00
};

/***********************************************************************************************************************
 * FUNCOES PRIVADAS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Escreve ou apaga o indicador de gravacao na ultima coluna da linha superior.
 * @retval Nenhum.
 **********************************************************************************************************************/
static void _WriteIndicator() {
    _lcd.setCursor(LCD_COLS - 1, 0);
    if (_recordingIndicator) {
        _lcd.write((uint8_t)0);
    } else {
        _lcd.print(' ');
    }
}

/***********************************************************************************************************************
 * FUNCOES PUBLICAS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Inicializa o barramento I2C e o LCD.
 * @details Utiliza PIN_LCD_SDA, PIN_LCD_SCL, LCD_I2C_ADDR, LCD_COLS e LCD_ROWS de PinConfig.h.
 * @retval Nenhum.
 **********************************************************************************************************************/
void LcdModule_Init() {
    /* --- ESP32: Wire.begin(sda, scl) permite remapear os pinos I2C em qualquer GPIO (comentado) ---
    Wire.begin(PIN_LCD_SDA, PIN_LCD_SCL);
    */
    Wire.begin(); // Arduino Uno: pinos SDA=A4 e SCL=A5 sao fixos em hardware; sem argumentos
    _lcd.init();
    _lcd.backlight();
    _lcd.createChar(0, charSDConnected);
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("     LCD OK!    ");
}

/***********************************************************************************************************************
 * @brief Exibe uma mensagem de duas linhas imediatamente.
 * @details A linha 0 e limitada a LCD_COLS-1 caracteres — a ultima coluna e reservada ao indicador.
 * @param line0  Linha superior (ate LCD_COLS-1 caracteres).
 * @param line1  Linha inferior (ate LCD_COLS caracteres).
 * @retval Nenhum.
 **********************************************************************************************************************/
void LcdModule_ShowMessage(const char* line0, const char* line1) {
    char buf[LCD_COLS + 1];

    _lcd.clear();

    _lcd.setCursor(0, 0);
    snprintf(buf, sizeof(buf), "%-*.*s", LCD_COLS - 1, LCD_COLS - 1, line0);
    _lcd.print(buf);

    _lcd.setCursor(0, 1);
    snprintf(buf, sizeof(buf), "%-*s", LCD_COLS, line1);
    _lcd.print(buf);

    _WriteIndicator();
}

/***********************************************************************************************************************
 * @brief Exibe ou apaga o indicador de gravacao na ultima coluna da linha superior.
 * @param active  true = losango visivel (gravando OK), false = apagado (idle ou erro SD).
 * @retval Nenhum.
 **********************************************************************************************************************/
void LcdModule_SetRecordingIndicator(bool active) {
    _recordingIndicator = active;
    _WriteIndicator();
}
