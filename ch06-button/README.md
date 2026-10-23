# ch06-button - GPIO input: a button and contact bounce

Reading a button, measuring contact bounce with the logic analyzer, and debouncing it.

## Goal

Read a button with the internal pull-up, measure its bounce, and write a debounce based on the measurement.

## Wiring

| From | To |
|------|----|
| GPIO 4 | Button, one side |
| Button, other side | GND |
| GPIO 18 | 330 ohm resistor -> LED -> GND |

## What I did

- Set GPIO 4 as an input with the internal pull-up, and GPIO 18 as an output
- Read the button from GPIO_IN_REG, which holds the input levels of GPIO 0 to 31
- Polled every 10 ms. The LED is on while the button is held, and each press and release is logged.
- Changed the polling to 1 ms and added a press counter
- Captured presses and releases in PulseView at 24 MHz with 200M samples
- Wrote a debounce function that is called every 1 ms with the raw reading. The state only changes after the
  reading has stayed the same for 5 ms: the longest bounce measured was 3 ms, plus 2 ms margin.

## Results

- 20 presses at 1 ms polling counted 20
- The main switch did not bounce at all on presses or releases
- Some worse switches bounced for 300 us to 3 ms

## Problems and fixes

- Pressed and released were the wrong way round. With a pull-up the pin reads 1 when released and 0 when
  pressed.
- Nothing worked at first. The configs were never passed to gpio_config, and the LED writes went to the
  registers for GPIO 32 to 48.
- The watchdog fired at 1 ms polling. At the default tick rate of 100 Hz, 1 ms rounds down to 0 ticks, so the
  task never paused and the idle task never ran. Fixed by setting the tick rate to 1000 Hz.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
