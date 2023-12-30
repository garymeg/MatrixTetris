# 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino"
# 2 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2

# 4 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2
# 5 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2
# 6 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2
# 7 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2
# 8 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2

// set matrix width and height here




// this contains the pinout for RP2040 and ESP32
# 16 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\picoAsteriods.ino" 2

Adafruit_Protomatter matrix(
  64, // Matrix width in pixels
  6, // Bit depth -- 6 here provides maximum color options
  1, rgbPins, // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true, // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!
  -(32/16)); // panel scanning order do not change




std::vector<Asteroid> myAsteroids;
Player myPlayer(&matrix);

void debugPrinting(String message)
{



  Serial.println(message);

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
  matrix.setTextWrap(false); // Allow text to run off right edge
  //matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font

  int x, y = 0;

  for (int i = 0; i < 4 + level; i++)
  {
    x = rand() % 64;
    y = rand() % 64;

    Asteroid myAster(&matrix, x, y, 6, level);
    myAsteroids.push_back(myAster);
  }
  myPlayer.NewShip();
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix.fillScreen(0);
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
