## Setting up the software (Arch)
### Base packages
- install base packages for EIM with
    ```
    sudo pacman -S --needed git cmake ninja python python-pip gcc make \
    flex bison gperf ccache dfu-util libusb wget curl
    ```

### Install EIM from Espressif's pacman repository
- Trust Espressif's key with
    ```
    curl -fsSL https://dl.espressif.com/dl/eim/eim.asc | sudo pacman-key --add -
    sudo pacman-key --lsign-key 06E9A6C0325E124198C685F18B1F38BDC9383E1D
    ```
- Add the repository to pacman.conf
    ```
    sudo tee -a /etc/pacman.conf << 'EOF'
    [eim]
    SigLevel = Required DatabaseOptional TrustAll
    Server = https://dl.espressif.com/dl/eim/pacman/$arch
    EOF
    ```
- Sync and install EIM
    ```
    sudo pacman -Syu
    sudo pacman -S eim-cli
    ```

### Install ESP-IDF
- Install latest version of ESP-IDF
    ```
    eim install
    ```
- Add ESP-IDF activation script to aliases (~/.bashrc)
    ```
    alias get_idf='source /home/username/.espressif/tools/activate_idf_v6.1.sh'
    ```

### USB permissions
- Add self to group uucp for serial device access
    ```
    sudo usermod -aG uucp $USER
    ```
- Get the location for the udev rules file EIM installed
    ```
    find ~ -name 60-openocd.rules 2>/dev/null
    ```
- Add Espressif's OpenOCD udev rule for JTAG access
    ```
    sudo cp /home/username/.espressif/tools/openocd-esp32/v0.12.0-esp32-20260703/openocd-esp32/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d/
    ```
- Reload the rules
    ```
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    ```

### VS Code setup
- Install the ESP-IDF extension from VS Code Extension Marketplace

### Logic analyzer setup
- Add GUI and firmware for logic analyzer
    ```
    sudo pacman -S pulseview sigrok-firmware-fx2lafw
    ```

## Building and flashing programs
- Build programs with
    ```
    idf.py build
    ```
    - Initial build takes longer due to ESP-IDF having to build all of the files from scratch.
    - Builds after are generally faster (if for example just configs are changed).
- Flash them to controller with
    ```
    idf.py -p /dev/ttyACM0 flash monitor
    ```
    - Exit the monitor with Ctrl+]
- Boot log from my ESP32-S3-DevKitC-1: see [ch02-hello](ch02-hello/README.md#results)
