# 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino"
# 2 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2
//--------------------------------------------------------------------------
// Animated 'sand' for Adafruit Feather.  Uses the following parts:
//   - Feather 32u4 Basic Proto (adafruit.com/product/2771)
//   - Charlieplex FeatherWing (adafruit.com/product/2965 - any color!)
//   - LIS3DH accelerometer (2809)
//   - 350 mAh LiPoly battery (2750)
//   - SPDT Slide Switch (805)
//
// This is NOT good "learn from" code for the IS31FL3731; it is "squeeze
// every last byte from the microcontroller" code.  If you're starting out,
// download the Adafruit_IS31FL3731 and Adafruit_GFX libraries, which
// provide functions for drawing pixels, lines, etc.
//--------------------------------------------------------------------------

# 17 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2
//#include <Adafruit_LIS3DH.h>    // For accelerometer
# 19 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2
# 20 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2
# 21 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2
# 22 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\PixelDust\\PixelDust.ino" 2





Adafruit_Protomatter matrix(
    64 /* Matrix width (pixels)*/, 4, 1, rgbPins, sizeof(addrPins), addrPins,
    clockPin, latchPin, oePin, true, -4);

// Adafruit_LIS3DH accel = Adafruit_LIS3DH();
Adafruit_MPU6050 accel;




uint16_t colors[8];

Adafruit_PixelDust sand(64 /* Matrix width (pixels)*/, 32 /* Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!*/, (8 * 8 * 8), 1, 128, false);

uint32_t prevTime = 0; // Used for frames-per-second throttle

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void err(int x)
{
    uint8_t i;
    pinMode((32u), OUTPUT); // Using onboard LED
    for (i = 1;; i++)
    { // Loop forever...
        digitalWrite((32u), i & 1); // LED on/off blink to alert user
        delay(x);
    }
}

void setup(void)
{
    Wire.setSDA(20);
    Wire.setSCL(21);
    Wire.begin();

    Serial.begin(115200);
    delay(5000);

    ProtomatterStatus status = matrix.begin();
    Serial.printf("Protomatter begin() status: %d\n", status);

    if (!sand.begin())
    {
        Serial.println("Couldn't start sand");
        err(1000); // Slow blink = malloc error
    }

    if (!accel.begin())
    {
        Serial.println("Couldn't find accelerometer");
        err(250); // Fast bink = I2C error
    }
    accel.setAccelerometerRange(MPU6050_RANGE_4_G); // 2, 4, 8 or 16 G!

    // sand.randomize(); // Initialize random sand positions

    // Set up initial sand coordinates, in 8x8 blocks
    int n = 0;
    for (int i = 0; i < 8; i++)
    {
        int xx = i * 64 /* Matrix width (pixels)*/ / 8;
        int yy = 32 /* Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!*/ - 8;
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 64 /* Matrix width (pixels)*/ / 8; x++)
            {
                // Serial.printf("#%d -> (%d, %d)\n", n,  xx + x, yy + y);
                sand.setPosition(n++, xx + x, yy + y);
            }
        }
    }
    Serial.printf("%d total pixels\n", n);

    colors[0] = matrix.color565(64, 64, 64); // Dark Gray
    colors[1] = matrix.color565(120, 79, 23); // Brown
    colors[2] = matrix.color565(228, 3, 3); // Red
    colors[3] = matrix.color565(255, 140, 0); // Orange
    colors[4] = matrix.color565(255, 237, 0); // Yellow
    colors[5] = matrix.color565(0, 128, 38); // Green
    colors[6] = matrix.color565(0, 77, 255); // Blue
    colors[7] = matrix.color565(117, 7, 135); // Purple
}

// MAIN LOOP - RUNS ONCE PER FRAME OF ANIMATION ----------------------------

void loop()
{
    // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
    // calculations are non-deterministic (don't always take the same amount
    // of time, depending on their current states), this helps ensure that
    // things like gravity appear constant in the simulation.
    uint32_t t;
    while (((t = micros()) - prevTime) < (1000000L / 45 /* Maximum redraw rate, frames/second*/))
        ;
    prevTime = t;

    // Read accelerometer...
    sensors_event_t ac, gy, tmp;
    accel.getEvent(&ac, &gy, &tmp);
    // Serial.printf("(%0.1f, %0.1f, %0.1f)\n", event.acceleration.x, event.acceleration.y, event.acceleration.z);

    double xx, yy, zz;
    yy = ac.acceleration.x * 1000;
    xx = ac.acceleration.y * 1000;
    zz = ac.acceleration.z * 1000;

    // Run one frame of the simulation
    sand.iterate(-xx, -yy, zz);

    // sand.iterate(-accel.y, accel.x, accel.z);

    // Update pixel data in LED driver
    dimension_t x, y;
    matrix.fillScreen(0x0);
    for (int i = 0; i < (8 * 8 * 8); i++)
    {
        sand.getPosition(i, &x, &y);
        int n = i / ((64 /* Matrix width (pixels)*/ / 8) * 8); // Color index
        uint16_t flakeColor = colors[n];
        matrix.drawPixel(x, y, flakeColor);
        // Serial.printf("(%d, %d)\n", x, y);
    }
    matrix.show(); // Copy data to matrix buffers
}
