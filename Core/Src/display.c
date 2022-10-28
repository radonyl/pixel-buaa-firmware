#include "display.h"

/**
 * Time system
 * time_counter: in millisecond, get increased in display_tick_callback();
 * time_hour: hour of day, calculated from time_counter;
 * time_accelerate: let time flows faster
 *
 * display_tick_callback() should be added to SysTick_Handler()
 */

uint8_t time_accelerate = 0;
uint64_t time_counter = TIME_INIT_VALUE;
uint8_t time_hour;

void display_set_accelerate(uint8_t on) { time_accelerate = on; }

void display_tick_callback() {
  time_counter += time_accelerate ? TIME_SCALE * 1000 : TIME_SCALE;
  time_hour = (time_counter / 1000 / 3600) % 24;
}

/**
 * Init display
 */
void display_init() {
  matrix_init();
  working_building_init_random(40);
}

/**
 * Display Loop
 * 1. Simulate building lights base on "timelapse.c" method;
 * 2. If a morse code is decoded, superimpose on (1);
 * 3. If the morse code hit a "key letter", get "excited" and
 *    display specific animate until another code input.
 */
const uint8_t max_excited_delay = 60;
uint8_t excited = 0;
uint8_t excited_counter = 0;
char excited_on = 0;

void (*excited_animate)(void);

void display_task(char morse_decode_result) {
  if (excited == 0) {
    working_building_with_letter(morse_decode_result, 0xff);
    excited = 1;
    excited_counter = 60;
    switch (morse_decode_result) {
    case 'F':
      excited_animate = &animate_heart;
      break;
    case 'B':
      excited_animate = &animate_70;
      break;
    default:
      excited = 0;
    }
    if (excited) {
      excited_on = morse_decode_result;
    }
  } else {
    // Fade this hit letter before show animate
    while (excited_counter) {
      working_building_with_letter(morse_decode_result,
                                   0xff / max_excited_delay * excited_counter);
      excited_counter--;
      return;
    }

    if (morse_decode_result == excited_on) {
      excited_animate();
    } else {
      excited = 0;
    }
  }
}
