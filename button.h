typedef enum {
    EVENT_IDLE,
    EVENT_PRESSED
} event_t;

typedef enum {
    STATE_UNPRESSED,
    STATE_PRESSED
} btnState_t;

typedef struct {
    volatile uint8_t* port;
    uint8_t pin;
    btnState_t state;
    btnState_t lastState;
    event_t event;
} button_t;

void BUTTONS_task();