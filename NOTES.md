## Chapter 0
### 0.1 Base packages
- installed base packages for EIM with
    ```
    sudo pacman -S --needed git cmake ninja python python-pip gcc make \
    flex bison gperf ccache dfu-util libusb wget curl
    ```

### 0.2 Install EIM from Espressif's pacman repository
- Trusted Espressif's key with
    ```
    curl -fsSL https://dl.espressif.com/dl/eim/eim.asc | sudo pacman-key --add -sudo pacman-key --lsign-key 06E9A6C0325E124198C685F18B1F38BDC9383E1D
    ```
- Added the repository to pacman.conf
    ```
    sudo tee -a /etc/pacman.conf << 'EOF'
    [eim]
    SigLevel = Required DatabaseOptional TrustAll
    Server = https://dl.espressif.com/dl/eim/pacman/$arch
    EOF
    ```
- Synced and installed EIM
    ```
    sudo pacman -Syu
    sudo pacman -S eim-cli
    ```

### 0.3 Install ESP-IDF
- Installed latest version of ESP-IDF
    ```
    eim install
    ```
- Added ESP-IDF activation script to aliases (~/.bashrc)
    ```
    alias get_idf='source /home/akseli/.espressif/tools/activate_idf_v6.1.sh'
    ```

### 0.4 USB permissions
- Added myself to group uucp for serial device access
    ```
    sudo usermod -aG uucp $USER
    ```
- Got the location for the udev rules file EIM installed
    ```
    find ~ -name 60-openocd.rules 2>/dev/null
    ```
- Added Espressif's OpenOCD udev rule for JTAG access
    ```
    sudo cp /home/akseli/.espressif/tools/openocd-esp32/v0.12.0-esp32-20260703/openocd-esp32/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d/
    ```
- Reloaded the rules
    ```
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    ```

### 0.5 VS Code setup
- Installed the ESP-IDF extension from VS Code Extension Marketplace

### 0.6 Logic analyzer setup
- Added GUI and firmware for logic analyzer
    ```
    sudo pacman -S pulseview sigrok-firmware-fx2lafw
    ```

### Tasks
- Task 0.1: Install everything
    - Done
- Task 0.2: Prove the toolchain works
    ```
    [akseli@arch ~]$ get_idf
    ...
    You are now using IDF version 6.1.0.
    eim select v6.1
    (venv) [akseli@arch ~]$ idf.py --version
    ESP-IDF v6.1
    (venv) [akseli@arch ~]$ xtensa-esp32s3-elf-gcc --version
    xtensa-esp-elf-gcc (crosstool-NG esp-15.2.0_20251204) 15.2.0
    ```
    - The reason why xtensa-esp32s3-elf-gcc isn't found in a terminal without get-idf is because the gcc is from ESP-IDF, which requires activating it first.
- Task 0.3: Prove the hardware paths work
    ```
    (venv) [akseli@arch ~]$ cat /dev/ttyACM0
    W (3372) i2c_scan: No devices found
    ```
    - PulseView finds Saleae Logic with 8 channels when searching with driver fx2lafw. It gets 8 straight lines from the analyzer.