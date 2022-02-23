

#include <arduino-timer.h>

#define SPEED_POT_PIN A0
#define PULSE_OUT_PIN 13
#define DELAY_FUNCTION delayMicroseconds
//#define DELAY_FUNCTION delay

Timer<1> sample_pot_timer;
uint16_t delayMicros = 0;

void samplePot() {
  int val, speed;
  val = analogRead(SPEED_POT_PIN);
  delayMicros = map(val, 0, 1100, 500, 10); 
  Serial.println(delayMicros);
}


void setup() {
  pinMode(PULSE_OUT_PIN, OUTPUT);
  sample_pot_timer.every(50, samplePot);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  int delayMicros;

  sample_pot_timer.tick();
  digitalWrite(PULSE_OUT_PIN, HIGH);  
  DELAY_FUNCTION(delayMicros);                       
  digitalWrite(PULSE_OUT_PIN, LOW);    
  DELAY_FUNCTION(delayMicros);                      
}
