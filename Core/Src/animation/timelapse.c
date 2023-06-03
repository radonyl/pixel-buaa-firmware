#include "../display.h"
#include "../font5x5.h"

uint8_t windows[6][11] = {0};

uint8_t p_on_work() {
  if (time_hour <= 6)
    return time_hour;
  if (time_hour <= 8)
    return time_hour * time_hour - 10;
  if (time_hour <= 9)
    return 40;
  if (time_hour <= 12)
    return 10;
  if (time_hour <= 14)
    return 5;
  if (time_hour <= 15)
    return 50;
  if (time_hour <= 16)
    return 10;
  if (time_hour <= 18)
    return 5;
  return 1;
}

uint8_t p_off_work() {
  if (time_hour <= 10)
    return 5;
  if (time_hour <= 12)
    return 80;
  if (time_hour <= 15)
    return 10;
  if (time_hour <= 16)
    return 5;
  if (time_hour <= 18)
    return 70;
  if (time_hour <= 20)
    return 30;
  return 20;
}

uint8_t get_random_brightness() {
  if (time_hour >= 8 && time_hour <= 16) {
    return (rand() % 4 + 1) * 25;
  } else {
    return (rand() % 4 + 1) * 63;
  }
}

void working_building_update() {
  /**
   * Update every windows randomly.
   * In Normal / Accelerated mode every window has a change of 0.1% / 10% of
   * updating when called. If this windows was dark, turn on the light to a
   * random brightness with a change of p_on_work; Else, turn off with a change
   * of p_off_work or decay its brightness gradually.
   */
  uint8_t randint;
  for (uint8_t x = 0; x < 11; x++) {
    for (uint8_t y = 0; y < 6; y++) {
      int32_t frequency = time_accelerate ? 10 : 1000; // control update rate
      if (rand() % frequency == 1) {
        randint = rand() % 100;
        if (windows[y][x] == 0) {
          windows[y][x] = p_on_work() > randint ? get_random_brightness() : 0;
        } else {
          windows[y][x] = p_off_work() > randint
                              ? 0
                              : MAX(windows[y][x] - 9 + rand() % 10, 0);
        }
      }
      matrix_write_pixel(LED_WHITE, x, y, windows[y][x]);
    }
  }
}

void working_building_init_random(uint8_t percent) {
  /**
   *  Init the building with random lights
   */
  uint8_t randint;
  for (uint8_t x = 0; x < 11; x++) {
    for (uint8_t y = 0; y < 6; y++) {
      randint = rand() % 100;
      windows[y][x] = percent > randint ? get_random_brightness() : 0;
      matrix_write_pixel(LED_WHITE, x, y, windows[y][x]);
    }
  }
  matrix_update();
}

void working_building_with_letter(char c, uint8_t pwm) {
  /**
   * Update window with a letter placed in center
   */
  static char last_char = ' ';
  working_building_update();

  // draw letter on building
  if (c >= ' ' && c <= 'Z') {
    uint32_t offset = (c - 0x20) * 5;
    matrix_write_red(Font + offset, pwm);
    last_char = c;
  }

  if (c == ' ') {
    if (last_char != ' ') {
      // fill the gap
      working_building_update();
    }
  } else {
    // invalid letter, turn off pixels of code area
    for (uint8_t x = 3; x < 8; x++) {
      for (uint8_t y = 0; y < 6; y++) {
        matrix_write_pixel(LED_WHITE, x, y, 0);
      }
    }
  }

  // adjust gate and title's brightness according to time
  uint8_t h = time_hour;
  if (h > 12) {
    h = 24 - h;
  }
  uint8_t brt = MAX(255 - 2 * SQR(h), 0);
  for (uint8_t x = 3; x <= 7; x++) {
    matrix_write_pixel(LED_WHITE, x, 5, brt);
  }

  matrix_write_title(0xff, brt);
  matrix_update();
}
