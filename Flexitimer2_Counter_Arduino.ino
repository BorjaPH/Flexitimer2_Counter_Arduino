#include <FlexiTimer2.h>


const uint8_t tabla [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x07F,0x67,0xE7,0xFF,0x87,0xFD,0xED,0xE6,0xCF,0xDB,0x86,0xBF};
volatile uint8_t censeg, decseg, seg, seg2, turn, flexitimer_interrupts;

// parámetros del flexitimer2
const double resolution = 0.0010;
const uint8_t DELAY_MSECS_Flexitimer = 1;
const uint8_t FLEXITIMER_COUNTS = 10;

// método para la visualización de los SSD
void showDisplay(int timecount) {
  uint8_t d, b;
  d = tabla[timecount] & 0x3F;
  PORTD = (PORTD & 0x03) | (d << 2);
  b = tabla [timecount] & 0xC0;
  PORTB = (PORTB & 0xFC) | (b >> 6);
}

void blink2() 
{
  //callback function of timer2/flexitimer2
  PINC^=(1<<0); //PWM signal  
  flexitimer_interrupts++;
  switch (turn){
      case 0:
        PORTB = (PORTB & 0x03) | 0b00111000;
        showDisplay(censeg);
        turn++;
        break;
      case 1:
        PORTB = (PORTB & 0x03) | 0b00110100;
        showDisplay(decseg);
        turn++;
        break;
      case 2:      
        PORTB = (PORTB & 0x03) | 0b00101100;
        showDisplay(seg);
        turn++;
        break;
      case 3:
        PORTB = (PORTB & 0x03) | 0b00011100;
        showDisplay(seg2);
        turn = 0;
      }
  if (flexitimer_interrupts == FLEXITIMER_COUNTS) {
    flexitimer_interrupts = 0;
    censeg++;
    if (censeg == 10) {
      censeg=0;
      decseg++;
      if (decseg == 10) {
        decseg = 0;
        seg++;
        if (seg == 10) {
          seg = 0;
          seg2++;
          if (seg2 == 10) {
            seg2 = 0;
          }
        }
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  DDRD = 0b11111100;
  DDRB = 0b00111111;
  DDRC = 0b00000001;
  PORTD=0;
  PORTB=0;
  PORTC=0;
  censeg = 0;
  decseg = 0;
  seg = 0;
  seg2 = 0;
  turn = 0;
  flexitimer_interrupts = 0;
  
  FlexiTimer2::set(1,resolution,blink2);
 // FlexiTimer2::set(DELAY_MSECS_Flexitimer, blink);
  FlexiTimer2::start();
  
}


void loop() {
  // only loops
}
