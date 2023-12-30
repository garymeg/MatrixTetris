#include <Adafruit_Protomatter.h>
#include <Matrix_Config_GM.h>

#define MYTIMEZONE "Europe/London"
#define clear() matrix.fillScreen(myBLACK)
#define show() matrix.show()
#define WIDTH 64
#define HEIGHT 64
Adafruit_Protomatter matrix(
    64,                        // Width of panels
    4,                         // Bitdepth
    1, rgbPins,                // , RGB pins
    4, addrPins,               // No of address pins, address pins
    clockPin, latchPin, oePin, // clock, latch output enable pins
    true,                      // Dubble buffering
    -2);                       // number of panels high (- if alternate panel upside down (zig-zag scanning))

void setup()
{
    // Initialize matrix...
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if (status != PROTOMATTER_OK)
    {
        for (;;)
            ; // halt of display error
    }

    for (int h = 0; h < 65536; h+=1000)
    {
        for (int s = 0; s < 256;s++)
        {
        //h = 0 -> 65535,  s = 0 -> 255, v = 0 -> 15
        matrix.drawPixel((int)(h / 1000), ((int)s / 4), matrix.colorHSV(h, 255, s));
        }
    }
    show();
}

void loop()
{
}
