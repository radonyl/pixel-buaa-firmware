#include "../display.h"

const char word[11] = {0x01, 0x0d, 0x03, 0x20, 0x20, 0x20,
                       0x20, 0x20, 0x06, 0x09, 0x06};

void animate_70() {
  // update every 6 frames
  static uint8_t div = 0;
  if (div++ % 6 != 0) {
    return;
  }

  const uint8_t brightness = 0x20;
  // draw word "70"
  matrix_set_all(0x00);
  matrix_write_title(0xff, brightness);
  matrix_write_white(word, brightness);
  // draw red rolling arrow
  static uint8_t ct = 0;
  const uint8_t offset[5] = {0, 1, 2, 1, 0};
  for (uint8_t x = 0; x < 5; x++) {
    for (uint8_t y = 0; y < 6; y++) {
      if ((y + offset[x] + ct) % 5 == 0) {
        matrix_write_pixel(LED_RED, x, y, brightness);
      }
    }
  }

  ct = (ct + 1) % 5;
  // commit buffer to led matrix
  matrix_update();
}
