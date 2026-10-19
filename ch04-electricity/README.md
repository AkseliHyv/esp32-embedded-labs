# ch04-electricity - The electricity you actually need

Breadboard layout, supply voltages, resistor colour codes and an LED circuit, measured with the multimeter. No code.

## Goal

Get comfortable with Ohm's law and the multimeter (voltage, resistance, continuity, current) before driving
anything from a GPIO.

## Breadboard (task 4.1)

- Power rails are continuous along the whole length (no split in the middle)
- The top and bottom + rails are not connected to each other

## Board supplies (task 4.2)

DevKit powered from USB, measured against GND:

| Pin | Measured |
| --- | --- |
| 3V3 | 3.3 V |
| 5V | 4.69 V |

The 5V pin isn't 5.00 V because it is fed from USB through a protection diode, which drops a few tenths of a volt.

## Resistors (task 4.3)

Picked three resistors instead of five.

| Bands | Nominal | Tolerance | Measured | Within tolerance |
| --- | --- | --- | --- | --- |
| brown black green gold | 1 Mohm | +-5% | 1 Mohm | yes |
| orange orange red gold | 3.3 kohm | +-5% (+-165 ohm) | 3.2 kohm | yes |
| brown black black gold | 10 ohm | +-5% | 10 ohm | yes |

## LED without code (task 4.4)

Circuit: 3V3 -> 330 ohm resistor -> LED anode, LED cathode -> GND.

- Resistor for a red LED at ~5 mA, assuming Vf = 2.0 V: (3.3 V - 2.0 V) / 0.005 A = 260 ohm.
  Used 330 ohm, the next value up in the kit, to stay below the target current.
- Measured with the red LED:
    - Across the LED: 1.85 V
    - Across the resistor: 1.39 V
    - Sum: 3.24 V, close to the 3.3 V supply. The rest is rounding and resistance in the wires.
- Current:
    - Calculated: (3.3 V - 1.85 V) / 330 ohm = 4.3 mA
    - Measured with the meter in series: 4.6 mA, within the resistor's tolerance and rounding
- Blue and white LEDs: the current dropped. Their forward voltage is about 3 V, so only ~0.3 V is left across
  the resistor, and I = V / R gives a much smaller current.

## Problems and fixes

- **No reading and the LED went off with the meter in series.** The meter wasn't set up for current, so it
  broke the circuit instead of conducting. With the red lead in the mA jack and the dial on DC mA, it read
  4.6 mA.
