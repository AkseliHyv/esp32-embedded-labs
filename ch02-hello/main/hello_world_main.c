/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"

void app_main(void)
{
    size_t internal_free = 0;
    size_t psram_free = 0;
    int64_t running_for = 0;

    while (1)
    {
        internal_free = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        psram_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        running_for = esp_timer_get_time();
        printf("Free internal heap: %zuKB\nFree PSRAM heap: %zuKB\nTime device has been running: %lld\n----\n",
        internal_free / 1024, psram_free / 1024, running_for / 1000000);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
