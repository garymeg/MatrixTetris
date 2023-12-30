#pragma once
#define USE_MATRIX
#include <Adafruit_Protomatter.h>
#include <Wire.h>
//#include "../Libraries/Adafruit_Protomatter/src/Adafruit_Protomatter.h"

// GM Matrix Daughterboard Config
// Nothing to edit in here
//#include <SPI.h>
//#include <SD.h>


#include <Fonts/FreeSansBold18pt7b.h> // Large friendly font
#include <Fonts/FreeSansBold9pt7b.h>  // Large friendly font
#include <Fonts/FreeSans9pt7b.h>      // Large friendly font
#include "../Fonts/Modeseven_L3n55pt7b.h"// clean 7x8 font

// Set Panel Width/Height if not using a Constants.h file
// to set display WIDTH/HEIGHT
#define WIDTH 64
#define HEIGHT 64

    #define matrix_CLK 12
    #define matrix_OE 14
    #define matrix_LAT 13
    #define matrix_A 2
    #define matrix_B 3
    #define matrix_C 4
    #define matrix_D 5  //5
    #define matrix_E 15

    #define matrix_R1 6
    #define matrix_G1 7
    #define matrix_B1 8
    #define matrix_R2 9
    #define matrix_G2 10
    #define matrix_B2 11

        // Set default SPI Pins (SD Card)

    #define SPI0_MISO   16
    #define SPI0_MOSI   19
    #define SPI0_CLK    18
    #define SDCARD_CS   17

        // Reset default I2C Pins
    #define I2C_SDA 20
    #define I2C_SCL 21

  

uint16_t Color333(uint8_t r, uint8_t g, uint8_t b)
{
    // RRRrrGGGgggBBBbb
    return ((r & 0x7) << 13) | ((r & 0x6) << 10) | ((g & 0x7) << 8) |
           ((g & 0x7) << 5) | ((b & 0x7) << 2) | ((b & 0x6) >> 1);
}

// #define bitDepth 6

uint8_t rgbPins[] = {matrix_R1, matrix_G1, matrix_B1, matrix_R2, matrix_G2, matrix_B2};
uint8_t addrPins[] = {matrix_A, matrix_B, matrix_C, matrix_D, matrix_E};
uint8_t clockPin = matrix_CLK;
uint8_t latchPin = matrix_LAT;
uint8_t oePin = matrix_OE;
uint8_t scanMode = (HEIGHT == 64) ? -2 : -1;

  // standard colors
    // 333 colours 
    uint16_t myRED = Color333(2, 0, 0);
    uint16_t myGREEN = Color333(0, 2, 0);
    uint16_t myBLUE = Color333(0, 0, 2);
    uint16_t myWHITE = Color333(2, 2, 2);
    uint16_t myYELLOW = Color333(2, 2, 0);
    uint16_t myCYAN = Color333(0, 2, 2);
    uint16_t myMAGENTA = Color333(2, 0, 2);
    uint16_t myShadow = Color333(1, 0, 2);
    uint16_t myROSE = Color333(2, 0, 1);
    uint16_t myBLACK = Color333(0, 0, 0);
    uint16_t myGREY = Color333(2, 2, 2);
    uint16_t myGREY2 = Color333(1, 1, 1);

    uint16_t myRED_verylow = (3 << 11);
    uint16_t myRED_low = (7 << 11);
    uint16_t myRED_medium = (15 << 11);
    uint16_t myRED_high = (31 << 11);

    uint16_t myGREEN_verylow = (1 << 5);
    uint16_t myGREEN_low = (15 << 5);
    uint16_t myGREEN_medium = (31 << 5);
    uint16_t myGREEN_high = (63 << 5);

    uint16_t myBLUE_verylow = 3;
    uint16_t myBLUE_low = 7;
    uint16_t myBLUE_medium = 15;
    uint16_t myBLUE_high = 31;

    uint16_t myORANGE_verylow =  (myRED_verylow    + myGREEN_verylow);
    uint16_t myORANGE_low =      (myRED_low        + myGREEN_low);
    uint16_t myORANGE_medium =   (myRED_medium     + myGREEN_medium);
    uint16_t myORANGE_high =     (myRED_high       + myGREEN_high);

    uint16_t myPURP_Leverylow =  (myRED_verylow    + myBLUE_verylow);
    uint16_t myPURP_Lelow =      (myRED_low        + myBLUE_low);
    uint16_t myPURP_Lemedium =   (myRED_medium     + myBLUE_medium);
    uint16_t myPURP_Lehigh =     (myRED_high       + myBLUE_high);

    uint16_t myCYAN_verylow =    (myGREEN_verylow  + myBLUE_verylow);
    uint16_t myCYAN_low =        (myGREEN_low      + myBLUE_low);
    uint16_t myCYAN_medium =     (myGREEN_medium   + myBLUE_medium);
    uint16_t myCYAN_high =       (myGREEN_high     + myBLUE_high);

    uint16_t myWHITE_verylow =   (myRED_verylow    + myGREEN_verylow + myBLUE_verylow);
    uint16_t myWHITE_low =       (myRED_low        + myGREEN_low     + myBLUE_low);
    uint16_t myWHITE_medium =    (myRED_medium     + myGREEN_medium  + myBLUE_medium);
    uint16_t myWHITE_high =      (myRED_high       + myGREEN_high    + myBLUE_high);


    //#define Clear() fillScreen(myBLACK)
    
    Adafruit_Protomatter matrix(
        WIDTH,                     // Width of panels
        4,                         // Bitdepth
        1, rgbPins,                // Number of parralell matrix (not linked), RGB pins
        5, addrPins,               // No of address pins, address pins
        clockPin, latchPin, oePin, // clock, latch output enable pins
        true,                      // Dubble buffering
        //(HEIGHT==64? -2:1));       // number of panels high (- if alternate panel upside down (zig-zag scanning))
        -1);
    Adafruit_Protomatter* display = nullptr;
    void changeState(uint8_t);

    void Matrix_Setup()
    {

        ProtomatterStatus status = matrix.begin();
        Serial.print("Protomatter begin() status: ");
        Serial.println((int)status);

        if (status != PROTOMATTER_OK)
        {
        for (;;)
            ; // halt of display error
        }

    matrix.setTextWrap(true); // Allow text to run off right edge
    matrix.setFont(&Modeseven_L3n55pt7b); // Set a tidy 6x8 font
    matrix.setTextSize(0);
    matrix.setTextColor(myYELLOW);
    matrix.fillScreen(myBLACK);
    matrix.setCursor(0, 0);
    SPI.setRX(SPI0_MISO);
    SPI.setTX(SPI0_MOSI);
    SPI.setSCK(SPI0_CLK);
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);

        delay(2000);
        return;

    matrix.show();
    delay(2000);
    matrix.fillScreen(myBLACK);
    matrix.setCursor(0, 0);
}
