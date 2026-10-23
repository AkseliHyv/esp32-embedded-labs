# ch03-blink - The on-board RGB LED: using a driver someone else wrote

Driving the DevKit's RGB LED with the led_strip component and checking the signal with the logic analyzer.

## Goal

Use a library through the component manager, read its source, and check its output on the logic analyzer.

## Wiring

| From | To |
|------|----|
| GPIO 38 (LED data) | Logic analyzer D0 |
| GND | Logic analyzer GND |

## What I did

- Copied ESP-IDF's blink example and added my module settings to its sdkconfig.defaults
- In menuconfig, chose the LED strip type with the RMT backend on GPIO 38
- The example gets the led_strip driver through main/idf_component.yml
- Changed the program to step through six dim colours
- Captured the LED data line in PulseView at 24 MHz

## Reading the driver

- Bit timings for each LED model are in led_strip_rmt_dev.c. For the WS2812:
    - 0 bit: 300 ns high, 900 ns low
    - 1 bit: 900 ns high, 600 ns low
- Setting a pixel checks the arguments and writes the colour bytes into a buffer in RAM, in G, R, B order. This
  runs on the CPU.
- Refreshing starts an RMT transmission and waits for it to finish. The CPU converts the bytes into pulse
  lengths, and the RMT hardware sends the pulses with exact timing.

## Results

- Short pulse: 300 ns. Long pulse: 1000 ns. The driver uses 300 ns and 900 ns, and at 24 MHz one sample is
  about 42 ns.
- One update while the LED was magenta: 00000000 00000011 00000011, which is G = 0, R = 3, B = 3

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
