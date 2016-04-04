#include <SPI.h>

char buffer [100];
volatile byte pos;
volatile bool process_it;
volatile bool initial;

void setup (void) {

  Serial.begin (115200);   // debugging

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;
  initial = true;
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}  // end of setup

// SPI interrupt routine
ISR (SPI_STC_vect) {

  byte c = SPDR;

  if (pos < sizeof buffer)
  {
    if (c >= 'A' && c <= 'Z') {
      if (initial) {
        if (c == 'A') {
          initial = false;
          buffer [pos++] = c;
          if (pos == 26) {
            process_it = true;
          }
        }
      }
      else {
        buffer [pos++] = c;
        if (pos == 26) {
          process_it = true;
        }
      }
      SPDR = c + 32;
    }
  }

}

void loop (void) {

  if (process_it) {
    buffer [pos] = 0;
    Serial.println (buffer);
    pos = 0;
    process_it = false;
    initial = true;
  }
}
