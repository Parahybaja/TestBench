#pragma once

/***********************************************************************************************************************
 * @file PinConfig.h
 * @brief Mapeamento de pinos e parametros do sistema.
 * @details Edite a secao USER para adicionar pinos de sensores entre testes.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @addtogroup PinConfig_SD SD Card — barramento HSPI
 * @{
 **********************************************************************************************************************/

#define PIN_SD_MOSI   13   ///< D13 — MOSI
#define PIN_SD_MISO   12   ///< D12 — MISO
#define PIN_SD_CLK    14   ///< D14 — SCK
#define PIN_SD_CS     27   ///< D27 — Chip Select

/** @} PinConfig_SD */

/***********************************************************************************************************************
 * @addtogroup PinConfig_Button Botao Start/Stop — INPUT_PULLUP, ativo em LOW
 * @{
 **********************************************************************************************************************/

#define PIN_BUTTON    26   ///< D26

/** @} PinConfig_Button */

/***********************************************************************************************************************
 * @addtogroup PinConfig_LCD LCD I2C
 * @{
 **********************************************************************************************************************/

#define PIN_LCD_SDA   32   ///< D32 — SDA
#define PIN_LCD_SCL   33   ///< D33 — SCL
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
