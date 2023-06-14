// #include <WiFi.h>
#include "TFT_eSPI.h"
#include <HTTPClient.h>
#include "bitcoinlogo.h"
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>


// Global Variables
TFT_eSPI tft = TFT_eSPI();
#define top_button 0
#define lower_button 14
#define PIN_POWER_ON 15  // Power Pin
#define PIN_LCD_BL 38  // BackLight Pin
int progress = 0;
int blocks = 0;
// WiFiClient client;
WiFiClientSecure client;
char server[] = "https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest?symbol=BTC";
const char* wifi_ssid = "wifi";
const char* wifi_password = "password";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 600000;
// Timer set to 10 minutes (600000)
unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
// unsigned long timerDelay = 10000;

String display = "";
String sensorReadings;
String sensorReadingsArr[1];

const char* aws_root_ca= \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
    "-----END CERTIFICATE-----\n";


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
  tft.setTextSize(3);
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
  Serial.println("Setup Complete!");
  delay(1000);
}

String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  client.setCACert(aws_root_ca);
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  Serial.print("http begin...");
  http.begin(client, serverName);

  // Set Headers
  Serial.print("http adding headers...");
  http.addHeader("Accept", "application/json");
  http.addHeader("X-CMC_PRO_API_KEY", "insert api key");

  // Send HTTP POST request
  Serial.print("http GET...");
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void loop() {
  Serial.print("loop...");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString(display, 200, 55, 4);

  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    Serial.print("Request every x min...");
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      Serial.print("Wifi connected...");
      sensorReadings = httpGETRequest(server);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      JSONVar data = myObject["data"];
      int price = int(data["BTC"]["quote"]["USD"]["price"]);
      display = String(price);
      // Serial.print(price);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  delay(1000);
}
