# ch02-hello - Hello, chip: the build-flash-run pipeline

Flashing a C program onto the ESP32-S3 and customizing the project config.

## Goal

Go through the full ESP-IDF build, flash and monitor loop once, configure the project for the board's
octal flash and PSRAM, and modify the stock `hello_world` example to print live system info.

## What I did

- Copied ESP-IDF's `hello_world` example into the repo
    ```
    get_idf
    cp -r $IDF_PATH/examples/get-started/hello_world ~/Projects/esp32-embedded-labs/ch02-hello
    cd ~/Projects/esp32-embedded-labs/ch02-hello
    ```
- Set the target chip
    ```
    idf.py set-target esp32s3
    ```
- Changed these options in `idf.py menuconfig`
    ```
    ESPTOOLPY_OCT_FLASH=y
    ESPTOOLPY_FLASHSIZE_32MB=y
    SPIRAM=y
    SPIRAM_MODE_OCT=y
    ESP_DEFAULT_CPU_FREQ_MHZ_240=y
    ESP_CONSOLE_USB_SERIAL_JTAG=y
    ```
- Saved the non-default options into `sdkconfig.defaults`, so the full `sdkconfig` can be regenerated
    ```
    idf.py save-defconfig
    ```
- Built, flashed and opened the serial monitor
    ```
    idf.py build
    idf.py -p /dev/ttyACM0 flash monitor
    ```
- Modified `main/hello_world_main.c` to print, every 2 seconds:
    - Free internal heap (`heap_caps_get_free_size(MALLOC_CAP_INTERNAL)`)
    - Free PSRAM heap (`heap_caps_get_free_size(MALLOC_CAP_SPIRAM)`)
    - Uptime in seconds (`esp_timer_get_time() / 1000000`)

## Results

Boot log from the ESP32-S3-DevKitC-1:

- Bootloader: `I (32) boot: ESP-IDF v6.1 2nd stage bootloader`
- Chip revision: v0.2
- Flash size: 32MB
- Flash mode: SLOW READ
- PSRAM found: 16MB, all 16384KB added to the heap
- Last line before user code runs: `I (1518) main_task: Calling app_main()`
- Partition table:

    | # | Label | Usage | Type | Subtype | Offset | Length |
    | --- | --- | --- | --- | --- | --- | --- |
    | 0 | nvs | WiFi data | 01 | 02 | 0x9000 | 0x6000 |
    | 1 | phy_init | RF data | 01 | 01 | 0xf000 | 0x1000 |
    | 2 | factory | factory app | 00 | 00 | 0x10000 | 0x100000 (1MB) |

Program output:

- Free internal heap: 381KB
- Free PSRAM heap: 16381KB
- Uptime counter increased by 2 every print, as expected

## Problems and fixes

- **PSRAM options missing / not taking effect.**
  The stock example's top-level `CMakeLists.txt` has `idf_build_set_property(MINIMAL_BUILD ON)`, which only
  builds `main` and the components it depends on. Components outside that set (like `esp_psram`) aren't part
  of the build, so their config options don't apply.
  Fix: commented out the `MINIMAL_BUILD` line so the full component set is included.
- **`esp_timer.h: No such file or directory`.**
  `main/CMakeLists.txt` lists its dependencies with `PRIV_REQUIRES spi_flash`, and a component can only include
  headers from components it declares. `esp_timer` wasn't listed.
  Fix: `PRIV_REQUIRES spi_flash esp_timer`.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
```
