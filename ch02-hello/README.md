# ch02-hello - Hello, chip: the build-flash-run pipeline

Building and flashing a first program, and configuring the project for this board.

## Goal

Learn the build, flash and monitor loop, configure ESP-IDF for the board's octal flash and PSRAM, and change the
hello_world example to print system info.

## What I did

- Copied ESP-IDF's hello_world example and set the target
    ```
    get_idf
    cp -r $IDF_PATH/examples/get-started/hello_world ~/Projects/esp32-embedded-labs/ch02-hello
    cd ~/Projects/esp32-embedded-labs/ch02-hello
    idf.py set-target esp32s3
    ```
- Set these options in menuconfig and saved them to sdkconfig.defaults with idf.py save-defconfig
    ```
    ESPTOOLPY_OCT_FLASH=y
    ESPTOOLPY_FLASHSIZE_32MB=y
    SPIRAM=y
    SPIRAM_MODE_OCT=y
    ESP_DEFAULT_CPU_FREQ_MHZ_240=y
    ESP_CONSOLE_USB_SERIAL_JTAG=y
    ```
- Built, flashed and opened the monitor
    ```
    idf.py build
    idf.py -p /dev/ttyACM0 flash monitor
    ```
- Changed the program to print the free internal heap, the free PSRAM heap and the uptime every 2 seconds

## Results

Boot log:

- Bootloader: I (32) boot: ESP-IDF v6.1 2nd stage bootloader
- Chip revision: v0.2
- Flash: 32MB, mode SLOW READ
- PSRAM: 16MB found, 16384KB added to the heap
- Last line before my code: I (1518) main_task: Calling app_main()
- Partition table:

    | # | Label | Usage | Type | Subtype | Offset | Length |
    | --- | --- | --- | --- | --- | --- | --- |
    | 0 | nvs | WiFi data | 01 | 02 | 0x9000 | 0x6000 |
    | 1 | phy_init | RF data | 01 | 01 | 0xf000 | 0x1000 |
    | 2 | factory | factory app | 00 | 00 | 0x10000 | 0x100000 |

    The app is in the factory partition, which starts at 0x10000 and is 1MB.

Program:

- Free internal heap: 381KB
- Free PSRAM heap: 16381KB
- Uptime goes up by 2 every print
- App size: 175188 bytes

How the chip boots:

1. ROM bootloader: fixed in the chip. If the BOOT button is held, it waits for new firmware. Otherwise it loads
   the second-stage bootloader from flash.
2. Second-stage bootloader: sets up flash and clocks, reads the partition table at 0x8000 and loads the app.
3. App startup: ESP-IDF sets up memory, PSRAM, the heap and FreeRTOS, then calls app_main.

The partition table describes how the flash is divided.

## Problems and fixes

- PSRAM settings had no effect. The example's CMakeLists.txt turns on MINIMAL_BUILD, which leaves out the
  esp_psram component. Fixed by commenting out that line.
- esp_timer.h was not found. A component can only include headers from components it lists in PRIV_REQUIRES,
  and esp_timer was missing. Fixed by adding it.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
