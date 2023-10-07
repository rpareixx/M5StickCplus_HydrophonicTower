#!/bin/bash  

ESP_VERSION=${ESP_VERSION:-1.0.6}
SPIFFS_VERSION=${SPIFFS_VERSION:-0.2.3}
ESPTOOL_VERSION=${ESPTOOL_VERSION:-3.0.0}
ARDUINO_DIR=`arduino-cli config dump --format json | jq -r .directories.data`

BOOTLOADER="${ARDUINO_DIR}/packages/esp32/hardware/esp32/${ESP_VERSION}/tools/sdk/bin/bootloader_dio_80m.bin"

# You can flash the app, bootloader and partions with arduino-cli,
# but I preferred a single command to do it all
# arduino-cli upload -p "/dev/cu.usb..." --fqbn esp32:esp32:esp32 arduino

$ARDUINO_DIR/packages/esp32/tools/esptool_py/${ESPTOOL_VERSION}/esptool \
  --chip esp32 \
  --before default_reset \
  --after hard_reset \
  write_flash -z \
    0x1000 $BOOTLOADER \
    0x8000 arduino/build/esp32.esp32.esp32/arduino.ino.partitions.bin \
    0x10000 arduino/build/esp32.esp32.esp32/arduino.ino.bin \
    0x290000 arduino/build/esp32.esp32.esp32/arduino.ino.spiffs.bin