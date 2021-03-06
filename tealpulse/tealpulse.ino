/** Ramp all the pixels up and then back down.
 */
#include <Adafruit_NeoPixel.h>

#define PIN		0  
#define NUM_PIXELS 	3	

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,			// how many pixels on strip?
	PIN,				// output pin?
	NEO_GRB + NEO_KHZ800		// type of pixels?
);

int brightnesses[NUM_PIXELS];
int directions[NUM_PIXELS];
int color[NUM_PIXELS];
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;


void setup() {
	pixels.begin();
	pixels.show();
	Serial.begin(9600); // debug
}

void loop() {
	for(int i = 0 ; i < NUM_PIXELS ; i++)
	{
		if (pixels.getPixelColor(i) == 0x0 || brightnesses[i] == 0){
			// If pixel has gone down to black
			// set it to ramp up again
			directions[i] = +1;	
			brightnesses[i] = 1; // no endless loops
			// pick pink or orange
			if (random(2) == 0){
				// pink
				color[i] = 0;
				r = 10;
				g = 0;
				b = 3;
			} else {
				// orange
				color[i] = 1;
				r = 5;
				g = 10;
				b = 10;
			}
		} else {

			// Pixel is going up or down, not at zero
			brightnesses[i]	= brightnesses[i] + directions[i];
			Serial.println(i);
			Serial.println(brightnesses[i]);
			// change direction at some point
			if (brightnesses[i] == random(100,200) || brightnesses[i] == 200)
				directions[i] = -1;
			// set the r, g, and b based on new bright and dir
			if (color[i] == 0){
				// pink mode
				Serial.println("in pink");
				r = brightnesses[i];
				g = 0;
				b = brightnesses[i]/3;
			} else {
				// orange mode
				Serial.println("in orange");
				r = brightnesses[i];
				g = brightnesses[i]/5;
				b = brightnesses[i]/5;
			}
		
		}

		pixels.setPixelColor(i, r, g, b);

	}
	// Make flashes; 10% the time, turn on pixel on full brightness
	if (random(200) == 0)
		pixels.setPixelColor(random(0, NUM_PIXELS), 255, 255, 255);

	pixels.show();
	delay(10);

}
