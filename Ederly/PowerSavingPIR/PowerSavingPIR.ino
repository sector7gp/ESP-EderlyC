/* v0.3 PIR
   Powersaving using hardware interrupts
   sends the reset signal to the ESP with the reading in PB2 (PCINT2) sensor pin
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>
#define SIGNAL PB3  //outputo to the ESP
#define RESET PB0   //output to the RST pin
#define BOTTON PB2  //input sensor
#define SIGNAL_TIME 5000
#define RESET_TIME 150
#define SCREEN_TIME 180000
void setup() {
}

void loop() {
  sleep();
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

  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on

  sei();                                  // Enable interrupts
} // sleep


ISR(PCINT0_vect) {
  // This is called when the interrupt occurs, but I don't need to do anything in it
}





