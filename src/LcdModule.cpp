#include "LcdModule.h"
#include "PinConfig.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C _lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
static bool _recordingIndicator = false;

// Losango com buraco no meio — slot 0 do CGRAM
static uint8_t charSDConnected[8] = {
  0x16,
  0x15,
  0x15,
  0x16,
  0x10,
  0x1F,
  0x1F,
  0x00
};

// Escreve o indicador na última coluna da linha superior
static void _WriteIndicator() {
    _lcd.setCursor(LCD_COLS - 1, 0);
    if (_recordingIndicator) {
        _lcd.write((uint8_t)0);   // char personalizado slot 0
    } else {
        _lcd.print(' ');
    }
}

// =============================================================
// LcdModule_Init
// =============================================================
void LcdModule_Init() {
    Wire.begin(PIN_LCD_SDA, PIN_LCD_SCL);
    _lcd.init();
    _lcd.backlight();
    _lcd.createChar(0, charSDConnected);
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("     LCD OK!    ");
}

// =============================================================
// LcdModule_ShowMessage
// line0 é limitada a LCD_COLS-1 chars — a última coluna é reservada ao indicador.
// =============================================================
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

// =============================================================
// LcdModule_SetRecordingIndicator
// =============================================================
void LcdModule_SetRecordingIndicator(bool active) {
    _recordingIndicator = active;
    _WriteIndicator();
}
