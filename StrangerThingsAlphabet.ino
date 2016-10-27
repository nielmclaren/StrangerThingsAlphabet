
#include <OctoWS2811.h>

const int ledsPerStrip = 100;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

/*
// Dimmer colours
#define BLACK 0x000000
#define RED    0x660000
#define ORANGE 0x663300
#define YELLOW 0x666600
#define GREEN  0x006600
#define BLUE   0x111166
#define VIOLET 0x773366
*/

// Full colours
#define BLACK 0x000000
#define RED    0xFF0000
#define ORANGE 0xFF6600
#define YELLOW 0xFFFF00
#define GREEN  0x00FF00
#define BLUE   0x2222FF
#define VIOLET 0xCC66FF

int ledColors[] = {
  RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET
};
int ledColorsLength;

char messages[] =
//"abcdefghijklmnopqrstuvwxyz."
"help me."
"theyre coming."
"i love lamp."
"im scared."
"help."
"bieber is after me."
"were doomed."
"theyre going to kill me."
"can i add you on linkedin."
"were all gonna die."
;

int charIndexMap[] = {
  11, // a
  13, // b
  15, // c
  17, // d
  20, // e
  22, // f
  24, // g
  27, // h
  51, // i
  50, // j
  49, // k
  48, // l
  46, // m
  44, // n
  42, // o
  41, // p
  37, // q
  59, // r
  61, // s
  63, // t
  65, // u
  67, // v
  68, // w
  71, // x
  72, // y
  77, // z
};

int messagesLength;
int messageCharIndex;

void setup() {
  leds.begin();
  leds.show();

  ledColorsLength = sizeof(ledColors) / sizeof(int);

  messagesLength = sizeof(messages) / sizeof(char);

  Serial.begin(9600);
}

void loop() {
  char c = getNextChar();
  handleChar(c);
  leds.show();
}

char getNextChar() {
  char c = messages[messageCharIndex];

  messageCharIndex++;
  if (messageCharIndex > messagesLength) {
    messageCharIndex = 0;
  }

  return c;
}

void handleChar(char c) {
  int index = charToIndex(c);
  Serial.print("Char: '");
  Serial.print(c);
  Serial.println("'");
  displayChar(c);
}

void displayChar(char c) {
  int led = charToIndex(c);
  int color = getLedColor(led);
  for (int i = 0; i < leds.numPixels(); i++) {
    if (i != led) {
      leds.setPixel(i, BLACK);
    }
  }
  leds.show();

  fade(led, BLACK, color, 2);
  delayForChar(c);
  fade(led, color, BLACK, 2);
}

int getLedColor(int index) {
  return ledColors[index % ledColorsLength];
}

void fade(int led, int fromColor, int toColor, int delayMs) {
  float v = 0;
  for (int i = 0; i < 64; i++) {
    float v = (float)i / 64;
    leds.setPixel(led, lerpColor(fromColor, toColor, v));
    leds.show();
    delay(delayMs);
  }
}

void delayForChar(char c) {
  switch (c) {
    case '.':
      delay(random(5000, 15000));
      break;
    case ' ':
      delay(random(300, 600));
      break;
    default:
      delay(random(750, 1250));
      break;
  }
}

int charToIndex(char c) {
  int index = c - 97;
  if (index >= 0 && index < 26) {
    return charIndexMap[index];
  }
  return -1;
}

void colorWipe(int color) {
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
  }
}

int redMask = 0xFF0000, greenMask = 0xFF00, blueMask = 0xFF;
int lerpColor(int a, int b, float v) {
  int ar = (a & redMask) >> 16;
  int ag = (a & greenMask) >> 8;
  int ab = (a & blueMask);
  int br = (b & redMask) >> 16;
  int bg = (b & greenMask) >> 8;
  int bb = (b & blueMask);

  ar += (br - ar) * v;
  ag += (bg - ag) * v;
  ab += (bb - ab) * v;

  int rgb = (ar << 16) + (ag << 8) + ab;
  return rgb;
}

