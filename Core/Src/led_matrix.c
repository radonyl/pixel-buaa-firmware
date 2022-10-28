#include "led_matrix.h"

/**
 * control matrix power
 * @param sleep : 1 - shutdown, 0 - working
 */
uint8_t power_state = 0;

void matrix_set_sleep(uint8_t sleep) {
  IS31FL3731_set_sleep(ADDR, sleep);
  power_state = sleep ? 0 : 1;
}

/**
 * map x, y and region to the matrix driver index.
 * @param led_region : region of led
 * @param x : nature coordinate x
 * @param y : nature coordinate y
 * @return the index
 */
uint8_t matrix_get_index(led_region_t led_region, uint8_t x, uint8_t y) {
  /**  Matrix Reference
       R: Red only / W: While Only / B: Red and While / x: No LED
       0 R R R R R R R R           <- TITLE
         0 1 2 3 4 5 6 7 8 9 A -> x
       0 W W W B B B B B W W W     <- BUILDING
       1 W W W B B B B B W W W
       2 W W W B B B B B W W W
       3 W W W B B B B B W W W
       4   W W   B B B   W
       5     W B B B B B W
       y
  */

  switch (led_region) {
  case LED_TITLE:
    if (x >= 8 || y > 0)
      return INDEX_404;
    return x;

  case LED_WHITE:
    if (x > 10 || y > 5)
      return INDEX_404;
    if (y <= 3)
      return x + y * 16 + 8;
    else if (y == 4) {
      if (0 <= x && x <= 2) {
        return x + 72;
      } else if (4 <= x && x <= 6) {
        return x + 71;
      } else if (8 <= x && x <= 10) {
        return x + 70;
      }
      return INDEX_404;
    } else if (y == 5) {
      if (0 <= x && x <= 10) {
        return 84 + x;
      }
      return INDEX_404;
    }
    break;

  case LED_RED:
    if (x > 4 || y > 5)
      return INDEX_404;
    if (y <= 3)
      return 19 + 16 * y + x;
    else if (y == 5) {
      if (1 <= x && x <= 3) {
        return 80 + x;
      }
    } else if (y == 4) {
      return 95 + x;
    }
    break;

  default:
    return INDEX_404;
  }
  return INDEX_404;
}

/**
 * write pwm to a pixel in matrix
 * @param led_region : region of led
 * @param x : coordinate
 * @param y : coordinate
 * @param pwm : pwm value 0x00~0xff
 */
void matrix_write_pixel(led_region_t led_region, uint8_t x, uint8_t y,
                        uint8_t pwm) {
  uint8_t index = matrix_get_index(led_region, x, y);
  if (index != INDEX_404)
    IS31FL3731_set_pwm(index, pwm);
}

/**
 * update led matrix
 */
void matrix_update() { IS31FL3731_update_pwm_buffers(ADDR, 0); }

/**
 * set all led to the same pwm
 * @param pwm : valid from 0x00(off) - 0xff(fully on)
 */
void matrix_set_all(uint8_t pwm) { IS31FL3731_set_pwm_all(pwm); }

/**
 * write all red area (5x6) with a array length of 5 (unchecked)
 * @param buf: see diagram
 * @param pwm: pwm value
 */
void matrix_write_red(const char *buf, uint8_t pwm) {
  /** buf = {0x01, 0x02, 0x04, 0x0A, 0x11}
   *     0 1 2 3 4
   *  0  X       X
   *  1    X   X
   *  2      X
   *  3        X
   *  4          X
   *  5
   */
  for (uint8_t y = 0; y <= 5; y++) {
    for (uint8_t x = 0; x <= 4; x++) {
      if ((buf[x] >> y) & 0x1) {
        matrix_write_pixel(LED_RED, x, y, pwm);
      } else {
        matrix_write_pixel(LED_RED, x, y, 0);
      }
    }
  }
}

/**
 * write all white area (11x6) with a array length of 1 (unchecked)
 * @param buf: see diagram
 * @param pwm: pwm value
 */
void matrix_write_white(const char *buf, uint8_t pwm) {
  for (uint8_t y = 0; y <= 5; y++) {
    for (uint8_t x = 0; x <= 10; x++) {
      if ((buf[x] >> y) & 0x1) {
        matrix_write_pixel(LED_WHITE, x, y, pwm);
      } else {
        matrix_write_pixel(LED_WHITE, x, y, 0);
      }
    }
  }
}

/**
 * write title area (8x1) with a mast(uint8)
 * @param mask : left aligned mask
 * @param pwm : pwm value
 */
void matrix_write_title(uint8_t mask, uint8_t pwm) {
  /**
   * 北 京 航 空 航 天 大 学
   * 1  1  1  1  1  1  1  1 = 0xff
   */
  for (uint8_t i = 0; i < 8; i++) {
    if ((mask >> (7 - i)) & 1)
      matrix_write_pixel(LED_TITLE, i, 0, pwm);
    else
      matrix_write_pixel(LED_TITLE, i, 0, 0x00);
  }
}

/**
 * init matrix, must be called before use other functions
 */
void matrix_init() {
  IS31FL3731_init(ADDR);
  for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
    IS31FL3731_set_led_control_register(i, true);
  }
  IS31FL3731_update_led_control_registers(ADDR, 0);
  matrix_set_all(0x00);
  matrix_update();
  matrix_set_sleep(0);
}
