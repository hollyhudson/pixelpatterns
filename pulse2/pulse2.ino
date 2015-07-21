/** Ramp all the pixels up and then back down.
 */
#include <Adafruit_NeoPixel.h>

#define led 11 
#define NEOPIN 0 
//#define PIN 10
#define NUM_PIXELS 2 
//#define NUM_PIXELS 25 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,  // how many pixels on strip?
	NEOPIN,  // output pin?
	NEO_GRB + NEO_KHZ800  // type of pixels?
);

int brightness = 0;
int direction = 1;

void setup() {
	pixels.begin();
	pixels.setPixelColor(0, 100, 100, 100);	
	pixels.show();
  pinMode(led, OUTPUT);     
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
	/******
	for(int i = 0 ; i < NUM_PIXELS ; i++)
		pixels.setPixelColor(i, 0, 0, brightness);

	// Make flashes; 10% the time, turn on pixel on full brightness
	if (random(10) == 0)
		pixels.setPixelColor(random(0, NUM_PIXELS), 255, 255, 255);

	pixels.show();
	delay(10);

	brightness = brightness + direction; // +=

	if (brightness == 200)
		direction = -1;
	else
	if (brightness == 0)
		direction = +1;
	******/
