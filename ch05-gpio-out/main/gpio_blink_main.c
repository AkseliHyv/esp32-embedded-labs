/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "soc/gpio_reg.h"

#define LED_PIN 18
void blink_loop(void);
void morse(char *string);
const char *char_to_morse(unsigned char c);

static const char *TAG = "ch05";
static const char *morse_table[128] = {
    ['A'] = ".-",    ['B'] = "-...",  ['C'] = "-.-.",  ['D'] = "-..",
    ['E'] = ".",     ['F'] = "..-.",  ['G'] = "--.",   ['H'] = "....",
    ['I'] = "..",    ['J'] = ".---",  ['K'] = "-.-",   ['L'] = ".-..",
    ['M'] = "--",    ['N'] = "-.",    ['O'] = "---",   ['P'] = ".--.",
    ['Q'] = "--.-",  ['R'] = ".-.",   ['S'] = "...",   ['T'] = "-",
    ['U'] = "..-",   ['V'] = "...-",  ['W'] = ".--",   ['X'] = "-..-",
    ['Y'] = "-.--",  ['Z'] = "--..",  ['0'] = "-----", ['1'] = ".----",
    ['2'] = "..---", ['3'] = "...--", ['4'] = "....-", ['5'] = ".....",
    ['6'] = "-....", ['7'] = "--...", ['8'] = "---..", ['9'] = "----.",
};

gpio_config_t led_config = {
    .pin_bit_mask = 1ULL << LED_PIN,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

void app_main(void) {
  ESP_ERROR_CHECK(gpio_config(&led_config));
  //blink_loop();
  /*
  while (1)
  {
    morse("SOS");
    morse("HELLO WORLD");
    morse("THIS IS A SENTENCE");
  }
*/
    REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);
}

void blink_loop(void) {
  uint32_t i = 0;
  while (1) {
    gpio_set_level(LED_PIN, i % 2);
    if (i % 2 == 1) {
      ESP_LOGI(TAG, "LED on");
    } else {
      ESP_LOGI(TAG, "LED off");
    }
    i++;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void morse(char *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    const char *morse = char_to_morse(toupper(string[i]));
    if (morse == NULL)
    {
        vTaskDelay(800 / portTICK_PERIOD_MS);
        continue;
    }
    
    for (int c = 0; morse[c] != '\0'; c++)
    {
        if (morse[c] == '.')
        {
            REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);
            printf("W1TS: %#010x\n", GPIO_OUT_W1TS_REG);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
        else
        {
            REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);
            printf("W1TS: %#010x\n", GPIO_OUT_W1TS_REG);
            vTaskDelay(600 / portTICK_PERIOD_MS);
            
        }
        REG_WRITE(GPIO_OUT_W1TC_REG, 1ULL << LED_PIN);
        printf("W1TC: %#010x\n", GPIO_OUT_W1TC_REG);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}

const char *char_to_morse(unsigned char c)
{
    if (c >= 128)
    {
        return NULL;
    }
    else {
        return morse_table[c];
    }
}