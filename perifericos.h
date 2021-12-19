// lib for inputs and outputs
#include <Ticker.h>

// Relay para encendido de la valvula solenoide para el riego
#define RELAY_PIN D1
#define HORA_EN_SEGUNDOS 3600

Ticker tickerON, tickerOFF;

// 10 min encendido
float riego_segs = HORA_EN_SEGUNDOS / 6;
// cada 1 hora
float espera_segs = HORA_EN_SEGUNDOS;

void turn_off()
{
    // turn off the RELAY
    digitalWrite(RELAY_PIN, LOW);   // turn the RELAY off (LOW is the voltage level)
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on by making the voltage LOW (active low on the ESP-01)
    tickerON.detach();
}

void turn_on()
{
    // turn on the RELAY
    digitalWrite(RELAY_PIN, HIGH);   // turn the RELAY on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off (Note that HIGH is the voltage level: active low)
    // timer to turn off the RELAY
    tickerON.attach(riego_segs, turn_off);
}

void timers_setup()
{
    // timer to turn off the RELAY
    tickerON.attach(riego_segs, turn_off);
    // set timer to turn on the relay
    tickerOFF.attach(espera_segs, turn_on);
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
