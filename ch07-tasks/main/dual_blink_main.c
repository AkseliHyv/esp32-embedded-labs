#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "esp_rom_sys.h"
#include <stdint.h>
#include "soc/gpio_reg.h"

#define LED_1_PIN 18
#define LED_1_HZ 2
#define LED_2_PIN 2
#define LED_2_HZ 2

gpio_config_t led_1_config = {
    .pin_bit_mask = 1ULL << LED_1_PIN,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

gpio_config_t led_2_config = {
    .pin_bit_mask = 1ULL << LED_2_PIN,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

typedef struct {
    int pin;
    int hz;
    bool on;
    bool delay_until;
} led;

static void blink_led(void *param) {
    led *l = (led *)param;
    TickType_t last_wake = xTaskGetTickCount();
    while (1)
    {
        if (l->on == false) {
            REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << l->pin);
        }
        else {
            REG_WRITE(GPIO_OUT_W1TC_REG, 1ULL << l->pin);

        }

        l->on = !l->on;

        UBaseType_t free = uxTaskGetStackHighWaterMark(NULL);
        printf("%d\n", free);

        // Fake work: busy wait 0-3 ms
        esp_rom_delay_us(esp_random() % 3001);

        if (l->delay_until) {
            xTaskDelayUntil(&last_wake, pdMS_TO_TICKS(1000 / l->hz));
        } else {
            vTaskDelay(pdMS_TO_TICKS(1000 / l->hz));
        }
    }
}

static led led_1 = {LED_1_PIN, LED_1_HZ, false, false};
static led led_2 = {LED_2_PIN, LED_2_HZ, false, true};

void app_main(void) {
    ESP_ERROR_CHECK(gpio_config(&led_1_config));
    ESP_ERROR_CHECK(gpio_config(&led_2_config));

    xTaskCreatePinnedToCore(blink_led, "blink_led_1", 4096, &led_1, 5, NULL, 0);
    xTaskCreatePinnedToCore(blink_led, "blink_led_2", 4096, &led_2, 5, NULL, 1);
}