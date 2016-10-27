
#include <OctoWS2811.h>

const int ledsPerStrip = 50;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

#define BLACK 0x000000
#define BROWN 0x6A332F
#define RED    0xFF0000
#define ORANGE 0xFF6600
#define YELLOW 0xFFFF00
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define VIOLET 0xCC66FF
#define GREY 0x888888
#define WHITE  0xFFFFFF

#define PINK   0xFF1088

int ledColors[] = {
  RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET
};

char messages[] = "abcdefghijklmnopqrstuvwxyz."
"help."
"help me."
"im scared."
"theyre coming for me."
"theyre going to kill me."
"bieber is after me."
"can i add you on linkedin."
"the cake is a lie."
"were doomed."
"were all gonna die."
"i love lamp.";

int charIndexMap[] = {
  0, // a
  1, // b
  3, // c
  5, // d
  7, // e
  9, // f
  11, // g
  13, // h
  15, // i
  17, // j
  19, // k
  21, // l
  23, // m
  25, // n
  27, // o
  29, // p
  31, // q
  33, // r
  35, // s
  37, // t
  39, // u
  41, // v
  43, // w
  45, // x
  47, // y
  49, // z
};

int messagesLength;
int messageCharIndex;

void setup() {
  leds.begin();
  leds.show();

  messagesLength = sizeof(messages);

  Serial.begin(9600);
}

void loop() {
  char c = getNextChar();
  handleChar(c);
  leds.show();
  delay(2000);
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
  // TODO: Replace with actual library.
  // TODO: Fade on and fade off.
  int led = charToIndex(c);
  for (int i = 0; i < leds.numPixels(); i++) {
    if (i == led) {
      leds.setPixel(i, getLedColor(led));
    } else {
      leds.setPixel(i, BLACK);
    }
  }
  leds.show();
  delayForChar(c);
}

int getLedColor(int index) {
  return ledColors[index % sizeof(ledColors)];
}

void delayForChar(char c) {
  switch (c) {
    case '.':
      delay(random(5000, 10000));
      break;
    case ' ':
      delay(random(1500, 2000));
      break;
    default:
      delay(random(750, 1000));
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
