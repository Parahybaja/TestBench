#pragma once
#include <Arduino.h>

// =============================================================
// SD MODULE — Logger CSV em cartão SD via HSPI
// Todas as funções retornam bool: true = sucesso, false = erro.
// Erros são reportados via Serial (115200 baud).
// =============================================================

/**
 * Inicializa o barramento HSPI e monta o cartão SD.
 * Deve ser chamada uma vez no setup(), antes de qualquer outra função.
 * Utiliza automaticamente os pinos PIN_SD_* de PinConfig.h.
 * Retorna false se o cartão não for detectado ou a montagem falhar.
 */
bool SdModule_Init();

/**
 * Abre um novo arquivo CSV no cartão e grava a linha de cabeçalho.
 * O nome é gerado automaticamente: /log_NNNN.csv
 * (primeiro índice de 4 dígitos não existente no cartão).
 *
 * @param csvHeader  String de cabeçalho fornecida por UserCode.
 *                   Exemplo: "time_ms,rpm,throttle_pct\n"
 *                   Deve ser null-terminated. O módulo não adiciona nada.
 * Retorna false se o arquivo não puder ser criado.
 */
bool SdModule_StartLog(const char* csvHeader);

/**
 * Faz flush e fecha o arquivo de log atual.
 * Seguro chamar mesmo sem arquivo aberto (no-op).
 * Retorna true se havia arquivo aberto e foi fechado corretamente.
 */
bool SdModule_StopLog();

/**
 * Adiciona uma linha de dados CSV ao arquivo aberto.
 * @param line  Linha CSV completa incluindo '\n' no final.
 *              Exemplo: "1024,3500,87\n"
 *              A formatação da string é responsabilidade do chamador (use snprintf).
 *
 * Internamente, o flush é feito a cada FLUSH_INTERVAL_MS (500 ms)
 * para equilibrar desgaste do cartão e segurança dos dados.
 * Retorna false se nenhum arquivo estiver aberto ou se a escrita falhar.
 */
bool SdModule_WriteLine(const char* line);

/**
 * Retorna true se um arquivo de log estiver aberto e aceitando dados.
 */
bool SdModule_IsLogging();

/**
 * Retorna o número de linhas de dados gravadas na sessão atual
 * (exclui a linha de cabeçalho).
 */
uint32_t SdModule_GetRowCount();
