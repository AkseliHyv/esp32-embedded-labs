# esp32-embedded-labs

Hands-on embedded systems labs on the ESP32-S3 and STM32, written in C with ESP-IDF and bare-metal ARM.
These are the learning experiments behind [aksbox](https://github.com/AkseliHyv/aksbox), a handheld game console.

Each lab is a small standalone project focused on one concept. Measurements, bugs and findings are logged in [NOTES.md](NOTES.md).

## Labs

| Lab | Topic | What it covers |
|-----|-------|----------------|
| `ch00-setup` | Toolchain setup | ESP-IDF on Arch via EIM, USB/JTAG permissions, logic analyzer (notes only) |
| `ch02-hello` | Toolchain and boot | Build/flash pipeline, boot log, octal flash and PSRAM configuration |
| `ch03-blink` | Using a driver | Addressable RGB LED over the RMT peripheral, protocol checked with a logic analyzer |
| `ch04-electricity` | Electronics basics | Supply voltages, resistor codes, LED resistor sizing and current measurement (notes only) |
| `ch05-gpio-out` | GPIO output | LED via the GPIO driver and via direct register writes (W1TS/W1TC) |
| `ch06-button` | GPIO input | Pull-ups, contact bounce measured with a logic analyzer, debouncing |
| `ch07-tasks` | FreeRTOS | Tasks, priorities, relative vs absolute delays, watchdog and stack sizing |
| `ch08-input` | Interrupts and queues | ISR-driven, debounced input events for 8 buttons |
| `ch11-spi` | SPI | First SPI transfers to an ILI9341, decoded on the logic analyzer |
| `ch20-adc` | Analog input | Calibrated ADC readings, noise filtering, hysteresis |
| `ch21-i2c` | I2C | BME280 driver written from the datasheet, including compensation formulas |
| `ch22-mosfet` | Power switching | Logic-level MOSFET low-side switch, measured R<sub>DS(on)</sub>, PWM |
| `ch23-encoder` | Hardware counting | Quadrature decoding with the PCNT peripheral |
| `ch29-stm32-baremetal` | Bare-metal ARM | Own startup code, vector table, linker script, SysTick and UART on a Cortex-M4 |

## Hardware

- ESP32-S3-DevKitC-1-N32R16V (ESP32-S3-WROOM-2, 32 MB flash, 16 MB PSRAM)
- STM32 NUCLEO-F446RE
- Breadboard parts: ILI9341 TFT, BME280, rotary encoder, potentiometers, IRLZ44N MOSFETs, LEDs, buttons
- Tools: multimeter, 8-channel logic analyzer (PulseView)

## Building a lab

ESP32 labs (ESP-IDF v6.x):

```sh
cd ch06-button
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
```

The STM32 lab has its own Makefile; see its folder.