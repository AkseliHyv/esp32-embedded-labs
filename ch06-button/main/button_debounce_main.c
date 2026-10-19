#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_reg.h"

#define BUTTON_PIN 4
gpio_config_t button_config = {
    .pin_bit_mask = 1ULL << BUTTON_PIN,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

#define LED_PIN 18
gpio_config_t led_config = {
    .pin_bit_mask = 1ULL << LED_PIN,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

#define DEBOUNCE_MS 5
bool debounce(bool raw) {
  static bool last_raw = false;
  static bool state = false;
  static int same_input = 0;

  if (raw == last_raw) {
    same_input++;
  } else {
    same_input = 0;
  }
  last_raw = raw;

  if (same_input >= DEBOUNCE_MS && raw != state) {
    state = raw;
  }
  return state;
}

void app_main(void) {
  ESP_ERROR_CHECK(gpio_config(&button_config));
  ESP_ERROR_CHECK(gpio_config(&led_config));

  bool was_pressed = false;
  int presses = 0;
  while (1) {
    bool raw =
        (REG_READ(GPIO_IN_REG) & 1ULL << BUTTON_PIN) != 1ULL << BUTTON_PIN;
    bool pressed = debounce(raw);
    if (pressed && !was_pressed) {
      presses++;
      printf("Pressed button (presses: %d)\n", presses);
      REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);
    } else if (!pressed && was_pressed) {
      printf("Released button\n");
      REG_WRITE(GPIO_OUT_W1TC_REG, 1ULL << LED_PIN);
    }
    was_pressed = pressed;
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}