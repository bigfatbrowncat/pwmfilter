/*
 * sinewave_pcm
 *
 * Generates 8-bit PCM sinewave on pin 6 using pulse-width modulation (PWM).
 * For Arduino with Atmega368P at 16 MHz.
 *
 * Uses timers 1 and 0. Timer 1 reads the sinewave table, SAMPLE_RATE times a
second.
 * The sinewave table has sinewave_length entries. Consequently, the sinewave has a
frequency of
 * f = SAMPLE_RATE / sinewave_length
 * Each entry in the sinewave table defines the duty-cycle of Timer 0. Timer
0
 * holds pin 6 high from 0 to 255 ticks out of a 256-tick cycle, depending on
 * the current duty cycle. Timer 0 repeats 16000000/sinewave_length times per second
 * much faster than the generated sinewave generated frequency.
 *
 * References:
 * http://www.atmel.com/dyn/resources/prod_documents/doc2542.pdf
 * http://www.analog.com/library/analogdialogue/archives/38-08/dds.html
 * http://www.evilmadscientist.com/article.php/avrdac
 * http://www.arduino.cc/playground/Code/R2APCMAudio
 * http://www.scienceprog.com/generate-sine-wave-modulated-pwm-with-avr-
microcontroller/
 * http://www.scienceprog.com/avr-dds-signal-generator-v10/
 * http://documentation.renesas.com/eng/products/region/rtas/mpumcu/apn/
sinewave.pdf
 * http://ww1.microchip.com/downloads/en/AppNotes/00655a.pdf
 *
 * By Gary Hill
 * Adapted from a script by Michael Smith <michael@hurts.ca>
 */
 
#include <math.h>

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define SAMPLE_RATE 22050
// 8 ksps
/*
 * The sinewave data needs to be unsigned, 8-bit
 *
 * sinewavedata.h should look like this:
 * const int sinewave_length=256;
 *
 * const unsigned char sinewave_data[] PROGMEM = {0x80,0x83, ...
 *
 */

//#include "sinewavedata.h"
/* Sinewave table
 * Reference:
 * http://www.scienceprog.com/generate-sine-wave-modulated-pwm-with-avr-
microcontroller/
 */

