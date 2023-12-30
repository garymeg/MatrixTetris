
// Adafruit Protomatter matrix driver
// Boilerplate code for raspberry Pico(W)
// This include ezTime library,
//              Wifi library,
//              Scrolling message library
//
// By Gary Metheringham (C) DEC 2023
// With thanks to OldSkoolCoder for the Scrolling message
// library.
//? #define DO_NOT_UPDATE_WEATHER

//? ===>   Quick Setup   <=====//
#include <Adafruit_GFX.h>
#include <Arduino_JSON.h>
#include "Secret.h"
#include "HWSetups/1_QuickSetupMatrix.h"
//& #include "HWSetups/QuickSetupWIFI.h"
//& #include "HWSetups/QuickSetupTime.h"
//#include "GPS.h"
//?=====> Project  <=======//
#include "MatrixTetris.h"

void setup() {
  Serial.begin(115200);
  // while (!Serial);  //! wait for serial port to connect. Needed for debugging
  //^ Initialize matrix...
#ifdef USE_MATRIX
  Matrix_Setup();
#endif
  //^ Attempt to connect to Wifi network:
#ifdef USE_WIFI
  WIFI_Setup();
#endif
  //^ setup ezTime
#ifdef USE_TIME
  EZtime_Setup();
#endif
  // setup GPS
#ifdef USE_GPS
  GPS_Setup();
#endif
  // Get a random seed from millis(), now as wifi
  // don't always take same time to connect
  randomSeed(millis());

  // Wait for any text from setup to be read
  delay(1000);

  // Ensure screen cleared ready for main program
  matrix.fillScreen(myBLACK);
  matrix.show();

  //========>  END OF QUICK SETUP  <==============//
  //=====>  Your Setup code goes here  <==========//
  TetrisSetup();

  //========> End of Your Setup code  <===========//
}
//Convert unix time to human readable time
void loop() {
  TetrisLoop();
}

// 1703519262