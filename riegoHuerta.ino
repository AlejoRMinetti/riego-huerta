// Riego para la huerta de CHAMA

// Relay para encendido de la valvula solenoide para el riego
#define RELAY_PIN D1

// 1 hora en milisegundos
#define HORA_EN_MILISEGUNDOS 3600000

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(RELAY_PIN, OUTPUT);     // Initialize the RELAY_PIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RELAY_PIN, HIGH);   // turn the RELAY on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off (Note that HIGH is the voltage level: active low)
  delay(HORA_EN_MILISEGUNDOS);                      // tiempo encendido del riego
  digitalWrite(RELAY_PIN, LOW);   // turn the RELAY off (LOW is the voltage level)
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on by making the voltage LOW (active low on the ESP-01)
  delay(HORA_EN_MILISEGUNDOS * 11);   // Wait for next Riego para la huerta
}