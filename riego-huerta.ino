// Riego para la huerta de CHAMA

// custom libs del proyecto
#include "perifericos.h"
#include "wifiapp.h"

void setup() {
  perifericos_setup();
  wifi_setup();
}

// the loop function runs over and over again forever
void loop() {
  wifi_refresh(); // only OTA for now
  // el uso de wifi me forza a no usar funciones 
  // bloqueantes como los delays

  // Se utiliza Ticker.h para el encendido y apagado del solenoide
}