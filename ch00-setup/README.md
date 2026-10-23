# ch00-setup - Software setup on Arch

Installing ESP-IDF and the tools, and checking that the board and logic analyzer work.

## Goal

Set up ESP-IDF on Arch with access to the board's serial and JTAG ports, and check that the DevKit and the logic
analyzer show up on the PC.

## What I did

Followed the steps in [NOTES.md](../NOTES.md#setting-up-the-software-arch):

- Installed EIM from Espressif's pacman repository and ESP-IDF v6.1 with it
- Added a get_idf alias for the activation script
- Added my user to the uucp group for serial access, and the OpenOCD udev rule for JTAG
- Installed VS Code with the ESP-IDF extension, and PulseView for the logic analyzer

## Results

Toolchain:

```
$ idf.py --version
ESP-IDF v6.1
$ xtensa-esp32s3-elf-gcc --version
xtensa-esp-elf-gcc (crosstool-NG esp-15.2.0_20251204) 15.2.0
```

The compiler is only found after running get_idf, because the activation script is what adds ESP-IDF's tools to
PATH.

Hardware:

- /dev/ttyACM0 can be read without sudo and shows output from the board
- PulseView finds the analyzer with the fx2lafw driver, and an empty capture shows eight flat lines
