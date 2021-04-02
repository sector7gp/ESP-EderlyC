/* v0.2
 * Powersaving using hardware interrupts
 * sends the reset signal to the ESP when its reads a "1" on the sensor pin 
 */
 
#include <avr/sleep.h>
#include <avr/interrupt.h>
//#define POWERCONTROL PB3
#define RESET PB0
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

}

// the loop function runs over and over again forever
void loop() {
  sleep();
  if (digitalRead(PB2)) {
    //pinMode(POWERCONTROL, OUTPUT);
    pinMode(RESET, OUTPUT);

    //digitalWrite(POWERCONTROL, HIGH);
    //delay(100);

    digitalWrite(RESET, HIGH);
    delay(50);
    digitalWrite(RESET, LOW);
    delay(50);
    pinMode(RESET, INPUT);
    //delay(3200);
    //digitalWrite(POWERCONTROL, LOW);


  }
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





