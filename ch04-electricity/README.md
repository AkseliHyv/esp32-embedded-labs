# ch04-electricity - The electricity you actually need

Breadboard layout, supply voltages, resistor colour codes and an LED circuit, measured with the multimeter.

## Goal

Get comfortable with the multimeter before driving anything from a GPIO.

## Breadboard

- The power rails run the whole length of the board
- The top and bottom + rails are separate

## Board supplies

Measured against GND, powered from USB:

| Pin | Measured |
| --- | --- |
| 3V3 | 3.3 V |
| 5V | 4.69 V |

The 5V pin is fed from USB through a protection diode, which drops a few tenths of a volt.

## Resistors

| Bands | Nominal | Tolerance | Measured |
| --- | --- | --- | --- |
| brown black green gold | 1 Mohm | 5% | 1 Mohm |
| orange orange red gold | 3.3 kohm | 5% | 3.2 kohm |
| brown black black gold | 10 ohm | 5% | 10 ohm |

All three are within tolerance.

## LED

Circuit: 3V3 -> 330 ohm resistor -> red LED -> GND

- For 5 mA with Vf = 2.0 V: (3.3 - 2.0) / 0.005 = 260 ohm. I used 330 ohm, the next value up in the kit.
- Voltage across the LED: 1.85 V
- Voltage across the resistor: 1.39 V
- Together 3.24 V, which is close to the supply
- Calculated current: (3.3 - 1.85) / 330 = 4.3 mA
- Measured current: 4.6 mA
- Blue and white LEDs drew less current. Their forward voltage is about 3 V, so only about 0.3 V is left across
  the resistor.
