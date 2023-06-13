#include <WiFi.h>
#include "TFT_eSPI.h"
#include "bitcoinlogo.h"

// Global Variables
TFT_eSPI tft = TFT_eSPI();
#define top_button 0
#define lower_button 14
#define PIN_POWER_ON 15  // Power Pin
#define PIN_LCD_BL 38  // BackLight Pin
int progress = 0;
int blocks = 0;
WiFiClient client;
char server[] = "www.google.com";
const char* wifi_ssid = "ssid";
const char* wifi_password = "pass";


void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_LCD_BL, OUTPUT);
  pinMode(lower_button, INPUT);
  pinMode(top_button, INPUT);
  delay(100);

  // Start Backlight and Power LED.
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);

  // Set USB CDC On Boot: "Enabled"
  Serial.begin(115200);
  delay(100);

  // Start Display. Thin Film Transistor.
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(205, 40, 136, 170, bitcoinlogo);
  tft.setTextSize(1);
  tft.setTextDatum(4);
  delay(100);

  // Connect to WiFi.
  Serial.printf("Connecting to %s ...", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("Wifi Connected.");

  if (client.connect(server, 443)) {
    Serial.printf("Connected to %s...", server);
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  Serial.println("Setup Complete!");

  delay(1000);
}

void loop() {
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  if (!digitalRead(top_button) && !digitalRead(lower_button)) {
    tft.drawString("  HODL!  ", 77, 55, 4);
  } else {
    tft.drawString("  BITCOIN!  ", 80, 55, 4);
  }

  

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    // Serial.println();
    // Serial.println("disconnecting from server.");
    // client.connect("www.google.com", 80);
    // Serial.printf("Connected to %s...", server);
    // client.println("GET /search?q=arduino HTTP/1.1");
    // client.println("Host: www.google.com");
    // client.println("Connection: close");
    // client.println();
  }

  delay(1000);
}
