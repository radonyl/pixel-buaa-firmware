#include "button.h"
#include "display.h"
#include "morse.h"

struct Button button1;
extern TIM_HandleTypeDef htim2;

/**
 * Overload default callback, called everytime TIM2 overflows.
 * TIM2 is used for key scanning, default period is 5ms (200Hz)
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == (&htim2)) {
    button_ticks();
  }
}

uint8_t BTN1_Read() { return HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin); }

/**
 * BTN1 Press Up Event Callback
 * Called in a interruption, be careful not to get blocked.
 * @param btn
 */
void BTN1_PRESS_UP_Handler(void *btn) {
  morse_loop(&morse_decoder, BI_UP);
  display_set_accelerate(0);
}

/**
 * BTN1 Press Down Event Callback
 * Called in a interruption, be careful not to get blocked.
 * @param btn
 */
void BTN1_PRESS_DOWN_Handler(void *btn) { morse_loop(&morse_decoder, BI_DOWN); }

/**
 * BTN1 Long Press Hold Event Callback
 * Called in a interruption, be careful not to get blocked.
 * @param btn
 */
void BTN1_LONG_PRESS_HOLD_Handler(void *btn) { display_set_accelerate(1); }

/**
 * Init MultiButton library
 */
void configure_button() {
  button_init(&button1, BTN1_Read, 1);
  button_attach(&button1, PRESS_DOWN, BTN1_PRESS_DOWN_Handler);
  button_attach(&button1, PRESS_UP, BTN1_PRESS_UP_Handler);
  button_attach(&button1, LONG_PRESS_HOLD, BTN1_LONG_PRESS_HOLD_Handler);
  button_start(&button1);
}
