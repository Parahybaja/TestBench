#pragma once

// =============================================================
// PIN CONFIGURATION 
// =============================================================

// --- SD Card (barramento HSPI) ---
#define PIN_SD_MOSI   13   // D13
#define PIN_SD_MISO   12   // D12
#define PIN_SD_CLK    14   // D14
#define PIN_SD_CS     27   // D27

// --- Botão Start/Stop (INPUT_PULLUP — ativo em LOW) ---
#define PIN_BUTTON    26   // D26

// --- LCD I2C (barramento I2C) ---
#define PIN_LCD_SDA   32   // D32
#define PIN_LCD_SCL   33   // D33
#define LCD_I2C_ADDR  0x27 // Endereço mais comum
#define LCD_COLS      16
#define LCD_ROWS       2

// =============================================================
// USER SENSOR PINS — adicione seus pinos abaixo desta linha
//
// =============================================================

// Exemplo:
// #define PIN_SENSOR_RPM        25   // Pulso digital — encoder/hall
// #define PIN_SENSOR_THROTTLE   34   // Entrada analógica — potenciômetro

// --- Taxa de salvamento no SD Card ---
// Define de quantos em quantos ms uma linha é gravada no arquivo CSV.
#define USER_LOG_INTERVAL_MS  100   // 100 ms → 10 amostras/s
