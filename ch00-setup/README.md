# ch00-setup - Software setup on Arch

Installing ESP-IDF, USB permissions, VS Code and PulseView, then proving the toolchain and hardware paths work.
No code.

## Goal

Get a working ESP-IDF toolchain on Arch, give my user access to the board's serial and JTAG ports, and confirm
both the DevKit and the logic analyzer are reachable from the PC.

## What I did

Installed everything as described in [NOTES.md](../NOTES.md#setting-up-the-software-arch) (task 0.1):

- Base build packages with pacman
- EIM (ESP-IDF Installation Manager) from Espressif's pacman repository
- ESP-IDF v6.1 with `eim install`, plus a `get_idf` alias for the activation script
- Added my user to `uucp` for serial access, and Espressif's OpenOCD udev rule for JTAG access
- VS Code with the ESP-IDF extension
- PulseView and the fx2lafw firmware for the logic analyzer

## Results

### Toolchain (task 0.2)

```
$ get_idf
You are now using IDF version 6.1.0.
$ idf.py --version
ESP-IDF v6.1
$ xtensa-esp32s3-elf-gcc --version
xtensa-esp-elf-gcc (crosstool-NG esp-15.2.0_20251204) 15.2.0
```

`xtensa-esp32s3-elf-gcc` isn't found in a terminal without `get_idf`, because the cross-compiler is part of
ESP-IDF's own tools in my home directory, and only the activation script puts it on `PATH`.

### Hardware paths (task 0.3)

- `cat /dev/ttyACM0` works without sudo and printed output from the board:
  `W (3372) i2c_scan: No devices found`
- PulseView finds the analyzer (Saleae Logic, 8 channels) with the fx2lafw driver. A capture with nothing
  connected gives eight flat lines.
