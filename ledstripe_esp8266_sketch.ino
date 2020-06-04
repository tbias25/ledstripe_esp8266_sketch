#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#include <Adafruit_DotStar.h>
#include <SPI.h>

#define FIREBASE_HOST "secret"
#define FIREBASE_AUTH "secret"
#define WIFI_SSID "secret"
#define WIFI_PASSWORD "secret"

#define DATAPIN    13
#define CLOCKPIN   14

#define NUMPIXELS 60

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

FirebaseData firebaseData;

String path = "/ledstripe";
int brightness, r, g, b;
boolean brightnessReady = false;
boolean redColorReady = false;
boolean greenColorReady = false;
boolean blueColorReady = false;
void setup()
{

  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1);
  #endif
  strip.begin();
  strip.show();
  
}

void fetchBrightness() {
  if (Firebase.getInt(firebaseData, path + "/brightness"))
    {
      if (firebaseData.dataType() == "int") {
        brightness = firebaseData.intData();
        Serial.println("BRIGHTNESS");
        Serial.println(brightness);
        brightnessReady = true;
      }
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      brightnessReady = false;
    }
}

void fetchRedColor() {
      if (Firebase.getInt(firebaseData, path + "/color/r"))
    {
      if (firebaseData.dataType() == "int") {
        r = firebaseData.intData();
        Serial.println("R");
        Serial.println(r);
        redColorReady = true;
      }
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      redColorReady = false;
    }
}

void fetchGreenColor() {
      if (Firebase.getInt(firebaseData, path + "/color/g"))
    {
      if (firebaseData.dataType() == "int") {
        g = firebaseData.intData();
        Serial.println("G");
        Serial.println(r);
        greenColorReady = true;
      }
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      greenColorReady = false;
    }
}

void fetchBlueColor() {
      if (Firebase.getInt(firebaseData, path + "/color/b"))
    {
      if (firebaseData.dataType() == "int") {
        b = firebaseData.intData();
        Serial.println("B");
        Serial.println(b);
        blueColorReady = true;
      }
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      blueColorReady = false;
    }
}

void loop()
{

fetchBrightness();
fetchRedColor();
fetchGreenColor();
fetchBlueColor();
if((blueColorReady == true) && (greenColorReady == true) && (redColorReady == true) && (brightnessReady == true)) {
 for(int i = 0; i < 60; i++) {
 uint32_t color = strip.Color(g, r, b);
 strip.setPixelColor(i, color); 
}
blueColorReady = false;
greenColorReady = false;
redColorReady = false;
brightnessReady = false;
strip.setBrightness(brightness);
Serial.println("SUCCESS");
}
strip.show();
delay(20);
}
