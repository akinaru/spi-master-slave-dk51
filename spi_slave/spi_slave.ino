#include <SPI.h>

uint8_t buffer [100];
volatile byte pos;
volatile bool process_it;
volatile bool initial;
volatile bool error_detected;
volatile byte process_command;

enum State{
  COMMAND,
  DATA
};

uint8_t command = 0;
uint16_t data_count = 0;

State state = COMMAND;

void setup (void) {

  Serial.begin (115200);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;
  process_command = 0;
  initial = true;
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}

bool check_command(byte cmd){
  if (cmd>0 && cmd <=4)
    return true;
  return false;
}

byte set_buffer(uint16_t max_length,byte c){

  if (data_count==0){
    buffer [pos++]=max_length;
  }
  if (data_count < max_length){
    buffer [pos++] = c;
    if (data_count==(max_length-1)){
      process_it = true;
      data_count=0;
      command=0;
      state = COMMAND;
      return 1;
    }
    else{
      data_count++;
    }
  }
  return 0;
}

// SPI interrupt routine
ISR (SPI_STC_vect) {

  byte c = SPDR;
  byte ret = 0;
  
  switch (state) {

    case COMMAND:
      if (check_command(c)) {
        command = c;
        process_command = c;
        state = DATA;
      }
      else if (c!='\0'){
        error_detected=true;
      }
      break;
    case DATA:

      switch (command) {
        case 1:
          ret = set_buffer(2,c);
          break;
        case 2:
          ret = set_buffer(16,c);
          break;
        case 3:
          ret = set_buffer(8,c);
          break;
        case 4:
          ret = set_buffer(4,c);
          break;
      }
      break;
  }
  SPDR=ret;
}

void loop (void) {

  if (process_it) {
    buffer [pos] = 0;
    Serial.print ("receive command : ");
    Serial.print(process_command);
    Serial.print(" | data length : ");
    Serial.print(buffer[0]);
    Serial.print(" | data : ");
    for (uint8_t i = 0;i< buffer[0];i++){
      Serial.print(buffer[1+i]);
      Serial.print(",");
    }
    Serial.println("");
    pos = 0;
    process_it = false;
    initial = true;
  }

  if (error_detected){
    error_detected=false;
    Serial.println("error detected");
  }
}
