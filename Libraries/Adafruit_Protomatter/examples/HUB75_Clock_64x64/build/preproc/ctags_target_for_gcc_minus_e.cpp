# 1 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino"
# 2 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2
# 3 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2
# 4 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2
# 5 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2
# 6 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2
# 7 "C:\\Users\\gmeth\\Desktop\\HUB75_Clock_64x64\\HUB75_Clock_64x64.ino" 2






Adafruit_Protomatter matrix(
    64, // Width of panels
    4, // Bitdepth
    1, rgbPins, // , RGB pins
    4, addrPins, // No of address pins, address pins
    clockPin, latchPin, oePin, // clock, latch output enable pins
    true, // Dubble buffering
    -2); // number of panels high (- if alternate panel upside down (zig-zag scanning))
Timezone myTZ;

const byte CENTRE_X = 32;
const byte CENTRE_Y = 32;
int Tz = 1; // Time Zone
uint8_t secondInt;
uint8_t minuteInt;
uint8_t hourInt;
uint8_t NewRTCm = 60;
int co0, co1;
uint16_t MyColor[92] = {0xF800, 0xF880, 0xF900, 0xF980, 0xFA20, 0xFAA0, 0xFB20, 0xFBA0, 0xFC40, 0xFCC0, 0xFD40, 0xFDC0,
                        0xFDC0, 0xFE60, 0xFEE0, 0xFF60, 0xFFE0, 0xEFE0, 0xDFE0, 0xCFE0, 0xBFE0, 0xAFE0, 0x9FE0, 0x8FE0,
                        0x77E0, 0x67E0, 0x57E0, 0x47E0, 0x37E0, 0x27E0, 0x17E0, 0x07E0, 0x07E2, 0x07E4, 0x07E6, 0x07E8,
                        0x07EA, 0x07EC, 0x07EE, 0x07F1, 0x07F3, 0x07F5, 0x07F7, 0x07F9, 0x07FB, 0x07FD, 0x07FF, 0x077F,
                        0x06FF, 0x067F, 0x05DF, 0x055F, 0x04DF, 0x045F, 0x03BF, 0x033F, 0x02BF, 0x023F, 0x019F, 0x011F,
                        0x009F, 0x001F, 0x101F, 0x201F, 0x301F, 0x401F, 0x501F, 0x601F, 0x701F, 0x881F, 0x981F, 0xA81F,
                        0xB81F, 0xC81F, 0xD81F, 0xE81F, 0xF81F, 0xF81D, 0xF81B, 0xF819, 0xF817, 0xF815, 0xF813, 0xF811,
                        0xF80E, 0xF80C, 0xF80A, 0xF808, 0xF806, 0xF804, 0xF802, 0xF800};

unsigned long lastTime = millis();


float ballX = 31;
float ballY = (random(16)) + 8;
float leftPlayerTargetY = ballY;
float rightPlayerTargetY = ballY;
float leftPlayerY = 8;
float rightPlayerY = 18;
float ballVX = 1;
float ballVY = 0.5;
int playerLoss = 0;
int gameStopped = 0;
String HourString, MinuteString, SecondString;
int HourOld, MinuteOld, secondOld;
String timeStr = "00";

void setup()
{
    Serial.begin(115200);
    randomSeed(analogRead(A3));
    // Attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);

    // Set WiFi to station mode and disconnect from an AP if it was Previously
    // connected
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize matrix...
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if (status != PROTOMATTER_OK)
    {
        for (;;)
            ; // halt of display error
    }
    matrix.setTextWrap(false); // Allow text to run off right edge
    matrix.setTextSize(0);
    matrix.setTextColor(myYELLOW);
    matrix.setCursor(0, 0);
    matrix.fillScreen(myBLACK);
    waitForSync();
    Serial.println();
    Serial.println("UTC:             " + UTC.dateTime());

    myTZ.setLocation((reinterpret_cast<const __FlashStringHelper *>(("Europe/London"))));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(("Time in your set timezone:         "))));
    Serial.println(myTZ.dateTime());

    matrix.fillScreen(myBLACK);
}
void redtime()
{
    //   secondInt = rtc.getsecondInt();
    //   minuteInt = rtc.getminuteInt();
    //   hour = rtc.getHour();
    //   co0 = random(0, 91);
    //   if (NewRTCm != minuteInt) {
    //     co1 = random(0, 91);
    //     NewRTCm = minuteInt;
    //   }
    HourString = myTZ.dateTime("H");
    MinuteString = myTZ.dateTime("i");
    SecondString = myTZ.dateTime(("s"));
    hourInt = HourString.toInt();
    minuteInt = MinuteString.toInt();
    secondInt = SecondString.toInt();

    co0 = random(0, 91);
    if (NewRTCm != minuteInt)
    {
        co1 = random(0, 91);
        NewRTCm = minuteInt;
    }
}
void drawclockNB()
{
    float radians, angle;
    int j = 0;
    matrix.setFont(&fontclock);
    for (int i = 0; i < 60; i += 5)
    {
        matrix.setTextColor(MyColor[(co1 + i * 5) % 92]);
        angle = 180 - i * 6;
        radians = ((angle)*0.017453292519943295769236907684886);
        int x0 = CENTRE_X + 26 * sin(radians);
        int y0 = CENTRE_Y + 26 * cos(radians);
        matrix.setCursor(x0, y0);
        matrix.print(char(j));
        j++;
        if (j == 10)
        {
            j = 11;
        }
    }
    matrix.setFont();
}

