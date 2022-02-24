

#include <PinChangeInt.h>
#include <arduino-timer.h>

#define SPEED_POT_PIN A0
#define PULSE_OUT_PIN 13
#define SPINDLE_PWM_PIN 11
#define SPINDLE_ENABLE_PIN 8
#define MANUAL_ENABLE_PIN 9
#define DELAY_FUNCTION delayMicroseconds
//#define DELAY_FUNCTION delay
 

//volatile unsigned long pwm_value = 0;
volatile  unsigned long prev_time = 0;
uint8_t latest_interrupted_pin;
 
void rising()
{
  latest_interrupted_pin=PCintPort::arduinoPin;
//  PCintPort::attachInterrupt(latest_interrupted_pin, &falling, FALLING);
  prev_time = micros();
}
 
//void falling() {
//  latest_interrupted_pin=PCintPort::arduinoPin;
//  PCintPort::attachInterrupt(latest_interrupted_pin, &rising, RISING);
//  pwm_value = micros()-prev_time;
//}



Timer<1> sample_pot_timer;
uint16_t delayMicros = 0;

// Convert the pot setting to a delay for steps
void samplePot() {
  int val, speed;
  val = analogRead(SPEED_POT_PIN);
  delayMicros = map(val, 0, 1100, 500, 10); 
  //Serial.println(delayMicros);
}


void setup() {
  // Set up pin change to measure PWM
  pinMode(SPINDLE_PWM_PIN, INPUT); digitalWrite(SPINDLE_PWM_PIN, HIGH);
  PCintPort::attachInterrupt(SPINDLE_PWM_PIN, &rising, RISING);
  pinMode(PULSE_OUT_PIN, OUTPUT);
  pinMode(SPINDLE_ENABLE_PIN, OUTPUT);
  pinMode(MANUAL_ENABLE_PIN, INPUT_PULLUP);
 
  sample_pot_timer.every(50, samplePot);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  int delayMicros;
  sample_pot_timer.tick();
  //Serial.println(micros() - prev_time);
  //Serial.println(delayMicros);
  // Output pulses if we are getting a PWM stream or the manual switch is on
  if((micros()- prev_time) < 1000 || !digitalRead(MANUAL_ENABLE_PIN)) {
    digitalWrite(SPINDLE_ENABLE_PIN, HIGH); 
    digitalWrite(PULSE_OUT_PIN, HIGH);  
    DELAY_FUNCTION(delayMicros);                       
    digitalWrite(PULSE_OUT_PIN, LOW);    
    DELAY_FUNCTION(delayMicros);  
//    pwm_value = 0;                    
  } else {
    digitalWrite(SPINDLE_ENABLE_PIN, LOW); 
  }
}
