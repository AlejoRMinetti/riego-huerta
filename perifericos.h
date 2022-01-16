// lib for inputs and outputs
#include <Ticker.h>

// Relay para encendido de la valvula solenoide para el riego
#define RELAY_PIN D1
#define HORA_EN_SEGUNDOS 3600

Ticker ticker_cada_seg;

// 10 min encendido
int riego_segs = HORA_EN_SEGUNDOS / 6;
// cada 2 hora
int espera_segs = HORA_EN_SEGUNDOS * 2;

// remain time for next riego state
int remain_riego_segs = riego_segs;

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

// set riego times
void set_riego_config(int horaEncendido, int minEncendido, int segEncendido,
                        int horaApagado, int minApagado, int segApagado)
{
    riego_segs = (horaEncendido * HORA_EN_SEGUNDOS) + (minEncendido * 60) + segEncendido;
    espera_segs = (horaApagado * HORA_EN_SEGUNDOS) + (minApagado * 60) + segApagado;
}

int get_horas_restantes()
{
    return remain_riego_segs / HORA_EN_SEGUNDOS;
}

int get_minutos_restantes()
{
    return (int(remain_riego_segs) % HORA_EN_SEGUNDOS) / 60;
}

int get_segundos_restantes()
{
    return (int(remain_riego_segs) % HORA_EN_SEGUNDOS) % 60;
}

// get riego times
String riego_config_str()
{
    String riego_config = "Encendido: ";
    riego_config += String(riego_segs / HORA_EN_SEGUNDOS) + ":";
    riego_config += String((int(riego_segs) % HORA_EN_SEGUNDOS) / 60) + ":";
    riego_config += String((int(riego_segs) % HORA_EN_SEGUNDOS) % 60);
    riego_config += " Apagado: ";
    riego_config += String(espera_segs / HORA_EN_SEGUNDOS) + ":";
    riego_config += String((int(espera_segs) % HORA_EN_SEGUNDOS) / 60) + ":";
    riego_config += String((int(espera_segs) % HORA_EN_SEGUNDOS) % 60);
    return riego_config;
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
    turn_on_riego();
    timers_setup();
}
