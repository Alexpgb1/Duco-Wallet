#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ==========================
// WIFI
// ==========================

const char* SSID = "Linksys";
const char* PASSWORD = "Alex1212";


// ==========================
// DUINO-COIN
// ==========================

String DUCO_USER = "ESP";
String MINER_KEY = "1234";


// ==========================
// OLED
// ==========================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// ==========================
// MINING DATA
// ==========================

float hashrate = 0;
int shares = 0;
int difficulty = 0;

float balance = 0;
int miners = 0;


// ==========================
// SETUP
// ==========================

void setup() {

  Serial.begin(115200);


  Wire.begin(SDA_PIN, SCL_PIN);


  if(!display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C)) {

    Serial.println("OLED ERROR");
    while(true);
  }


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0,0);
  display.println("DUCO MINER");
  display.println("ESP32-C6");
  display.display();


  WiFi.begin(
    SSID,
    PASSWORD
  );


  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connecting WiFi...");
  display.display();


  while(WiFi.status()!=WL_CONNECTED){

    delay(500);
    Serial.print(".");
  }


  Serial.println();
  Serial.println("WiFi OK");


  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi Connected");
  display.println(WiFi.localIP());
  display.display();

  delay(2000);

}


// ==========================
// LOOP
// ==========================

void loop() {


  updateDisplay();


  delay(1000);

}



// ==========================
// OLED DISPLAY
// ==========================

void updateDisplay(){

  display.clearDisplay();

  display.setCursor(0,0);

  display.println("DUCO MINER");

  display.print("User: ");
  display.println(DUCO_USER);


  display.print("Hash: ");
  display.print(hashrate);
  display.println(" kH/s");


  display.print("Shares: ");
  display.println(shares);


  display.print("Diff: ");
  display.println(difficulty);


  display.println();


  display.print("Balance: ");
  display.println(balance);


  display.print("Miners: ");
  display.println(miners);


  display.display();

}