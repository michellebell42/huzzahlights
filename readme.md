# Feather Huzzah ESP8266 LED Controller with Adafruit IO

This code is designed to run on the Adafruit Feather Huzzah ESP8266 board.

## Instructions
After downloading, rename the config_template.h to config.h and edit the first four #define lines to your personal Adafruit IO username/key and your WiFi SSID and password.

Open huzzahlights.ino in the Arduino IDE. Edit #define PIXEL_COUNT to the qty of NeoPixel LEDs attached to the huzzah board (example code has 240 LEDs). 

Run code and use Adafruit IO to change LED colors & change modes.
