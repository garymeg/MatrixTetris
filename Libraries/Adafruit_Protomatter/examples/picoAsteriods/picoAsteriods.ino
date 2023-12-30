#include <Adafruit_Protomatter.h>

#include <vector>
#include <string>
#include "Asteroid.cpp"
#include "Constants.cpp"
#include "Player.cpp"

// set matrix width and height here
#undef M_WIDTH
#undef M_HEIGHT
#define M_WIDTH 64
#define M_HEIGHT 32
// this contains the pinout for RP2040 and ESP32
#include <Matrix_Config_GM.h>

Adafruit_Protomatter matrix(
  M_WIDTH,          // Matrix width in pixels
  6,           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true,       // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!
  -(M_HEIGHT/16)); // panel scanning order do not change

#define setBrightness(x) fillScreen(0)  // no-op, no brightness on this board
#define clear() fillScreen(0)

std::vector<Asteroid> myAsteroids;
Player myPlayer(&matrix);

void debugPrinting(String message)
{
#if defined(ARDUINO_ARCH_RP2040)
  Serial.println(message);
#elif defined(ARDUINO_ARCH_ESP32)
  Serial.println(message);
#endif
}

int level = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  ProtomatterStatus status = matrix.begin();
  debugPrinting("Protomatter begin() status: ");
  debugPrinting((String)status);
  if(status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }
  matrix.setTextWrap(false);  // Allow text to run off right edge
  //matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font

  int x, y = 0;

  for (int i = 0; i < 4 + level; i++)
  {
    x = rand() % WIDTH;
    y = rand() % HEIGHT;

    Asteroid myAster(&matrix, x, y, ASTEROID_BIG, level);
    myAsteroids.push_back(myAster);
  }
  myPlayer.NewShip();
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix.clear();
  std::vector<Asteroid>::iterator astIT = myAsteroids.begin();

  while (astIT != myAsteroids.end())
  {
      astIT->Draw();
      astIT->Update();
      astIT->SetColour(255, 255, 255);

      Serial.println(astIT->getXPoint());
      ++astIT;
  }

  myPlayer.Draw();
  myPlayer.Update();

  matrix.show();
  Serial.println("Delay");
  delay(20);

}
