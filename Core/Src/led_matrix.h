#ifndef PIXEL_BUAA_FIRMWARE_LED_MATRIX_H
#define PIXEL_BUAA_FIRMWARE_LED_MATRIX_H

#include "is31fl3731.h"

#define ADDR 0b1110100
#define INDEX_404 255

typedef enum { LED_TITLE, LED_WHITE, LED_RED } led_region_t;

void matrix_set_all(uint8_t pwm);

void matrix_update();

void matrix_write_pixel(led_region_t led_region, uint8_t x, uint8_t y,
                        uint8_t pwm);

void matrix_write_red(const char *buf, uint8_t pwm);

void matrix_write_white(const char *buf, uint8_t pwm);

void matrix_write_title(uint8_t mask, uint8_t pwm);

void matrix_set_sleep(uint8_t sleep);

void matrix_init();

#endif // PIXEL_BUAA_FIRMWARE_LED_MATRIX_H
