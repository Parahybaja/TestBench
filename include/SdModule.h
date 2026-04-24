#pragma once
#include <Arduino.h>

/***********************************************************************************************************************
 * @file SdModule.h
 * @addtogroup SdModule
 * @brief Interface do modulo de logging CSV em cartao SD via HSPI.
 * @details Todas as funcoes retornam bool: true = sucesso, false = erro.
 *          Erros sao reportados via Serial (115200 baud).
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * PROTOTIPOS PUBLICOS
 **********************************************************************************************************************/

bool     SdModule_Init();
bool     SdModule_StartLog(const char* csvHeader);
bool     SdModule_StopLog();
bool     SdModule_WriteLine(const char* line);
bool     SdModule_IsLogging();
uint32_t SdModule_GetRowCount();
