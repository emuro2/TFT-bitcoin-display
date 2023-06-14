// #include <WiFi.h>
#include "TFT_eSPI.h"
#include <HTTPClient.h>
#include "bitcoinlogo.h"
#include <WiFiClientSecure.h>


// Global Variables
TFT_eSPI tft = TFT_eSPI();
#define top_button 0
#define lower_button 14
#define PIN_POWER_ON 15  // Power Pin
#define PIN_LCD_BL 38  // BackLight Pin
// WiFiClient client;
WiFiClientSecure client;
const char* wifi_ssid = "wifi";
const char* wifi_password = "pass";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 300000;
// Timer set to 3 minutes (300000)
unsigned long timerDelay = 300000;

String block_height = "0";
String block_reward = "0";
String total_mined = "0";
String price = "0";

const char* digicert_global_root_ca= \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
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
  tft.pushImage(0, 5, 136, 170, bitcoinlogo);
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
  Serial.println("Setup Complete!");
  delay(1000);
}

String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  client.setCACert(digicert_global_root_ca);
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  Serial.print("http begin...");
  http.begin(client, serverName);

  // Send HTTP Get request
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
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString(block_height, 220, 40, 4);
  tft.drawString(price, 220, 90, 4);
  tft.drawString(block_reward, 220, 140, 4);

  //Send an HTTP POST request every 5 minutes
  if ((millis() - lastTime) > timerDelay) {
    Serial.print("Request every x min...");
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      Serial.print("Wifi connected...");
      block_height = httpGETRequest("https://blockchain.info/q/getblockcount");
      block_reward = httpGETRequest("https://blockchain.info/q/bcperblock");
      total_mined = httpGETRequest("https://blockchain.info/q/totalbc");
      price = String(httpGETRequest("https://blockchain.info/q/24hrprice").toInt());
      Serial.println(block_height);
      Serial.println(block_reward);
      Serial.println(total_mined);
      Serial.println(price);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  delay(1000);
}