const int sinewave_length = 512;
const unsigned char sinewave_data[] PROGMEM = {
	0x80, 0x81, 0x83, 0x84, 0x86, 0x87, 0x89, 0x8a, 0x8c, 0x8e, 0x8f, 0x91, 0x92, 0x94, 0x95, 0x97, 
	0x98, 0x9a, 0x9c, 0x9d, 0x9f, 0xa0, 0xa2, 0xa3, 0xa5, 0xa6, 0xa8, 0xa9, 0xab, 0xac, 0xae, 0xaf, 
	0xb0, 0xb2, 0xb3, 0xb5, 0xb6, 0xb8, 0xb9, 0xba, 0xbc, 0xbd, 0xbf, 0xc0, 0xc1, 0xc3, 0xc4, 0xc5, 
	0xc7, 0xc8, 0xc9, 0xca, 0xcc, 0xcd, 0xce, 0xcf, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd7, 0xd8, 0xd9, 
	0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 
	0xea, 0xeb, 0xec, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf0, 0xf1, 0xf2, 0xf3, 0xf3, 0xf4, 0xf5, 0xf5, 
	0xf6, 0xf6, 0xf7, 0xf7, 0xf8, 0xf9, 0xf9, 0xfa, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfd, 
	0xfd, 0xfd, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 
	0xfd, 0xfd, 0xfc, 0xfc, 0xfc, 0xfb, 0xfb, 0xfa, 0xfa, 0xfa, 0xf9, 0xf9, 0xf8, 0xf7, 0xf7, 0xf6, 
	0xf6, 0xf5, 0xf5, 0xf4, 0xf3, 0xf3, 0xf2, 0xf1, 0xf0, 0xf0, 0xef, 0xee, 0xed, 0xec, 0xec, 0xeb, 
	0xea, 0xe9, 0xe8, 0xe7, 0xe6, 0xe5, 0xe4, 0xe3, 0xe2, 0xe1, 0xe0, 0xdf, 0xde, 0xdd, 0xdc, 0xdb, 
	0xda, 0xd9, 0xd8, 0xd7, 0xd5, 0xd4, 0xd3, 0xd2, 0xd1, 0xcf, 0xce, 0xcd, 0xcc, 0xca, 0xc9, 0xc8, 
	0xc7, 0xc5, 0xc4, 0xc3, 0xc1, 0xc0, 0xbf, 0xbd, 0xbc, 0xba, 0xb9, 0xb8, 0xb6, 0xb5, 0xb3, 0xb2, 
	0xb0, 0xaf, 0xae, 0xac, 0xab, 0xa9, 0xa8, 0xa6, 0xa5, 0xa3, 0xa2, 0xa0, 0x9f, 0x9d, 0x9c, 0x9a, 
	0x98, 0x97, 0x95, 0x94, 0x92, 0x91, 0x8f, 0x8e, 0x8c, 0x8a, 0x89, 0x87, 0x86, 0x84, 0x83, 0x81, 
	0x80, 0x7e, 0x7c, 0x7b, 0x79, 0x78, 0x76, 0x75, 0x73, 0x71, 0x70, 0x6e, 0x6d, 0x6b, 0x6a, 0x68, 
	0x67, 0x65, 0x63, 0x62, 0x60, 0x5f, 0x5d, 0x5c, 0x5a, 0x59, 0x57, 0x56, 0x54, 0x53, 0x51, 0x50, 
	0x4f, 0x4d, 0x4c, 0x4a, 0x49, 0x47, 0x46, 0x45, 0x43, 0x42, 0x40, 0x3f, 0x3e, 0x3c, 0x3b, 0x3a, 
	0x38, 0x37, 0x36, 0x35, 0x33, 0x32, 0x31, 0x30, 0x2e, 0x2d, 0x2c, 0x2b, 0x2a, 0x28, 0x27, 0x26, 
	0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 
	0x15, 0x14, 0x13, 0x13, 0x12, 0x11, 0x10, 0x0f, 0x0f, 0x0e, 0x0d, 0x0c, 0x0c, 0x0b, 0x0a, 0x0a, 
	0x09, 0x09, 0x08, 0x08, 0x07, 0x06, 0x06, 0x05, 0x05, 0x05, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02, 
	0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 
	0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06, 0x07, 0x08, 0x08, 0x09, 
	0x09, 0x0a, 0x0a, 0x0b, 0x0c, 0x0c, 0x0d, 0x0e, 0x0f, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x13, 0x14, 
	0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 
	0x25, 0x26, 0x27, 0x28, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x30, 0x31, 0x32, 0x33, 0x35, 0x36, 0x37, 
	0x38, 0x3a, 0x3b, 0x3c, 0x3e, 0x3f, 0x40, 0x42, 0x43, 0x45, 0x46, 0x47, 0x49, 0x4a, 0x4c, 0x4d, 
	0x4f, 0x50, 0x51, 0x53, 0x54, 0x56, 0x57, 0x59, 0x5a, 0x5c, 0x5d, 0x5f, 0x60, 0x62, 0x63, 0x65, 
	0x67, 0x68, 0x6a, 0x6b, 0x6d, 0x6e, 0x70, 0x71, 0x73, 0x75, 0x76, 0x78, 0x79, 0x7b, 0x7c, 0x7e
};

const double tones[] = {
  1, 1.05946, 1.12246, 1.18920, 1.25992, 1.33483, 1.41421, 1.49830, 1.58740, 1.68179, 1.78179, 1.88774
};

double tune = 2.5;  // Tuning coefficient

#define  NNO   0
#define  NC    1
#define  NCS   2
#define  ND    3
#define  NDS   4
#define  NE    5
#define  NF    6
#define  NFS   7
#define  NG    8
#define  NGS   9
#define  NA    10
#define  NAS   11

