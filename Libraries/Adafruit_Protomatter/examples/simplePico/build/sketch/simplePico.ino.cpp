#include <Arduino.h>
#line 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"

#include <Adafruit_Protomatter.h>

#define matrix_CLK 12
#define matrix_OE 14
#define matrix_LAT 13
#define matrix_A 2
#define matrix_B 3
#define matrix_C 4
#define matrix_D 5

#define matrix_R1 6
#define matrix_R2 9
#define matrix_G1 7
#define matrix_G2 10
#define matrix_B1 8
#define matrix_B2 11

uint8_t rgbPins[] = {matrix_R1, matrix_G1, matrix_B1, matrix_R2, matrix_G2, matrix_B2};
uint8_t addrPins[] = {matrix_A, matrix_B, matrix_C, matrix_D};
uint8_t clockPin = matrix_CLK;
uint8_t latchPin = matrix_LAT;
uint8_t oePin = matrix_OE;

Adafruit_Protomatter matrix(
    64,
    4,
    1, rgbPins,
    4, addrPins,
    clockPin, latchPin, oePin,
    true,
    1);

#line 34 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"
void setup(void);
#line 67 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"
void loop(void);
#line 34 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"
void setup(void)
{
  Serial.begin(9600);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK)
  {
    for (;;)
      ;
  }

  for (int x = 0; x < matrix.width(); x++)
  {
    uint8_t level = x * 256 / matrix.width();
    matrix.drawPixel(x, matrix.height() - 4, matrix.color565(level, 0, 0));
    matrix.drawPixel(x, matrix.height() - 3, matrix.color565(0, level, 0));
    matrix.drawPixel(x, matrix.height() - 2, matrix.color565(0, 0, level));
    matrix.drawPixel(x, matrix.height() - 1,
    matrix.color565(level, level, level));
  }
  matrix.drawCircle(12, 10, 9, matrix.color565(255, 0, 0));
  matrix.drawRect(14, 6, 17, 17, matrix.color565(0, 255, 0));
  matrix.drawTriangle(32, 9, 41, 27, 23, 27, matrix.color565(0, 0, 255));

  matrix.fillCircle(12 + 32, 10, 9, matrix.color565(50, 0, 0));
  matrix.fillRect(14 + 32, 6, 17, 17, matrix.color565(0, 50, 0));
  matrix.fillTriangle(32 + 32, 9, 41 + 32, 27, 23 + 32, 27, matrix.color565(0, 0, 50));
  matrix.println("ADAFRUIT");
  matrix.show();
}
void loop(void)
{
  delay(1);
}

