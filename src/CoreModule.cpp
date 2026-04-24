#include "CoreModule.h"
#include "PinConfig.h"
#include "SdModule.h"
#include "LcdModule.h"

// Funções implementadas em UserCode.cpp
extern void        UserCode_Setup();
extern void        UserCode_Loop();
extern void        UserCode_UpdateDisplay(uint32_t elapsedMs);
extern void        UserCode_Stop();
extern const char* UserCode_GetCsvHeader();              // reativar com SD
extern const char* UserCode_GetDataRow(uint32_t elapsedMs); // reativar com SD

// =============================================================
// Timer ISR — tick de 1 ms via hardware timer
// =============================================================
static hw_timer_t*        _timer   = nullptr;
static volatile uint32_t  _sysTick = 0;

void IRAM_ATTR _Timer_ISR() {
    _sysTick++;
}

static inline uint32_t _GetMs() {
    return _sysTick;
}

// =============================================================
// Estado interno da máquina de estados
// =============================================================
enum State { STATE_IDLE, STATE_LOGGING };

static State    _state      = STATE_IDLE;
static uint32_t _logStartMs = 0;
static uint32_t _lastLogMs  = 0;
static bool     _btnPrev    = false;

// =============================================================
// Debounce — entrada única
// Amostra a cada tick (1 ms); confirma mudança após DEBOUNCE_TICKS
// amostras consecutivas no novo estado.
// =============================================================
static const uint8_t DEBOUNCE_TICKS = 10;   // 10 ms

static bool     _debouncedBtn    = false;
static uint8_t  _debounceCounter = 0;
static uint32_t _lastTick        = 0;

static bool _Debounce_Read(bool rawInput) {
    uint32_t currentTick = _GetMs();

    if (currentTick != _lastTick) {
        _lastTick = currentTick;

        if (rawInput != _debouncedBtn) {
            if (++_debounceCounter >= DEBOUNCE_TICKS) {
                _debouncedBtn    = rawInput;
                _debounceCounter = 0;
            }
        } else {
            _debounceCounter = 0;
        }
    }

    return _debouncedBtn;
}

// =============================================================
// CoreModule_Init
// =============================================================
void CoreModule_Init() {
    Serial.begin(115200);

    // Timer 0: 80 MHz / prescaler 80 = 1 MHz; alarme a cada 1000 ticks = 1 ms
    _timer = timerBegin(0, 80, true);
    timerAttachInterrupt(_timer, &_Timer_ISR, true);
    timerAlarmWrite(_timer, 1000, true);
    timerAlarmEnable(_timer);

    LcdModule_Init();
    vTaskDelay(1500 / portTICK_PERIOD_MS);

    if (!SdModule_Init()) {
        LcdModule_ShowMessage(" SDCard ERROR!  ", "");
    }
    else{
        LcdModule_ShowMessage("   SDCard OK!   ", "");
    }
    vTaskDelay(1500 / portTICK_PERIOD_MS);

    pinMode(PIN_BUTTON, INPUT_PULLUP);

    /* --- USER CODE SETUP ---- */
    UserCode_Setup();
    /* ------------------------ */

    LcdModule_ShowMessage("Pressione B1 p/", "comecar teste");
    Serial.println("[CORE] Sistema pronto.");
}

// =============================================================
// CoreModule_Update
// =============================================================
void CoreModule_Update() {
    // Botão com debounce — ativo em LOW → true = pressionado
    bool btnNow     = _Debounce_Read(digitalRead(PIN_BUTTON) == LOW);
    bool btnPressed = (btnNow && !_btnPrev);
    _btnPrev = btnNow;
    uint32_t elapsed = _GetMs() - _logStartMs;

    switch (_state) {

        // ---------------------------------------------------------
        case STATE_IDLE:
        // ---------------------------------------------------------
            if (btnPressed) {
                bool ok = SdModule_StartLog(UserCode_GetCsvHeader());
                LcdModule_SetRecordingIndicator(ok);
                LcdModule_ShowMessage("", "");
                _logStartMs = _GetMs();
                _state      = STATE_LOGGING;
                Serial.println("[CORE] Gravacao iniciada.");
            }
            break;

        // ---------------------------------------------------------
        case STATE_LOGGING:
        // ---------------------------------------------------------

            if (_GetMs() - _lastLogMs >= USER_LOG_INTERVAL_MS) {
                _lastLogMs = _GetMs();
                if (!SdModule_WriteLine(UserCode_GetDataRow(elapsed))) {
                    LcdModule_SetRecordingIndicator(false);
                    Serial.println("[CORE] Erro ao escrever linha no SD.");
                }

                /* --- USER CODE UPDATE DISPLAY --- */
                UserCode_UpdateDisplay(elapsed);
                /* -------------------------------- */
            }

            /* --- USER CODE LOOP --- */
            UserCode_Loop();
            /* ---------------------- */

            if (btnPressed) {
                SdModule_StopLog();
                LcdModule_SetRecordingIndicator(false);
                _state = STATE_IDLE;
                LcdModule_ShowMessage("Pressione B1 p/", "comecar teste");
                Serial.println("[CORE] Gravacao encerrada.");

                /* --- USER CODE STOP LOGGING --- */
                UserCode_Stop();
                /* ------------------------------ */
            }
            break;
    }
}
