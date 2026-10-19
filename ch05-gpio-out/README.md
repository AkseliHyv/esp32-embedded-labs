# ch05-gpio-out - GPIO output: your own LED

Driving an external LED from a GPIO, first through the ESP-IDF GPIO driver and then by writing registers directly.

## Goal

Configure a GPIO as an output with the driver, use ESP-IDF logging and error checking, and then drive the same
pin through the W1TS/W1TC registers from the Technical Reference Manual.

## Wiring

| From | To | Notes |
|------|----|-------|
| GPIO 18 | 330 ohm resistor | Same LED circuit as ch04, fed from a GPIO instead of 3V3 |
| Resistor | LED anode (long leg) | |
| LED cathode (short leg) | GND | |

## What I did

- Copied `hello_world` as a template, renamed the source file to `gpio_blink_main.c` and updated
  `main/CMakeLists.txt` to match. The GPIO driver needs `PRIV_REQUIRES esp_driver_gpio`.
- Configured the pin with `gpio_config()`:
    - `pin_bit_mask = 1ULL << LED_PIN`: one bit per GPIO. `1ULL` makes the shift 64-bit, since shifting a
      32-bit `1` by 32 or more is undefined behaviour.
    - Output mode, no pull-up or pull-down, no interrupt
    - Wrapped in `ESP_ERROR_CHECK()`, which aborts with the error name, file and line if the call fails
- **Blink (task 5.2):** `blink_loop()` toggles the LED every 500 ms (1 Hz) and logs each change with
  `ESP_LOGI`.
- **Morse code (task 5.3):** `morse()` blinks any string of letters, digits and spaces.
    - The Morse table is an array of 128 strings indexed by the ASCII character, filled with designated
      initializers (`['A'] = ".-"`). Characters not in the table are `NULL`.
    - `char_to_morse()` returns `NULL` for anything outside the table, and `morse()` treats that as a space.
      Lowercase works through `toupper()`.
    - Timing, with one unit = 200 ms:

        | Element | Units | How |
        | --- | --- | --- |
        | Dot | 1 | LED on 200 ms |
        | Dash | 3 | LED on 600 ms |
        | Gap inside a letter | 1 | LED off 200 ms after every symbol |
        | Gap between letters | 3 | 200 ms after the last symbol + 400 ms |
        | Gap between words | 7 | Letter gap (600 ms) + 800 ms for the space |
- **Bypassing the driver (task 5.4):** kept `gpio_config()`, since it also sets up the pin's routing and
  output enable, and replaced `gpio_set_level()` in `morse()` with direct register writes:
    ```c
    REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);  // LED on
    REG_WRITE(GPIO_OUT_W1TC_REG, 1ULL << LED_PIN);  // LED off
    ```
    - `REG_WRITE(address, value)` comes from `soc/soc.h`, which `soc/gpio_reg.h` includes
    - The address picks the register; the value picks the pins, with bit n = GPIO n

## Results

Register addresses (task 5.4). The TRM register summary lists offsets relative to the GPIO base address, which
is in the peripheral address table in the System and Memory chapter.

| Register | GPIO base | Offset | Base + offset | Printed macro value |
| --- | --- | --- | --- | --- |
| `GPIO_OUT_W1TS_REG` | 0x60004000 | 0x0008 | 0x60004008 | 0x60004008 |
| `GPIO_OUT_W1TC_REG` | 0x60004000 | 0x000C | 0x6000400C | 0x6000400c |

Both match. The register version blinks the same Morse as the driver version.

Loaded pin (task 5.5), LED held on, GPIO 18 measured against GND:

| Load | Current | GPIO 18 voltage |
| --- | --- | --- |
| No LED | 0 mA | 3.3 V |
| Bigger resistor + red LED | 2.7 mA | 3.3 V |
| 330 ohm + red LED | 3.6 mA | 3.13 V |

The high level isn't exactly 3.3 V under load. The output transistor that connects the pin to 3.3 V has a
small resistance of its own, in series with the resistor and LED, so current through it drops some voltage
inside the chip. No current means no drop; more current means a bigger drop. With the bigger resistor the
drop was too small for the meter to show.

**Why write-1-to-set/clear registers exist:** with a single output register, changing one pin means reading
the register, changing one bit and writing it back. If an interrupt changes another pin between the read and
the write, that change gets overwritten. With W1TS/W1TC, each change is a single write that only touches the
bits that are 1, so it can't clobber other pins.

## Problems and fixes

- **Every Morse symbol came out as a dash.** The check was `if (c == '.')`, where `c` is the loop index, not
  the character, so it never matched and every symbol took the dash branch. Fix: `if (morse[c] == '.')`.
- **Getting the register write right took several attempts.** The mistakes, in order:
    - Used offset 0x0014 instead of 0x0008/0x000C, and the same register for on and off. Off has to go to
      the clear register (W1TC), on to the set register (W1TS).
    - Wrote `1ULL < LED_PIN` instead of `1ULL << LED_PIN`. `<` is a comparison, so the value was 1, which is
      GPIO 0.
    - Used the TRM offsets (0x0008, 0x000C) as the address. The TRM's "Address" column is relative to the
      GPIO base address, so this wrote to near address 0 and would crash.
    - Moved the pin into the address (`0x0008 << LED_PIN`) and wrote `1`/`0` as the value. The address
      only selects the register, the value selects the pins, and writing 0 to a write-1-to-clear register
      does nothing.
    - Subtracted the pin mask from the macro (`GPIO_OUT_W1TS_REG - (1ULL << LED_PIN)`). The macro is already
      the full address (`DR_REG_GPIO_BASE + 0x8`), so anything added or subtracted moves it off the register.
  - Lesson: register address = peripheral base + offset; the macros already contain that sum. Pin selection
    always goes in the value.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
