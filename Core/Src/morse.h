#ifndef PIXEL_BUAA_FIRMWARE_MORSE_H
#define PIXEL_BUAA_FIRMWARE_MORSE_H

#include "stm32f0xx_hal.h"

#define MORSE_TIMEOUT 600 // Hardware ticks (usually in ms)

typedef enum {
  BI_NONE = 0,
  BI_UP,
  BI_DOWN,
} ButtonInput;

typedef struct {
  ButtonInput last_input;
  uint32_t last_update;
  uint32_t last_press_down;
  uint8_t input_count;
  uint32_t input[10];
  char result;
} Morse;

inline uint32_t get_tick() { return HAL_GetTick(); }

void morse_loop(Morse *handle, uint8_t button_input);

extern Morse morse_decoder;
#endif // PIXEL_BUAA_FIRMWARE_MORSE_H
