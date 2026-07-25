#include <Wire.h>
#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi
const char *ssid = "your wifi ";
const char *password = "your wifi pass";

// Duino-Coin
const String ducoUser = "your duco username";
const String ducoReportJsonUrl = "https://server.duinocoin.com/v2/users/" + ducoUser + "?limit=1";

const int run_in_ms = 2000;


void setup() {
  Serial.begin(115200);

  setupWifi();
  initDisplayOled();
}


void setupWifi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


String httpGetString(String URL) {

  String payload = "";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  if (http.begin(client, URL)) {

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
    }
    else {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
    }

    http.end();
  }

  return payload;
}


void initDisplayOled() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

    Serial.println("OLED error!");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.println("Duino-Coin Monitor");
  display.println("Connecting...");
  display.display();

  delay(2000);
}


boolean runEvery(unsigned long interval) {

  static unsigned long previousMillis = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    return true;
  }

  return false;
}



void loop() {


  if (runEvery(run_in_ms)) {


    String input = httpGetString(ducoReportJsonUrl);


    DynamicJsonDocument doc(8000);

    DeserializationError error = deserializeJson(doc, input);


    if(error) {

      Serial.print("JSON Error: ");
      Serial.println(error.c_str());

      return;
    }


    JsonObject result = doc["result"];


    double balance = result["balance"]["balance"];

    const char* username = result["balance"]["username"];


    float totalHashrate = 0;

    int workers = 0;


    for(JsonObject miner : result["miners"].as<JsonArray>()) {

      float hashrate = miner["hashrate"];

      totalHashrate += hashrate;

      workers++;

    }


    Serial.println("----------------");
    Serial.print("User: ");
    Serial.println(username);

    Serial.print("Balance: ");
    Serial.println(balance);

    Serial.print("Workers: ");
    Serial.println(workers);

    Serial.print("Hashrate: ");
    Serial.println(totalHashrate);



    display.clearDisplay();

    display.setCursor(0,0);

    display.println("Duino-Coin");

    display.println();

    display.print("User: ");
    display.println(username);

    display.print("DUCO: ");
    display.println(balance);

    display.print("Workers: ");
    display.println(workers);

    display.print("Hash: ");
    display.print(totalHashrate / 1000);
    display.println(" Kh/s");


    display.display();

  }

}
