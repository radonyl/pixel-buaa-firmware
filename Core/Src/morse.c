#include "morse.h"

Morse morse_decoder;

const char *letter = "**ETIANMSURWDKGOHVF?L?PJBXCYZQ??";

void update_result(Morse *handle) {
  /**
   * Example data in handle->input: {300, 100, 120, 110}
   * in morse code, "-" (long marker) should be three times long of "." (short
   * marker) judging: int(period * 4 / input_min) >= 8 ? LONG : SHORT
   */

  // 1. find max and min of input series
  uint32_t min_press = 999999;
  uint32_t max_press = 0;
  for (uint8_t i = 0; i < handle->input_count; i++) {
    if (handle->input[i] < min_press) {
      min_press = handle->input[i];
    }
    if (handle->input[i] > max_press) {
      max_press = handle->input[i];
    }
  }
  // 2. if difference between "long" and "short" is small, half the divisor
  if (max_press / min_press < 2) {
    if (max_press > 300) {
      min_press = min_press >> 1;
    }
  }

  // 3. do decode
  uint8_t state = 1;
  for (uint8_t i = 0; i < handle->input_count; i++) {
    if ((handle->input[i] << 2) / min_press >= 8) {
      // long marker
      state = state * 2 + 1;
    } else {
      // short marker
      state = state * 2;
    }
  }

  if (state < 32) {
    handle->result = letter[state];
  } else {
    handle->result = '*';
  }

  // 4. reset state
  handle->input_count = 0;
}

void morse_loop(Morse *handle, uint8_t button_input) {
  if (get_tick() - handle->last_update > MORSE_TIMEOUT && handle->input_count) {
    // if timeout
    if (handle->last_input == BI_DOWN) {
      // if the button is kept pressed, exit morse mode and set decode result to
      // ' '
      handle->input[handle->input_count - 1] =
          get_tick() - handle->last_press_down;
      // reset state
      handle->result = ' ';
      handle->input_count = 0;
      handle->last_input = BI_NONE;
    } else {
      // user input complete, update result
      handle->last_input = BI_NONE;
      update_result(handle);
    }
  } else {
    // no news, return
    if (button_input == BI_NONE)
      return;

    uint32_t now = get_tick();
    switch (button_input) {
    case BI_DOWN:
      if (handle->last_input != BI_DOWN) {
        // if press down, record tick now, increase input count
        handle->last_press_down = now;
        handle->last_input = BI_DOWN;
        handle->input_count++;
      }
      break;
    case BI_UP:
      if (get_tick() - handle->last_press_down > 0 &&
          handle->last_input == BI_DOWN) {
        // record ticks between [press down -> press up]
        handle->input[handle->input_count - 1] = now - handle->last_press_down;
        handle->last_input = BI_UP;
      }
      break;
    default:
      break;
    }
    handle->last_update = now;
  }
}