int melody_notes_count = 48 * 8;
const int melody_notes[] = {
  NF, NF, NF, NF, NNO, NNO, NC, NC, NC, NC, NNO, NNO, NF, NF, NF, NF, NNO, NNO, 
  NC, NC, NC, NC, NNO, NNO, NF, NF, NF, NF, NNO, NNO, NE, NE, NE, NE, NNO, NNO, 
  NE, NE, NE, NE, NE, NE, NNO, NNO, NNO, NNO, NNO, NNO,    //48 v trave sidel kuznechik
  
  NE, NE, NE, NE, NNO, NNO, NC, NC, NC, NC, NNO, NNO, NE, NE, NE, NE, NNO, NNO, 
  NC, NC, NC, NC, NNO, NNO, NE, NE, NE, NE, NNO, NNO, NF, NF, NF, NF, NNO, NNO, 
  NF, NF, NF, NF, NF, NF, NNO, NNO, NNO, NNO, NNO, NNO,    //48 v trave sidel kuznechik

  NF, NF, NF, NF, NNO, NNO, NC, NC, NC, NC, NNO, NNO, NF, NF, NF, NF, NNO, NNO, 
  NC, NC, NC, NC, NNO, NNO, NF, NF, NF, NF, NNO, NNO, NE, NE, NE, NE, NNO, NNO, 
  NE, NE, NE, NE, NE, NE, NNO, NNO, NNO, NNO, NNO, NNO,    //48 sovsem kak ogurechik
  
  NE, NE, NE, NE, NNO, NNO, NC, NC, NC, NC, NNO, NNO, NE, NE, NE, NE, NNO, NNO, 
  NC, NC, NC, NC, NNO, NNO, NE, NE, NE, NE, NNO, NNO, NF, NF, NF, NF, NF, NF, 
  NF, NF, NF, NF, NF, NF, NNO, NNO, NNO, NNO, NNO, NNO,    //48 zelyonenkiy on byl zelyonenkiy on byl
  
  NF, NF, NF, NF, NNO, NNO, NG, NG, NG, NG, NNO, NNO, NG, NG, NNO, NG, NG, NNO, NG, NG, NG, NG, NNO, NNO, // 24
  NG, NG, NG, NG, NNO, NNO, NGS, NGS, NGS, NGS, NNO, NNO, NGS, NGS, NNO, NGS, NGS, NNO, NGS, NGS, NGS, NGS, NNO, NNO, // 24

  NGS, NGS, NGS, NGS, NNO, NNO, NGS, NGS, NGS, NGS, NNO, NNO, NG, NG, NG, NG, NNO, NNO, 
  NF, NF, NF, NF, NNO, NNO, NE, NE, NE, NE, NNO, NNO, NF, NF, NF, NF, NNO, NNO, 
  NF, NF, NF, NF, NF, NF, NNO, NNO, NNO, NNO, NNO, NNO,    //48 zelyonenkiy on byl zelyonenkiy on byl*/

  NF, NF, NF, NF, NNO, NNO, NG, NG, NG, NG, NNO, NNO, NG, NG, NNO, NG, NG, NNO, NG, NG, NG, NG, NNO, NNO, // 24
  NG, NG, NG, NG, NNO, NNO, NGS, NGS, NGS, NGS, NNO, NNO, NGS, NGS, NNO, NGS, NGS, NNO, NGS, NGS, NGS, NGS, NNO, NNO, // 24

  NGS, NGS, NGS, NGS, NNO, NNO, NGS, NGS, NGS, NGS, NNO, NNO, NG, NGS, NG, NG, NNO, NNO, 
  NF, NF, NF, NF, NNO, NNO, NE, NE, NE, NE, NNO, NNO, NF, NF, NF, NF, NF, NF, 
  NF, NF, NF, NF, NF, NF, NNO, NNO, NNO, NNO, NNO, NNO,    //48 zelyonenkiy on byl zelyonenkiy on byl*/
};

double tone(int k, int octave_mp) {
  return tune * octave_mp * tones[k - 1];
}

int outputPin = 6; // (PCINT22/OC0A/AIN0)PD6, Arduino Digital Pin 6
volatile double sample;
volatile int note_len = SAMPLE_RATE / 24; // Eights
volatile int pos = 0, note = 0;
volatile bool up = true;

// This is called at SAMPLE_RATE kHz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (sample >= sinewave_length) {
      sample -= sinewave_length;
    }
    
    OCR0A = pgm_read_byte(&sinewave_data[(int)(sample)]);
    
    if (melody_notes[note] == NNO) sample = 0; else {
      sample += tone(melody_notes[note], 4);
    }
    pos ++;
    
    if (pos == note_len) {
      pos = 0;
      note ++;
    }
    
    if (note == melody_notes_count) {
      note = 0;
    }
}

void startPlayback()
{
    pinMode(outputPin, OUTPUT);
    // Set Timer 0 Fast PWM Mode (Section 14.7.3)
    // WGM = 0b011 = 3  (Table 14-8)
    // TOP = 0xFF, update OCR0A register at BOTTOM
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B &= ~_BV(WGM02);
    // Do non-inverting PWM on pin OC0A, arduino digital pin 6
    // COM0A = 0b10, clear OC0A pin on compare match,
    //               set 0C0A pin at BOTTOM (Table 14-3)
    TCCR0A = (TCCR0A | _BV(COM0A1)) & ~_BV(COM0A0);
    // COM0B = 0b00, OC0B disconnected (Table 14-6)
    TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0));
    // No prescaler, CS = 0b001 (Table 14-9)
    TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
    // Set initial pulse width to the first sample.
    OCR0A = pgm_read_byte(&sinewave_data[0]);
    // Set up Timer 1 to send a sample every interrupt.
    cli();   // disable interrupts
    // Set CTC mode (Section 15.9.2 Clear Timer on Compare Match)
    // WGM = 0b0100, TOP = OCR1A, Update 0CR1A Immediate (Table 15-4)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
    // No prescaler, CS = 0b001 (Table 15-5)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000
    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    sample = 0;

    sei();  // enable interrupts
}
void setup()
{
    startPlayback();
}
void loop() {
    while (true);
}

