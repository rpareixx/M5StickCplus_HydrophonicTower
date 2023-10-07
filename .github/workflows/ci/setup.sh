#!/bin/bash  

arduino-cli core update-index
arduino-cli core install esp32:esp32@1.0.6
arduino-cli lib install ArduinoJson@6.19.4 AnotherPackage@x.y.x