#include "SdModule.h"
#include "PinConfig.h"
#include <SPI.h>
#include <SD.h>

// =============================================================
// Internals
// =============================================================
static SPIClass  _hspi(HSPI);
static File      _logFile;
static bool      _isLogging     = false;
static uint32_t  _rowCount      = 0;
static uint32_t  _lastFlushMs   = 0;

static const uint32_t FLUSH_INTERVAL_MS = 500;

// =============================================================
// SdModule_Init
// =============================================================
bool SdModule_Init() {
    _hspi.begin(PIN_SD_CLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);

    if (!SD.begin(PIN_SD_CS, _hspi)) {
        Serial.println("[SD] ERRO: cartao nao detectado ou falha na montagem.");
        return false;
    }

    Serial.println("[SD] Cartao montado com sucesso.");
    return true;
}

// =============================================================
// SdModule_StartLog
// =============================================================
bool SdModule_StartLog(const char* csvHeader) {
    if (_isLogging) {
        SdModule_StopLog();
    }

    // Encontra o primeiro nome de arquivo disponivel
    char filename[16];
    for (uint16_t i = 1; i <= 9999; i++) {
        snprintf(filename, sizeof(filename), "/log_%04u.csv", i);
        if (!SD.exists(filename)) {
            break;
        }
        if (i == 9999) {
            Serial.println("[SD] ERRO: limite de arquivos de log atingido (9999).");
            return false;
        }
    }

    _logFile = SD.open(filename, FILE_WRITE);
    if (!_logFile) {
        Serial.printf("[SD] ERRO: nao foi possivel criar %s\n", filename);
        return false;
    }

    _logFile.print(csvHeader);
    _rowCount   = 0;
    _isLogging  = true;
    _lastFlushMs = millis();

    Serial.printf("[SD] Log iniciado: %s\n", filename);
    return true;
}

// =============================================================
// SdModule_StopLog
// =============================================================
bool SdModule_StopLog() {
    if (!_isLogging) return false;

    _logFile.flush();
    _logFile.close();
    _isLogging = false;

    Serial.printf("[SD] Log encerrado. Linhas gravadas: %lu\n", (unsigned long)_rowCount);
    return true;
}

// =============================================================
// SdModule_WriteLine
// =============================================================
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

// =============================================================
// SdModule_IsLogging / SdModule_GetRowCount
// =============================================================
bool SdModule_IsLogging() {
    return _isLogging;
}

uint32_t SdModule_GetRowCount() {
    return _rowCount;
}
