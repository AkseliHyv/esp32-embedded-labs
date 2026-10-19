# ch06-button - GPIO input: a button and contact bounce

Reading a button reliably despite contact bounce, with the bounce measured on the logic analyzer.

## Goal

Read a tactile switch with the internal pull-up, see contact bounce on the logic analyzer, and write a debounce
based on my own measurements.

## Wiring

| From | To | Notes |
|------|----|-------|
| GPIO 4 | Button, one side | Across the breadboard's centre gap. GPIO 4 will be D-pad Up on the console |
| Button, other side | GND | |
| GPIO 18 | 330 ohm resistor -> LED -> GND | Same LED circuit as ch05 |

## What I did

- **Wire and poll (task 6.2):**
    - Configured GPIO 4 as an input with the internal pull-up, and GPIO 18 as an output, with `gpio_config()`
    - Read the button straight from the register instead of `gpio_get_level()`: `REG_READ(GPIO_IN_REG)` and a
      mask for bit 4. `GPIO_IN_REG` (offset 0x3C) holds the input level of GPIO 0 to 31.
    - Drove the LED with `GPIO_OUT_W1TS_REG` / `GPIO_OUT_W1TC_REG` like in ch05
    - Polls every 10 ms and compares with the previous reading, so it only reacts to changes: logs "Pressed
      button" and turns the LED on when the button goes down, logs "Released button" and turns it off when it
      comes up. No debouncing yet.
- **Catch the bounce (task 6.3):**
    - Changed the poll interval to 1 ms. At the default 100 Hz FreeRTOS tick rate, one tick is 10 ms, so
      `vTaskDelay(1 / portTICK_PERIOD_MS)` was `vTaskDelay(0)`. Set `CONFIG_FREERTOS_HZ=1000` so one tick is 1 ms.
    - Added a press counter to the log
    - Captured presses and releases in PulseView: GND first, channel 0 on GPIO 4, 24 MHz, 200M samples
      (~8.3 s per capture, ~42 ns resolution), trigger on falling edge for presses and rising for releases

## Results

- 20 presses at 1 ms polling: counter read 20, no over-counting
- Bounce on the main switch: none on presses or releases. Every edge was a single clean transition at
  ~42 ns resolution.
- Some of the worse switches did bounce, from 300 us up to 3 ms

## Problems and fixes

- **Pressed and released were the wrong way round.** With a pull-up, the button is active-low: released, the
  resistor pulls the pin to 3.3 V (reads 1); pressed, the switch connects it to GND (reads 0). The first
  version treated 1 as pressed. Fix: pressed = bit 4 is 0.
- **Nothing worked at first.** Both `gpio_config_t` structs were defined but never passed to `gpio_config()`,
  so the pull-up was off and the LED pin was never set as an output. The LED writes also went to
  `GPIO_OUT1_W1TS_REG`/`GPIO_OUT1_W1TC_REG`, which are for GPIO 32 to 48, not GPIO 18.
- **Task watchdog fired with a 1 ms poll.** At 100 Hz, `1 / portTICK_PERIOD_MS` is `1 / 10` = 0 in integer
  maths, so `vTaskDelay(0)` never blocked. The task ran non-stop, the lower-priority idle task never ran, and
  the watchdog (which checks that the idle task runs) fired. Fix: tick rate 1000 Hz, so the delay is 1 tick.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
