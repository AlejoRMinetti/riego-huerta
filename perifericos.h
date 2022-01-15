// lib for inputs and outputs
#include <Ticker.h>

// Relay para encendido de la valvula solenoide para el riego
#define RELAY_PIN D1
#define HORA_EN_SEGUNDOS 3600

Ticker ticker_cada_seg;

// 10 min encendido
float riego_segs = 5;
// cada 1 hora
float espera_segs = 10;

// remain time for next riego state
float remain_riego_segs = riego_segs;

// Riego state
bool riego_state = false;

void turn_off_riego()
{
    riego_state = false;
    remain_riego_segs = espera_segs;
    // turn off the RELAY
    digitalWrite(RELAY_PIN, LOW);   // turn the RELAY off (LOW is the voltage level)
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on by making the voltage LOW (active low on the ESP-01)

}

void turn_on_riego()
{
    riego_state = true;
    remain_riego_segs = riego_segs;
    // turn on the RELAY
    digitalWrite(RELAY_PIN, HIGH);   // turn the RELAY on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off (Note that HIGH is the voltage level: active low)
}

// return remain time for next riego state en minutos
float get_remain_riego_segs()
{
    return remain_riego_segs;
}


void timers_setup()
{
    // timer to save remain time for next riego state
    ticker_cada_seg.attach(1, []() {
        remain_riego_segs--;
        if (remain_riego_segs <= 0)
        {
            if (riego_state)
            {
                turn_off_riego();
            }
            else
            {
                turn_on_riego();
            }
        }
        
    });
    
}

void perifericos_setup()
{
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
    pinMode(RELAY_PIN, OUTPUT);   // Initialize the RELAY_PIN pin as an output
    // RELAY starts on
    digitalWrite(RELAY_PIN, HIGH);   // turn the RELAY on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off (Note that HIGH is the voltage level: active low)
    timers_setup();
}
