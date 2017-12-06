// output the test pattern requested over serial
// format looks like "000000000000" to set all off "111111111111" to set all pulsing
// for {PORTB 8->13, PORT C0-5}
// does about 75kHz on Arduino Uno

void setup()
{
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  
  // PORTB -> pins 8->13 (top 2 bits are unusable)
  DDRB = 0b00111111;
  PORTB = 0; // start low
  
  // PORTC -> analogue pins 0-5
  DDRC = 0b00111111;
  PORTC = 0; // start low
}

// reads in desired value for bit _n of port B
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

// reads in desired value for bit _n of port C
//   adjustments to _n because we are run after port B
#define CHANGE_BIT_C(_n)                \
  uint8_t char##_n = Serial.read();     \
  while (255 == char##_n) {             \
    char##_n = Serial.read();           \
  }                                     \  
    switch(char##_n) {                  \
      case '1':                         \
        port_c_high |= 1 << (_n - 6);   \
        break;                          \
      case '0':                         \
        port_c_high &= ~(1 << (_n - 6));\
        break;                          \
      default:                          \
        goto end;                       \
    }

void loop()
{
  // toggle bits for port B
  static uint8_t port_b_high = 0;
  
  if (0 == PORTB) {
    PORTB = port_b_high;
  } else {
    PORTB = 0;
  }
  
  // toggle bits for port C
  static uint8_t port_c_high = 0;
  
  if (0 == PORTC) {
    PORTC = port_c_high;
  } else {
    PORTC = 0;
  }
  
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // change toggle value to match new instructions
    CHANGE_BIT_B(0)
    CHANGE_BIT_B(1)
    CHANGE_BIT_B(2)  
    CHANGE_BIT_B(3)
    CHANGE_BIT_B(4)
    CHANGE_BIT_B(5) 
    CHANGE_BIT_C(6)
    CHANGE_BIT_C(7)
    CHANGE_BIT_C(8)  
    CHANGE_BIT_C(9)
    CHANGE_BIT_C(10)
    CHANGE_BIT_C(11)  
  }
  
  end: ; // keep looping around
}
