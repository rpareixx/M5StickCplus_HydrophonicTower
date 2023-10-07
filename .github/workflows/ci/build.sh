#!/usr/bin/env sh

ESP_VERSION=${ESP_VERSION:-1.0.6}
SPIFFS_VERSION=${SPIFFS_VERSION:-0.2.3}
ARDUINO_DIR=`arduino-cli config dump --format json | jq -r .directories.data`

# Compile firmware
# → My sketch is named `arduino`, not very original
# → Surprisingly you need --export-binaries otherwise it does nothing
# → My custom libraries are submodules in "arduino/libraries"
arduino-cli compile --verbose \
  --fqbn esp32:esp32:esp32 \
  --libraries arduino/libraries \
  --export-binaries \
  arduino

# Generate the spiff
# → "arduino/data" is where my web assets are
# → I chose to put the binary alongside the binaries arduino-cli built
# → Lots of magic numbers here, more below
${ARDUINO_DIR}/packages/esp32/tools/mkspiffs/${SPIFFS_VERSION}/mkspiffs \
  -c arduino/data \
  -b 4096 \
  -p 256 \
  -s 0x170000 \
  arduino/build/esp32.esp32.esp32/arduino.ino.spiffs.bin