# chXX-name - Topic

<!-- One line: what this lab is about. Example: "Reading a button reliably despite contact bounce." -->

## Goal

<!-- 1-2 sentences: what you set out to do or learn. -->

## Wiring

| From | To | Notes |
|------|----|-------|
| GPIO __ | | |
| GND | | |

<!-- Optional: wiring photo -->
<!-- ![Wiring](img/wiring.jpg) -->

## What I did

<!-- A few lines on the approach, not the full code. Example:
- Polled the pin every 1 ms
- Counter-based debounce: state changes only after 20 ms stable
-->

## Results

<!-- Measurements and observations. Numbers are the most valuable part. Example:
- Bounce lasted up to 1.2 ms on press, 0.3 ms on release (10 presses each)
- Chose 20 ms debounce time for margin
- 50/50 presses counted correctly
-->

<!-- Optional: logic analyzer capture -->
<!-- ![Capture](img/capture.png) -->

## Problems and fixes

<!-- Anything that went wrong and how you found the cause. Delete this section if nothing did. -->

## Build

```sh
idf.py set-target esp32s3
idf.py -p /dev/ttyACM0 build flash monitor
```