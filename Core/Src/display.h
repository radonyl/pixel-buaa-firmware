#ifndef PIXEL_BUAA_FIRMWARE_DISPLAY_H
#define PIXEL_BUAA_FIRMWARE_DISPLAY_H

#include "led_matrix.h"
#include "stdlib.h"

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define SQR(x) ((x)*(x))

#define TIME_SCALE 10 // virtual vs real
#define TIME_INIT_VALUE 6 * 3600 * 1000 // in millisecond

extern uint8_t time_accelerate;
extern uint64_t time_counter;
extern uint8_t time_hour;

void display_set_accelerate(uint8_t on);

void display_init();

void display_tick_callback();

void display_task(char morse_decode_result);

void animate_heart();

void animate_70();

void working_building_with_letter(char c, uint8_t pwm);

void working_building_init_random(uint8_t percent);

#endif //PIXEL_BUAA_FIRMWARE_DISPLAY_H
