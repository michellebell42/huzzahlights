# Feather Huzzah ESP8266 LED Controller with Adafruit IO

This code is designed to run on the Adafruit Feather Huzzah ESP8266 board.

## Instructions
After downloading, rename the config_template.h to config.h and edit the first four #define lines to your personal Adafruit IO username/key and your WiFi SSID and password.

Open huzzahlights.ino in the Arduino IDE. Edit #define PIXEL_COUNT to the qty of NeoPixel LEDs attached to the huzzah board (example code has 240 LEDs). 

Run code and use Adafruit IO to change LED colors & change modes.

## Adafruit IO Setup
If you don't have an account, create one at https://io.adafruit.com/. Use this username and key to populate config.h. Key can be found under "My Key".

Create a new Feed titled "ledfeed". If you use a different name, this must be updated in huzzahlights.ino. Feed name must match code: io.feed("ledfeed");

Optional: create a dashboard and add the Color Picker block. Attach it to your ledfeed Feed to send the selected color to your LEDs.

## Use
LEDs are controlled entirely through the Adafruit IO ledfeed Feed. Control options:

1. Hex color: use dashboard color picker or manually enter a hex color into the feed (i.e. #ff0000 for red) 
2. Hard-coded colors/modes: enter any of the following into the feed: red, blue, green, party
3. Mode commands: enter any of the following into the feed (m stands for Mode): mparty, mchristmas, mchristmasstatic
