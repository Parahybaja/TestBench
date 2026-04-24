#include <Arduino.h>
#include "PinConfig.h"
#include "LcdModule.h"

/* USER CODE BEGIN INCLUDES */
/* USER CODE END INCLUDES */

/* USER CODE BEGIN VARIABLES */
static int      contador        = 0;
static int      contador1       = 0;
static uint32_t lastDisplayUpdate = (uint32_t)(-1000UL); // força update imediato na 1ª chamada
/* USER CODE END VARIABLES */

// =============================================================
// UserCode_Setup
// Chamado uma vez no inicio, apos a inicializacao dos modulos core.
// =============================================================
void UserCode_Setup() {
    /* USER CODE BEGIN SETUP */

    /* USER CODE END SETUP */
}

// =============================================================
// UserCode_UpdateDisplay
// Chamado a cada iteracao do loop durante a gravacao.
// Controle total do display — use LcdModule_ShowMessage().
// @param elapsedMs  Tempo em ms desde o inicio da gravacao atual.
// =============================================================
void UserCode_UpdateDisplay(uint32_t elapsedMs) {
    /* USER CODE BEGIN UPDATE_DISPLAY */

    if (elapsedMs - lastDisplayUpdate >= 1000) {
        lastDisplayUpdate = elapsedMs;
        contador++;
        contador1 += 3;
        char linha[17];
        snprintf(linha, sizeof(linha), "C1: %d  C2: %d", contador, contador1);
        LcdModule_ShowMessage(linha, "T: 24.5C  U: 60%");
    }

    /* USER CODE END UPDATE_DISPLAY */
}


// =============================================================
// UserCode_GetCsvHeader
// Retorna o cabecalho CSV. Reativar chamada no CoreModule com SD.
// DEVE ser compativel com as colunas de UserCode_GetDataRow().
// =============================================================
const char* UserCode_GetCsvHeader() {
    /* USER CODE BEGIN CSV_HEADER */
    return "time_ms,contador,contador1\n";
    /* USER CODE END CSV_HEADER */
}


// =============================================================
// UserCode_GetDataRow
// Retorna uma linha CSV por iteracao durante a gravacao.
// Use snprintf em buf[] — nao aloque memoria dinamica aqui.
// Aumente buf[] se precisar de mais colunas.
// =============================================================
const char* UserCode_GetDataRow(uint32_t timestampMs) {
    static char buf[128];

    /* USER CODE BEGIN DATA_ROW */
    snprintf(buf, sizeof(buf), "%lu,%d,%d\n",
        (unsigned long)timestampMs,
        contador,
        contador1
    );
    /* USER CODE END DATA_ROW */

    return buf;
}

// =============================================================
// UserCode_Stop
// Chamado uma vez quando a gravacao eh encerrada (borda de descida do botao).
// =============================================================
void UserCode_Stop() {
    /* USER CODE BEGIN STOP */
    contador          = 0;
    contador1         = 0;
    lastDisplayUpdate = (uint32_t)(-1000UL); // força update imediato na próxima sessão
    /* USER CODE END STOP */
}
