// output the test pattern requested over serial

void setup()
{
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  
  // PORTB -> pins 8->18 (top 2 bits are unusable)
  DDRB = 0b00111111;
  PORTB = 0; // start low
}

#define CHANGE_BIT_B(_n)                \
  uint8_t char##_n = Serial.read();     \
  while (255 == char##_n) {             \
    char##_n = Serial.read();           \
  }                                     \  
    switch(char##_n) {                  \
      case '1':                         \
        port_b_high |= 1 << _n;         \
        break;                          \
      case '0':                         \
        port_b_high &= ~(1 << _n);      \
        break;                          \
      default:                          \
        goto end;                       \
    }

void loop()
{
  static uint8_t port_b_high = 0b00111111;
  
  if (0 == PORTB) {
    PORTB = port_b_high;
  } else {
    PORTB = 0;
  }

  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // change bits
    CHANGE_BIT_B(0)
    CHANGE_BIT_B(1)
    CHANGE_BIT_B(2)  
    CHANGE_BIT_B(3)
    CHANGE_BIT_B(4)
    CHANGE_BIT_B(5)  
  }
  
  end: ;
}
