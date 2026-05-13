/***********************************************************************************************************************
 * @file UserCode.cpp
 * @addtogroup UserCode
 * @brief Codigo de usuario — unico arquivo a ser editado entre testes.
 * @author  Gabriel Domingos
 * @details Implementa os hooks chamados pelo CoreModule durante a sessao de gravacao.
 *          Edite apenas as regioes delimitadas por USER CODE BEGIN / END.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include <Arduino.h>
#include "PinConfig.h"
#include "LcdModule.h"

/* USER CODE BEGIN INCLUDES */

/* USER CODE END INCLUDES */

/***********************************************************************************************************************
 * VARIAVEIS LOCAIS
 **********************************************************************************************************************/

/* USER CODE BEGIN VARIABLES */

/* USER CODE END VARIABLES */

static uint32_t lastDisplayUpdate = (uint32_t)(-1000UL); // forca update imediato na 1a chamada

/***********************************************************************************************************************
 * FUNCOES PUBLICAS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Chamado uma vez no inicio, apos a inicializacao dos modulos core.
 * @details Inicialize sensores, configure pinos e objetos de biblioteca aqui.
 * @code
 *     pinMode(PIN_SENSOR_RPM, INPUT);
 *     bmp.begin(0x76);
 * @endcode
 * @retval Nenhum.
 **********************************************************************************************************************/
void UserCode_Setup() {
    /* USER CODE BEGIN SETUP */

    /* USER CODE END SETUP */
}

/***********************************************************************************************************************
 * @brief Chamado a cada iteracao do loop durante a gravacao.
 * @details Use para leitura continua de sensores. Evite bloqueios — impactam
 *          a responsividade do botao e a frequencia de amostragem.
 * @code
 *     rpm         = readRpm(PIN_SENSOR_RPM);
 *     temperature = bmp.readTemperature();
 * @endcode
 * @retval Nenhum.
 **********************************************************************************************************************/
void UserCode_Loop() {
    /* USER CODE BEGIN LOOP */

    /* USER CODE END LOOP */
}

/***********************************************************************************************************************
 * @brief Chamado a cada iteracao do loop durante a gravacao para atualizar o display.
 * @details Controle total do LCD — use LcdModule_ShowMessage(). Use elapsedMs para
 *          throttle e evitar flickering.
 * @code
 *     if (elapsedMs - lastDisplayUpdate >= 1000) {
 *         lastDisplayUpdate = elapsedMs;
 *         char l0[16], l1[16];
 *         snprintf(l0, sizeof(l0), "RPM: %d", rpm);
 *         snprintf(l1, sizeof(l1), "T: %.1fC", temperature);
 *         LcdModule_ShowMessage(l0, l1);
 *     }
 * @endcode
 * @param elapsedMs  Tempo em ms desde o inicio da gravacao atual.
 * @retval Nenhum.
 **********************************************************************************************************************/
void UserCode_UpdateDisplay(uint32_t elapsedMs) {
    /* USER CODE BEGIN UPDATE_DISPLAY */

    /* USER CODE END UPDATE_DISPLAY */
}

/***********************************************************************************************************************
 * @brief Retorna o cabecalho CSV. Chamado uma vez ao iniciar a gravacao.
 * @details Deve ser compativel com as colunas retornadas por UserCode_GetDataRow().
 * @code
 *     return "time_ms,rpm,temperature\n";
 * @endcode
 * @retval Ponteiro para string de cabecalho terminada em '\n'.
 **********************************************************************************************************************/
const char* UserCode_GetCsvHeader() {
    /* USER CODE BEGIN CSV_HEADER */
    return "\n";
    /* USER CODE END CSV_HEADER */
}

/***********************************************************************************************************************
 * @brief Retorna uma linha CSV por iteracao durante a gravacao.
 * @details Chamado a cada USER_LOG_INTERVAL_MS. Use snprintf em buf[] —
 *          nao aloque memoria dinamica aqui. Aumente buf[] se precisar de mais colunas.
 * @code
 *     snprintf(buf, sizeof(buf), "%lu,%d,%.2f\n",
 *         (unsigned long)timestampMs,
 *         rpm,
 *         temperature
 *     );
 * @endcode
 * @param timestampMs  Tempo em ms desde o inicio da gravacao.
 * @retval Ponteiro para buffer estatico com a linha CSV.
 **********************************************************************************************************************/
const char* UserCode_GetDataRow(uint32_t timestampMs) {
    static char buf[128];

    /* USER CODE BEGIN DATA_ROW */

    /* USER CODE END DATA_ROW */

    return buf;
}

/***********************************************************************************************************************
 * @brief Chamado uma vez quando a gravacao e encerrada (borda de descida do botao).
 * @details Resete variaveis para o estado inicial para que a proxima sessao comece limpa.
 *          Mantenha sempre o reset de lastDisplayUpdate.
 * @code
 *     rpm                = 0;
 *     temperature        = 0.0f;
 *     lastDisplayUpdate  = (uint32_t)(-1000UL);
 * @endcode
 * @retval Nenhum.
 **********************************************************************************************************************/
void UserCode_Stop() {
    /* USER CODE BEGIN STOP */

    /* USER CODE END STOP */

    lastDisplayUpdate = (uint32_t)(-1000UL); // forca update imediato na proxima sessao
}
