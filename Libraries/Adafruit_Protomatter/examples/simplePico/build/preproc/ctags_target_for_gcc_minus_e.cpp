# 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"

# 3 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino" 2
# 19 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\simplePico\\simplePico.ino"
uint8_t rgbPins[] = {6, 7, 8, 9, 10, 11};
uint8_t addrPins[] = {2, 3, 4, 5};
uint8_t clockPin = 12;
uint8_t latchPin = 13;
uint8_t oePin = 14;

Adafruit_Protomatter matrix(
    64,
    4,
    1, rgbPins,
    4, addrPins,
    clockPin, latchPin, oePin,
    true,
    1);

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
