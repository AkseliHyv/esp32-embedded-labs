# ch05-gpio-out - GPIO output: your own LED

Driving an LED from a GPIO, first with the GPIO driver and then by writing registers directly.

## Goal

Use the GPIO driver with logging and error checking, then drive the same pin through the W1TS and W1TC registers.

## Wiring

| From | To |
|------|----|
| GPIO 18 | 330 ohm resistor |
| Resistor | LED anode (long leg) |
| LED cathode (short leg) | GND |

## What I did

- Made the project from hello_world. The GPIO driver needs esp_driver_gpio in PRIV_REQUIRES.
- Set GPIO 18 as an output with gpio_config, wrapped in ESP_ERROR_CHECK
- Blinked the LED at 1 Hz and logged each change
- Wrote a function that blinks any message in Morse code. The Morse table is an array of strings indexed by
  the character. One unit is 200 ms:

    | Element | Units |
    | --- | --- |
    | Dot | 1 |
    | Dash | 3 |
    | Gap inside a letter | 1 |
    | Gap between letters | 3 |
    | Gap between words | 7 |

- Replaced gpio_set_level with direct register writes:
    ```c
    REG_WRITE(GPIO_OUT_W1TS_REG, 1ULL << LED_PIN);  // LED on
    REG_WRITE(GPIO_OUT_W1TC_REG, 1ULL << LED_PIN);  // LED off
    ```
    The address picks the register, and each bit in the value is one pin.

## Results

Register addresses. The TRM lists offsets from the GPIO base address, which is in the System and Memory chapter.

| Register | Base | Offset | Base + offset | Printed |
| --- | --- | --- | --- | --- |
| GPIO_OUT_W1TS_REG | 0x60004000 | 0x0008 | 0x60004008 | 0x60004008 |
| GPIO_OUT_W1TC_REG | 0x60004000 | 0x000C | 0x6000400C | 0x6000400c |

Pin voltage under load, LED on:

| Load | Current | GPIO 18 |
| --- | --- | --- |
| No LED | 0 mA | 3.3 V |
| Bigger resistor | 2.7 mA | 3.3 V |
| 330 ohm | 3.6 mA | 3.13 V |

The transistor inside the pin has a small resistance, so the voltage drops as more current flows.

## Problems and fixes

- Every Morse symbol came out as a dash. The code compared the loop index to '.' instead of the character.
- The register writes took several tries:
    - I used the TRM offsets as addresses. They are relative to the GPIO base address.
    - I put the pin into the address. The pin belongs in the value.
    - I subtracted the pin mask from the macro. The macro is already the full address.

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
# Exit the monitor with Ctrl+]
```
