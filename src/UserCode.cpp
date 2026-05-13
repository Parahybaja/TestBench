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
#include "HX711.h"
/* USER CODE END INCLUDES */

/***********************************************************************************************************************
 * VARIAVEIS LOCAIS
 **********************************************************************************************************************/

/* USER CODE BEGIN VARIABLES */
HX711 scale;
const float CALIBRATION_FACTOR = 12.0; // Use o fator que você descobriu na calibração
float currentWeight = 0.0;
float currentNewton = 0.0;

// Pinos da célula de carga (certifique-se de que não conflitam com o SD ou LCD no PinConfig.h)
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
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
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare(); // Zera a balança ao ligar
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
    // Leitura contínua para máxima velocidade
    if (scale.is_ready()) {
        currentWeight = scale.get_units(1) / 1000.0; // kg
        currentNewton = currentWeight * 9.80665;     // N
    }
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
    // Atualiza o display a cada 250ms para evitar flickering (piscar)
    if (elapsedMs - lastDisplayUpdate >= 250) {
        lastDisplayUpdate = elapsedMs;
        
        char l0[17], l1[17];
        char wStr[10], nStr[10];
        dtostrf(currentWeight, 6, 3, wStr);
        dtostrf(currentNewton, 6, 1, nStr);
        snprintf(l0, sizeof(l0), "P:%s kg", wStr);
        snprintf(l1, sizeof(l1), "F:%s N", nStr);
        LcdModule_ShowMessage(l0, l1);
    }
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
    return "timestamp_ms,peso_kg,forca_n\n";
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
    static char buf[48];

    /* USER CODE BEGIN DATA_ROW */
    char wStr[12], nStr[12];
    dtostrf(currentWeight, 1, 3, wStr);
    dtostrf(currentNewton, 1, 2, nStr);
    snprintf(buf, sizeof(buf), "%lu,%s,%s\n",
             (unsigned long)timestampMs,
             wStr,
             nStr);
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
    // Reseta valores ao parar a gravação
    currentWeight = 0;
    currentNewton = 0;
    /* USER CODE END STOP */

    lastDisplayUpdate = (uint32_t)(-1000UL); // forca update imediato na proxima sessao
}
