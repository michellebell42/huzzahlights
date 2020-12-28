// Adafruit IO RGB LED Output Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

#include "Adafruit_NeoPixel.h"

#define PIXEL_PIN     2
#define PIXEL_COUNT   240
#define PIXEL_TYPE    NEO_GRB + NEO_KHZ800

#define BOARD_LED_PIN 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// set up the twitch feed
AdafruitIO_Feed *ledFeed = io.feed("ledfeed");
long color = strip.Color(255, 0, 255);
long colorB;

int brightness = 220;

int pinlvl = 0;
String strData;

void setup() {

  pinMode(BOARD_LED_PIN, OUTPUT);
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'color' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  ledFeed->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  ledFeed->get();

  // neopixel init
  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

}



// Some functions of our own for creating animated effects -----------------

// set whole strand with no delay
void colorSet(int startPixel, int endPixel, uint32_t color) {
  for(int i=startPixel; i<endPixel; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    //strip.show();                          //  Update strip to match
  }
}

// set whole strand with no delay
void colorSetAll(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<90; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void partyMode(int wait){

}

void alternatingStatic(long colorA, long colorB, int block){
  
  int pixelCtr = 0;
  
  while (pixelCtr < (PIXEL_COUNT/(block*2))*(block*2) ){
    colorSet(pixelCtr, pixelCtr + block, colorA);
    colorSet(pixelCtr + block, pixelCtr + block*2, colorB);
    pixelCtr += (block*2);
  }
  int mod = PIXEL_COUNT % (block*2);
  if (mod < block) {
    colorSet(pixelCtr, pixelCtr + mod, colorA);
  } else {
    colorSet(pixelCtr, pixelCtr + block, colorB);
    colorSet(pixelCtr + block, pixelCtr + block + mod, colorB);
  }
  strip.show();
}

void alternatingAnimated(long colorA, long colorB, int block, int wait){
  for (int i = 0; i < 30; i++) {
    alternatingStatic(colorA, colorB, block);
    delay(wait);
    alternatingStatic(colorB, colorA, block);
    delay(wait);
  }
  
}

// this function is called whenever a 'color' message
// is received from Adafruit IO. it was attached to
// the color feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received data: ");
  strData = data->toString();
  Serial.println(strData);

  if(strData == "party"){
    Serial.println("party");
    pinlvl = LOW;
    theaterChaseRainbow(100); // Rainbow-enhanced theaterChase variant

    //switch back to last color automatically
    colorWipe(color, 50); // colorwipe
  } else if (strData.charAt(0) == '#'){
    color = data->toNeoPixel();

    colorWipe(color, 50); // colorwipe
  } else if (strData == "red") {
    colorWipe(strip.Color(255, 0, 0), 50); // colorwipe red
    
  } else if (strData == "green") {
    colorWipe(strip.Color(0, 255, 0), 50); // colorwipe green
    
  } else if (strData == "blue") {
    colorWipe(strip.Color(0, 0, 255), 50); // colorwipe blue
   
  } else if (strData == "off") {
    
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    strip.show();
  } else if (strData.charAt(0) == 'b'){
    // brightness command
    // if not blue and char at 0 = b
    strData = strData.substring(1); // cut off the b
    brightness = strData.toInt();
    strip.setBrightness(brightness);
    strip.show();
   } else if (strData.charAt(0) == 'm'){
    // modes command
    strData = strData.substring(1); // cut off the m
    if (strData == "party") {
      partyMode(100);
    } else if (strData == "christmas") {
      // alternate red x green 
      alternatingAnimated(strip.Color(255, 0, 0), strip.Color(0, 255, 0), 20, 325);
      //switch back to last color automatically
      colorWipe(color, 50); // colorwipe
    } else if (strData == "christmasstatic") {
      // alternate red x green 
      alternatingStatic(strip.Color(255, 0, 0), strip.Color(0, 255, 0), 20);

    
    } else if (strData == "gators"){
      // alternate orange x blue 
      alternatingAnimated(strip.Color(0, 0, 255), strip.Color(255, 60, 0), 20, 500);
      //switch back to last color automatically
      colorWipe(color, 50); // colorwipe
    }
  } else {
    Serial.println("HIGH");
    pinlvl = HIGH;
  }
    
  digitalWrite(BOARD_LED_PIN, pinlvl);



}
