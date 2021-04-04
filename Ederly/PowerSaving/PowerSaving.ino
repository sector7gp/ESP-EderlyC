/* v0.3
   Powersaving using hardware interrupts
   sends the reset signal to the ESP with the reading in PB2 (PCINT2) sensor pin
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>
#define SIGNAL PB3
#define RESET PB0
#define BOTTON PB2
#define SIGNAL_TIME 3000
#define RESET_TIME 150
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

}

// the loop function runs over and over again forever
void loop() {
  sleep();
  boolean sensor;
  do  {
    sensor = digitalRead(BOTTON);
    //sets the pin and send the reset signal to the ESP
    pinMode(RESET, OUTPUT);
    digitalWrite(RESET, HIGH);
    delay(RESET_TIME);
    digitalWrite(RESET, LOW);
    delay(RESET_TIME);
    pinMode(RESET, INPUT);

    //sets the signal pin
    pinMode(SIGNAL, OUTPUT);

    //send to the ESP the actual sensor value
    digitalWrite(SIGNAL, sensor);
    delay(SIGNAL_TIME);
    pinMode(SIGNAL, INPUT);
  } while (digitalRead(BOTTON) != sensor);

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





