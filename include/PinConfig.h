#pragma once

/***********************************************************************************************************************
 * @file PinConfig.h
 * @brief Mapeamento de pinos e parametros do sistema.
 * @details Edite a secao USER para adicionar pinos de sensores entre testes.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @addtogroup PinConfig_SD SD Card — barramento SPI
 * @{
 **********************************************************************************************************************/

/* --- ESP32: barramento HSPI com pinos remapeados (comentado) ---
#define PIN_SD_MOSI   13   ///< D13 — MOSI (HSPI)
#define PIN_SD_MISO   12   ///< D12 — MISO (HSPI)
#define PIN_SD_CLK    14   ///< D14 — SCK  (HSPI)
#define PIN_SD_CS     27   ///< D27 — Chip Select
*/

/* --- Arduino Uno: SPI de hardware fixo (MOSI=11, MISO=12, SCK=13) --- */
#define PIN_SD_CS     10   ///< D10 — Chip Select (unico pino configuravel no Uno)

/** @} PinConfig_SD */

/***********************************************************************************************************************
 * @addtogroup PinConfig_Button Botao Start/Stop — INPUT_PULLUP, ativo em LOW
 * @{
 **********************************************************************************************************************/

/* --- ESP32 (comentado) ---
#define PIN_BUTTON    26   ///< D26
*/

/* --- Arduino Uno --- */
#define PIN_BUTTON      5   ///< D5 — entrada do botao
#define PIN_BUTTON_GND  3   ///< D3 — OUTPUT LOW, serve como GND virtual para o botao

/** @} PinConfig_Button */

/***********************************************************************************************************************
 * @addtogroup PinConfig_LCD LCD I2C
 * @{
 **********************************************************************************************************************/

/* --- ESP32: pinos I2C remapeaveis (comentado) ---
#define PIN_LCD_SDA   32   ///< D32 — SDA
#define PIN_LCD_SCL   33   ///< D33 — SCL
*/

/* --- Arduino Uno: SDA=A4, SCL=A5 (fixos em hardware; Wire.begin() sem argumentos) --- */
#define PIN_LCD_SDA   A4   ///< A4 — SDA (fixo)
#define PIN_LCD_SCL   A5   ///< A5 — SCL (fixo)
#define LCD_I2C_ADDR  0x27 ///< Endereco I2C (alternativa: 0x3F)
#define LCD_COLS      16   ///< Colunas do display
#define LCD_ROWS       2   ///< Linhas do display

/** @} PinConfig_LCD */

/***********************************************************************************************************************
 * @addtogroup PinConfig_User Pinos de Sensores — USER SECTION
 * @brief Adicione seus pinos abaixo desta linha.
 * @{
 **********************************************************************************************************************/

/* USER CODE BEGIN SENSOR_PINS */

/* USER CODE END SENSOR_PINS */

#define USER_LOG_INTERVAL_MS  100   ///< Intervalo de gravacao no SD (ms). 100 ms = 10 amostras/s

/** @} PinConfig_User */
