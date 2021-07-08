/* v0.32
   Powersaving using hardware interrupts
   sends the reset signal to the ESP with the reading in PB2 (PCINT2) sensor pin
   implements config mode in pins PB3 & PB4
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>
#define S1  PB3
#define S2  PB4
#define SIGNAL PB1  //outputo to the ESP
#define RESET PB0   //output to the RST pin
#define BOTTON PB2  //input sensor
#define SIGNAL_TIME 5000
#define RESET_TIME 150
#define SCREEN_TIME 180000
boolean s1, s2 = false;
byte mode = 0;

void setup() {
  ADCSRA &= ~_BV(ADEN);
  pinMode(BOTTON, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(RESET, OUTPUT);
  s1 = digitalRead(S1);
  s2 = digitalRead(S2);

  //MODE:1
  if (!s2 && !s1) {
    for (int i = 0; i < 1; i++) {
      digitalWrite(RESET, LOW);
      delay(200);
      digitalWrite(RESET, HIGH);
      delay(200);
    }
    mode = 1;
  }

  //MODE:2
  if (!s2 && s1) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(RESET, LOW);
      delay(200);
      digitalWrite(RESET, HIGH);
      delay(200);
    }
    mode = 2;
  }

  //MODE:3
  if (s2 && !s1) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(RESET, LOW);
      delay(200);
      digitalWrite(RESET, HIGH);
      delay(200);
    }
    mode = 3;
  }

  //MODE:4
  if (s2 && s1) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(RESET, LOW);
      delay(200);
      digitalWrite(RESET, HIGH);
      delay(200);
    }
    mode = 4;
  }
  pinMode(RESET, INPUT);

}

void loop() {
  sleep();

  switch (mode) {
    case 1:  boolean sensor;
      do  {
        sensor = digitalRead(BOTTON);
        //sets the pin and send the reset signal to the ESP
        pinMode(RESET, OUTPUT);
        digitalWrite(RESET, HIGH);
        delay(RESET_TIME);
        digitalWrite(RESET, LOW);
        delay(RESET_TIME);
        pinMode(RESET, INPUT);
        delay(RESET_TIME * 2);

        //sets the signal pin
        pinMode(SIGNAL, OUTPUT);

        //send to the ESP the actual sensor value
        digitalWrite(SIGNAL, sensor);
        delay(SIGNAL_TIME);
        pinMode(SIGNAL, INPUT);
      } while (digitalRead(BOTTON) != sensor);
      break;
    case 2:
      do {
        if (digitalRead(BOTTON) == 1) {
          rst();

          //send to the ESP the actual sensor value
          pinMode(SIGNAL, OUTPUT);
          digitalWrite(SIGNAL, HIGH);
          delay(SIGNAL_TIME);
          pinMode(SIGNAL, INPUT);

          while (digitalRead(BOTTON) == 1) {
          }

          rst();
          //send to the ESP the actual sensor value
          pinMode(SIGNAL, OUTPUT);
          digitalWrite(SIGNAL, LOW);
          delay(SIGNAL_TIME);
          pinMode(SIGNAL, INPUT);
          delay(SCREEN_TIME);
        }
      } while (digitalRead(BOTTON) != 0);
      break;
    case 3:
      break;
    case 4:
      break;
  }


}



void rst() {
  //sets the pin and send the reset signal to the ESP
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  delay(RESET_TIME);
  digitalWrite(RESET, LOW);
  delay(RESET_TIME);
  pinMode(RESET, INPUT);
  delay(RESET_TIME * 2);
}

void sleep() {

  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin

  //ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
  sleep_disable();                        // Clear SE bit
  //ADCSRA |= _BV(ADEN);                    // ADC on

  sei();                                  // Enable interrupts
} // sleep


ISR(PCINT0_vect) {
  // This is called when the interrupt occurs, but I don't need to do anything in it
}
