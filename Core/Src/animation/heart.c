#include "../display.h"

const char heart_red[5] = {0x02, 0x07, 0x0e, 0x07, 0x02};
uint8_t sweep[] = {1,  2,  3,  4,  6,  8,  10, 15, 20, 30, 40, 60,
                   60, 40, 30, 20, 15, 10, 8,  6,  4,  3,  2,  1};

void animate_heart() {
  /**
   * Display a beating heart on a sweeping background
   */
  static int brt = 0;
  static int inc = 8;
  static uint8_t incr = 0;
  // sweeping background
  for (uint8_t x = 0; x < 11; x++) {
    for (uint8_t y = 0; y < 6; y++) {
      if ((x >= 3 && x <= 7) && (y >= 0 && y <= 3)) {
        matrix_write_pixel(LED_WHITE, x, y, sweep[(x + y + incr) % 24] >> 1);
      } else {
        matrix_write_pixel(LED_WHITE, x, y, sweep[(x + y + incr) % 24] >> 0);
      }
    }
  }

  // gate light keep constant
  for (uint8_t x = 4; x <= 6; x++)
    matrix_write_pixel(LED_WHITE, x, 4, 0x00);
  for (uint8_t x = 3; x <= 7; x++)
    matrix_write_pixel(LED_WHITE, x, 5, 0x20);

  // heart glyph
  matrix_write_red(heart_red, brt);
  if (brt + inc > 0xa0 || brt + inc < 0x0) {
    inc = -inc;
  }
  brt += inc;
  incr = incr < 23 ? incr + 1 : 0;

  matrix_update();
}