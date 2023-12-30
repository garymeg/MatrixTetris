

/*
  Listfiles

  This example shows how print out the files in a
  directory on a SD card

  This Code is designed to be used with My
  RPi-Pico(w), ESP32 Px.x HAL75
  LED Matrix Display Breakout board

  created   Nov 2010 by David A. Mellis
  modified 9 Apr 2012 by Tom Igoe
  modified 2 Feb 2014 by Scott Fitzgerald
  modified 11Mar 2023 by Gary Metheringham

  This example code is in the public domain.

*/

// This are GP pins for SPI0 on the Raspberry Pi Pico board, and connect
// to different *board* level pinouts.  Check the PCB while wiring.
// Only certain pins can be used by the SPI hardware, so if you change
// these be sure they are legal or the program will crash.
// See: https://datasheets.raspberrypi.com/picow/PicoW-A4-Pinout.pdf

// todo
// ESP SD Card Not working

#include <Matrix_Config_GM.h>
#include <Adafruit_Protomatter.h>
#include <SPI.h>
#include <SD.h>
#define HEIGHT 32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH 64  // Matrix width (pixels)

// setup display object
// Pins are defined in Protomatter library in file 
// Adafruit_Protomatter/src/Matrix_Config_GM

Adafruit_Protomatter Matrix(
	WIDTH, 4, 1, rgbPins, sizeof(addrPins), addrPins,
	clockPin, latchPin, oePin, true, scanMode);

File root;

void setup()
{
	// Open serial communications and wait for port to open:
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}
	// Initialize matrix...
	ProtomatterStatus status = Matrix.begin();
	Serial.print("Protomatter begin() status: ");
	Serial.println((int)status);
	if (status != PROTOMATTER_OK)
	{
		// DO NOT CONTINUE if matrix setup encountered an error.
		// Flash Onboard LED
		flashLED("MATRIX init!");
	}
	delay(5000);
	Serial.print("Initializing SD card...");
	// Ensure the SPI pinout the SD card is connected to is configured properly
	SPI.setRX(SPI0_MISO);
	SPI.setTX(SPI0_MOSI);
	SPI.setSCK(SPI0_CLK);

	if (!SD.begin(SDCARD_CS))
	{
		Serial.println("initialization failed!");
		flashLED("SD CARD Init!");
		return;
	}

	Matrix.setTextSize(0);
	Matrix.color565(2, 2, 0);
	
	Serial.println("initialization done.");
	Serial.println();
	Serial.print("Card type:         ");
	Matrix.setCursor(0, 8);
	Matrix.print("Type ");
	

	switch (SD.type())
	{
	case SD_CARD_TYPE_SD1:
		Serial.println("SD1");
		Matrix.println("SD1");
		break;

	case SD_CARD_TYPE_SD2:
		Serial.println("SD2");
		Matrix.println("SD2");
		break;

	case SD_CARD_TYPE_SDHC:
		Serial.println("SDHC");
		Matrix.println("SDHC");
		break;

	default:
		Serial.println("SDHC");
		Matrix.println("SDHC");
		root = SD.open("/");

		printDirectory(root, 0);

		Serial.println("done!");
	}
	Matrix.show();
}
void loop()
{
	// nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs)
{
// we have to seperate out raspberry pico and esp32 as they use different SD card library builds
// thus having different commands
#if defined (ARDUINO_ARCH_RP2040)
	while (true)
	{

		File entry = dir.openNextFile();
		if (!entry)
		{
			// no more files
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++)
		{
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory())
		{
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
		}
		else
		{
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.print(entry.size(), DEC);
			time_t cr = entry.getCreationTime();
			time_t lw = entry.getLastWrite();
			struct tm *tmstruct = localtime(&cr);
			Serial.printf("\tCREATION: %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
			tmstruct = localtime(&lw);
			Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
		}
		entry.close();
	}
	#endif

}
void flashLED(String ERROR){
	Serial.print("STOP!! ERROR :  ");
	Serial.print(ERROR);
	while (true)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);

		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
	}
}