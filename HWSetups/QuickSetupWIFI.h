
#include <WiFi.h>
#define USE_WIFI

WiFiClient client;


const char *HostName;
void WIFI_Setup()
{
    matrix.println("\nConnecting ");
    matrix.println("To WiFi ");
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    matrix.show();

    String Host = "PicoW";
    HostName = Host.c_str();
    // Set WiFi to station mode and disconnect from an AP if it was Previously connected
    WiFi.mode(WIFI_STA);
    // WiFi.config(ip);
    WiFi.setHostname(HostName);
    WiFi.begin(ssid, password);
    int i = 0;

    while (WiFi.status() != WL_CONNECTED)
        {
        Serial.print(".");
        matrix.drawPixel(i  , 25,myYELLOW);
        matrix.drawPixel(i-1, 25,myBLACK);
        matrix.show();
        delay(500);
        i++;
        if (i > 64)
            i = 0;
        }

    Serial.println("");
    Serial.println("WiFi\nconnected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    matrix.fillScreen(myBLACK);
    matrix.setCursor(0, 0);
    matrix.println("\nWiFi\nconnected");
    String IPadd = "IP address :- ";
    IPadd += WiFi.localIP().toString().c_str();
    matrix.println(IPadd);
    matrix.show();

    delay(2000);
}