void drawclockDot()
{
    float radians, angle;
    for (int i = 0; i < 60; i += 5)
    {
        uint16_t color = MyColor[(co0 + i * 5) % 92];
        angle = 360 - i * 6;
        radians = ((angle)*0.017453292519943295769236907684886);
        int x0 = CENTRE_X + 30 * sin(radians);
        int y0 = CENTRE_Y + 30 * cos(radians);
        matrix.fillCircle(x0, y0, 1, color);
    }
}

void drawclockBDot()
{
    float radians, angle;
    matrix.drawCircle(CENTRE_X, CENTRE_Y, 30, MyColor[co1]);
    for (int i = 0; i < 60; i += 15)
    {
        uint16_t color = MyColor[(co1 + i * 5) % 92];
        angle = 360 - i * 6;
        radians = ((angle)*0.017453292519943295769236907684886);
        int x0 = CENTRE_X + 30 * sin(radians);
        int y0 = CENTRE_Y + 30 * cos(radians);
        matrix.fillCircle(x0, y0, 2, color);
    }
}

void drawHMS(float angle, int x0, int y0, int w, int c0, int c1, int z1)
{
    int x[3];
    int y[3];
    float radians = ((angle)*0.017453292519943295769236907684886);
    x[0] = x0 + w * sin(radians);
    y[0] = y0 + w * cos(radians);
    radians = ((angle + z1)*0.017453292519943295769236907684886);
    x[1] = x0 + 6 * sin(radians);
    y[1] = y0 + 6 * cos(radians);
    radians = ((angle - z1)*0.017453292519943295769236907684886);
    x[2] = x0 + 6 * sin(radians);
    y[2] = y0 + 6 * cos(radians);
    matrix.fillTriangle(x0, y0, x[0], y[0], x[1], y[1], MyColor[c0]);
    matrix.fillTriangle(x0, y0, x[0], y[0], x[2], y[2], MyColor[c0]);
    matrix.drawLine(x[0], y[0], x[1], y[1], MyColor[c1]);
    matrix.drawLine(x[0], y[0], x[2], y[2], MyColor[c1]);
}

void drawtime()
{
    if (hourInt > 12)
        hourInt -= 12;
    float angle = 180 - (30 * hourInt) - (minuteInt / 2);
    drawHMS(angle, CENTRE_X, CENTRE_Y, 17, 0, 0, 30);
    angle = 180 - (6 * minuteInt);
    drawHMS(angle, CENTRE_X, CENTRE_Y, 25, 32, 34, 25);
    angle = 180 - (secondInt * 6);
    drawHMS(angle, CENTRE_X, CENTRE_Y, 29, co0, co1, 20);
    matrix.fillCircle(CENTRE_X, CENTRE_Y, 2, MyColor[co1]);
    matrix.fillCircle(CENTRE_X, CENTRE_Y, 1, MyColor[co0]);
}

void updateTime()
{
    // rtc.DSgetdatime();
    // configTime(Tz * 3600, 0, "", "");
}

void writeTime()
{
    // configTime(0, 0, "", "");
    // rtc.DSsetdatime();
    // configTime(Tz * 3600, 0, "", "");
}
void loop()
{
    matrix.fillScreen(myBLACK);
    redtime();
    drawclockBDot();
    drawclockDot();
    drawclockNB();
    drawtime();
    matrix.show();
    delay(1000);
}
