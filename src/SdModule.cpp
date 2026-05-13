/***********************************************************************************************************************
 * @file SdModule.cpp
 * @addtogroup SdModule
 * @author  Gabriel Domingos
 * @brief Implementacao do modulo de logging CSV em cartao SD via HSPI.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "SdModule.h"
#include "PinConfig.h"
#include <SPI.h>
#include <SD.h>

/***********************************************************************************************************************
 * VARIAVEIS LOCAIS
 **********************************************************************************************************************/

/* --- ESP32: HSPI e o segundo barramento SPI do ESP32, com pinos remapeaveis (comentado) ---
static SPIClass  _hspi(HSPI);
*/
static File      _logFile;
static bool      _isLogging   = false;
static uint32_t  _rowCount    = 0;
static uint32_t  _lastFlushMs = 0;

static const uint32_t FLUSH_INTERVAL_MS = 500; ///< Intervalo de flush para o SD (ms)

/***********************************************************************************************************************
 * FUNCOES PUBLICAS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Inicializa o barramento HSPI e monta o cartao SD.
 * @details Deve ser chamada uma vez no setup(), antes de qualquer outra funcao.
 *          Utiliza automaticamente os pinos PIN_SD_* de PinConfig.h.
 * @retval true   Cartao montado com sucesso.
 * @retval false  Cartao nao detectado ou falha na montagem.
 **********************************************************************************************************************/
bool SdModule_Init() {
    /* --- ESP32: HSPI permite configurar pinos SPI arbitrarios; necessario passar a instancia para SD.begin (comentado) ---
    _hspi.begin(PIN_SD_CLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
    if (!SD.begin(PIN_SD_CS, _hspi)) {
    */
    /* --- Arduino Uno: SPI de hardware com pinos fixos (MOSI=11, MISO=12, SCK=13); apenas o CS e configuravel --- */
    if (!SD.begin(PIN_SD_CS)) {
        Serial.println(F("[SD] ERRO: cartao nao detectado ou falha na montagem."));
        return false;
    }

    Serial.println(F("[SD] Cartao montado com sucesso."));
    return true;
}

/***********************************************************************************************************************
 * @brief Abre um novo arquivo CSV no cartao e grava a linha de cabecalho.
 * @details O nome e gerado automaticamente: /log_NNNN.csv (primeiro indice livre).
 * @param csvHeader  String de cabecalho terminada em '\n'. Exemplo: "time_ms,rpm\n"
 * @retval true   Arquivo criado e cabecalho gravado.
 * @retval false  Impossivel criar o arquivo (SD cheio ou limite de 9999 arquivos).
 **********************************************************************************************************************/
bool SdModule_StartLog(const char* csvHeader) {
    if (_isLogging) {
        SdModule_StopLog();
    }

    char filename[16];
    for (uint16_t i = 1; i <= 9999; i++) {
        snprintf(filename, sizeof(filename), "/log_%04u.csv", i);
        if (!SD.exists(filename)) {
            break;
        }
        if (i == 9999) {
            Serial.println(F("[SD] ERRO: limite de arquivos de log atingido (9999)."));
            return false;
        }
    }

    _logFile = SD.open(filename, FILE_WRITE);
    if (!_logFile) {
        /* --- ESP32: Serial.printf existe no core Espressif; nao disponivel no AVR (comentado) ---
        Serial.printf("[SD] ERRO: nao foi possivel criar %s\n", filename);
        */
        Serial.print(F("[SD] ERRO: nao foi possivel criar ")); Serial.println(filename);
        return false;
    }

    _logFile.print(csvHeader);
    _rowCount    = 0;
    _isLogging   = true;
    _lastFlushMs = millis();

    /* --- ESP32 (comentado) --- Serial.printf("[SD] Log iniciado: %s\n", filename); */
    Serial.print(F("[SD] Log iniciado: ")); Serial.println(filename);
    return true;
}

/***********************************************************************************************************************
 * @brief Faz flush e fecha o arquivo de log atual.
 * @details Seguro chamar mesmo sem arquivo aberto (no-op).
 * @retval true   Arquivo fechado corretamente.
 * @retval false  Nenhum arquivo estava aberto.
 **********************************************************************************************************************/
bool SdModule_StopLog() {
    if (!_isLogging) return false;

    _logFile.flush();
    _logFile.close();
    _isLogging = false;

    /* --- ESP32 (comentado) --- Serial.printf("[SD] Log encerrado. Linhas gravadas: %lu\n", (unsigned long)_rowCount); */
    Serial.print(F("[SD] Log encerrado. Linhas gravadas: ")); Serial.println(_rowCount);
    return true;
}

/***********************************************************************************************************************
 * @brief Adiciona uma linha de dados CSV ao arquivo aberto.
 * @details Flush automatico a cada FLUSH_INTERVAL_MS (500 ms).
 * @param line  Linha CSV completa incluindo '\n'. A formatacao e responsabilidade do chamador.
 * @retval true   Linha gravada.
 * @retval false  Nenhum arquivo aberto ou falha na escrita.
 **********************************************************************************************************************/
bool SdModule_WriteLine(const char* line) {
    if (!_isLogging) return false;

    _logFile.print(line);
    _rowCount++;

    uint32_t now = millis();
    if (now - _lastFlushMs >= FLUSH_INTERVAL_MS) {
        _logFile.flush();
        _lastFlushMs = now;
    }

    return true;
}

/***********************************************************************************************************************
 * @brief Retorna true se um arquivo de log estiver aberto e aceitando dados.
 * @retval true   Sessao de log ativa.
 * @retval false  Sessao encerrada ou nao iniciada.
 **********************************************************************************************************************/
bool SdModule_IsLogging() {
    return _isLogging;
}

/***********************************************************************************************************************
 * @brief Retorna o numero de linhas de dados gravadas na sessao atual.
 * @details Exclui a linha de cabecalho.
 * @retval Contagem de linhas desde o ultimo SdModule_StartLog().
 **********************************************************************************************************************/
uint32_t SdModule_GetRowCount() {
    return _rowCount;
}
