#pragma once

/***********************************************************************************************************************
 * @file CoreModule.h
 * @addtogroup CoreModule
 * @brief Interface do modulo principal — maquina de estados e ciclo de atualizacao.
 * @details Responsavel pela inicializacao dos sistemas e pela maquina de estados (IDLE <-> LOGGING).
 *          Nao editar entre testes.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * PROTOTIPOS PUBLICOS
 **********************************************************************************************************************/

void CoreModule_Init();
void CoreModule_Update();
