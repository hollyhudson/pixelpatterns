/**
 * 24-pixel ring on my backpack.
 * Cycles through a few patterns.
 */
#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 7
#define PIN 0

#define BUTTON 17 // F6
#define BUTTON2 18 // F5

// Parameter 1 = number of pixels in pixels
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
	pinMode(BUTTON2, OUTPUT); // Make it ground
	digitalWrite(BUTTON2, 0); // Make it ground
}

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
uint8_t pos = 0;
uint8_t skip = 0;

uint8_t brightness = 0;
int direction = 1;
int position = 0;
static uint8_t last_read;

int brightnesses[5] = {0,0,0,0,0};
int directions[5] = {1,1,1,1,1};
int color[5] = {0,0,0,0,0};
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

static const uint8_t palette[][4] PROGMEM =
{
#if 0
	{ 128,   0,   0 },
	{ 128, 128,   0 },
	{   0, 128,   0 },
	{   0, 128, 128 },
	{ 128, 128, 128 },
	{ 128,   0, 128 },
#endif

#if 0
	{  50,   0,   0 },
	{ 100,   0,   0 },
	{   0,  50,   0 },
	{   0, 100,   0 },
	{   0,   0,  50 },
	{   0,   0, 100 },
#endif

#if 1
	//http://www.colourlovers.com/palette/1473/Ocean_Five
	{ 0, 160, 176 },
	{ 0, 160, 176 },
	{ 0, 160, 176 },
	{ 106, 74, 60 },
	{ 106, 74, 60 },
	{ 106, 74, 60 },
	{ 204, 51, 63 },
	{ 204, 51, 63 },
	{ 204, 51, 63 },
	{ 235, 104, 65 },
	{ 235, 104, 65 },
	{ 235, 104, 65 },
	{ 237, 201, 81 },
	{ 237, 201, 81 },
	{ 237, 201, 81 },
#endif

#if 0
	// http://www.colourlovers.com/palette/3419536/Hot_Pink_Print_Short
	{ 226, 31, 128 },
	{ 73, 1, 51 },
	{ 214, 140, 204 },
	{ 227, 252, 160 },
	{ 71, 219, 253 },
#endif
};

static void
spin()
{
	// based on chase2

	// turn off all the pixels
	for(int i = 0 ; i < NUM_PIXELS ; i++)
		pixels.setPixelColor(i, 0, 0, 0);

	// and now turn on the one at position and the next few
	pixels.setPixelColor((position+0) % NUM_PIXELS, 128, 0, 130);
	pixels.setPixelColor((position+1) % NUM_PIXELS, 64, 0, 66);
	pixels.setPixelColor((position+2) % NUM_PIXELS, 32, 0, 34);
	pixels.setPixelColor((position+3) % NUM_PIXELS, 16, 0, 18);
	pixels.setPixelColor((position+4) % NUM_PIXELS, 8, 0, 10);
	pixels.setPixelColor((position+5) % NUM_PIXELS, 4, 0, 6);
	pixels.setPixelColor((position+6) % NUM_PIXELS, 2, 0, 4);

	// go in the opposite direction; note that modular arithmetic
	// does not work right with negative numbers, so add in the modulus
	position = (position + NUM_PIXELS - 1) % NUM_PIXELS;

	pixels.show();
	delay(50);
}

static void
pulse()
{
	// based on pulse2
	for(int i = 0 ; i < NUM_PIXELS ; i++)
		pixels.setPixelColor(i, brightness/25, brightness/10, brightness);

	// Make flashes; 10% the time, turn on pixel on full brightness
	if (random(80) == 0)
		pixels.setPixelColor(random(0, NUM_PIXELS), 255, 255, 255);

	pixels.show();
	if (direction == 1) 
		delay(2);
	else
		delay(6);


	brightness = brightness + direction; // +=

	if (brightness == 200)
		direction = -1;
	else
	if (brightness == 0)
		direction = +1;
}

static void
pornjpulse()
{
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
			brightnesses[i] = brightnesses[i] + directions[i];
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
				r = brightnesses[i] * 250 / 256;
				g = brightnesses[i] * 80 / 256;
				b = brightnesses[i] * 0 / 256;
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

static void
blend(
	unsigned i,
	int smooth,
	int red,
	int green,
	int blue
)
{
	uint32_t c = pixels.getPixelColor(i);
	int r = (c >> 16) & 0xFF;
	int g = (c >>  8) & 0xFF;
	int b = (c >>  0) & 0xFF;

	r = (r * smooth + red) / (smooth+1);
	g = (g * smooth + green) / (smooth+1);
	b = (b * smooth + blue) / (smooth+1);

	pixels.setPixelColor(i, r, g, b);
}


static void
sparkle(
	int chance
)
{
	if (random(chance) == 0)
		pixels.setPixelColor(random(NUM_PIXELS), 255, 255, 255);
}


void circle_pattern(void)
{
	for (int i = 0 ; i < NUM_PIXELS ; i++)
	{
		blend(i, 30, red, green, blue);
	}

	blend(pos, 30, 128, 128, 128);

	red = (red + 1) % 64;
	//green++;

	if (++skip >= 8)
	{
		skip = 0;
		if (++pos == NUM_PIXELS)
			pos = 0;
	}

	pixels.show();
	delay(10);
}


static const uint32_t colors[] =
{
	0x400000,
	0x400000,
	0x402000,
	0x402000,
	0x400020,
	0x400020,
};

void multichase_pattern(void)
{
	red = 64;
	blue = 0;
	green = 0;

	for (int i = 0 ; i < NUM_PIXELS ; i++)
	{
		if (i % 6 == pos)
			blend(i, 8, 64, 20, 0);
		else
			blend(i, 100, 0, 0, 0);
	}

	if (++skip >= 16)
	{
		pos = (pos + 1) % 6;
		skip = 0;
	}

	sparkle(400);

	pixels.show();
	delay(1);
}


static unsigned start_pixel = 0;

void pulse_pattern(void)
{
	red = 64;
	blue = 0;
	green = 0;

	++skip;

	if ((0 <= skip && skip < 48) || (96 <= skip && skip < 96+48))
	{
		unsigned phase = skip % 48;
		if (phase == 0)
			start_pixel = random(NUM_PIXELS);

		if (phase > NUM_PIXELS)
			phase = NUM_PIXELS;

		for (int i = 0 ; i < phase ; i++)
			blend(
				(i+start_pixel) % NUM_PIXELS,
				8,
				red, green, blue
			);
	} else
	{
		if (skip > 300)
			skip = 0;
		for (int i = 0 ; i < NUM_PIXELS ; i++)
			blend(i, 30, 0, 0, 0);
	}

	sparkle(200);

	pixels.show();
	delay(1);
}


typedef void (*function_t)(void);

static function_t patterns[] = {
	pornjpulse,
	spin,
	pulse,
};

static const unsigned num_patterns = sizeof(patterns)/sizeof(*patterns);

void
loop()
{
	for (int i = 0 ; i < num_patterns; i++)
	{
		function_t pattern = patterns[i];
	
		while(1)
		{

			if (digitalRead(BUTTON) == 0)
			{
				if (last_read == 0)
				{
					last_read = 1;
					//brightness = (brightness + 64) % 256;
					//pixels.setBrightness(brightness);
					break;
				}
			} else {
				last_read = 0;
			}

			pattern();
		}
	}
}
