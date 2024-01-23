#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include "LiquidCrystal.h"
#include "String.h"

#define RS 13
#define EN 12
#define D4 14
#define D5 27
#define D6 26
#define D7 25
LiquidCrystal lcd00(RS, EN, D4, D5, D6, D7);

// ------------- * CONST * -------------

const char WIFI_SSID[] = "belisarius";
const char WIFI_PASSWORD[] = "senhaaaa";

const char FB_HOST[] = "https://plangotchi-default-rtdb.firebaseio.com/";
const char FB_SECRET[] = "H8faDHZoKN2657IZLqz3O7ARvnx2IDKZkWI7oC22";

// ------------- * VARIABLES * -------------

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

uint8_t PIN_BUTTON = 32;
int button;

// ------------- * FUNCTIONS * -------------

void connectWiFi(const char* ssid, const char* password)
{
  Serial.print("Connecting");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED)
  {
      Serial.write('.');
      delay(500);
  }

  Serial.print("\nConnected on IP: ");
  Serial.println(WiFi.localIP());
}

void sendData()
{
  FirebaseJson json;
  json.set("button", button);

  bool status = Firebase.updateNode(fbdo, "/sensors", json);

  if (!status)
  {
      Serial.print("Failed to send data to Firebase: ");
      Serial.println(fbdo.errorReason().c_str());
  }
  else
  {
    Serial.println("Data sent with success!");
    flag++;
  }
}

void printlcd01(){
  lcd00.clear();

  lcd00.setCursor(0, 0);
  lcd00.print("* Temp - 30 *");

  lcd00.setCursor(0, 1);
  lcd00.print("* Lumi - 2 *");

  delay(2500);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd00.scrollDisplayRight();
    delay(150);
  }

  delay(500);

  lcd00.setCursor(0, 0);
  lcd00.print("* Humi - 80% *");

  lcd00.setCursor(0, 1);
  lcd00.print("* PLANGOTCHI! *");

  delay(2500);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd00.scrollDisplayLeft();
    delay(150);
  }

  delay(2500);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd00.scrollDisplayRight();
    delay(150);
  }

  delay(500);
}

void setup() 
{
  Serial.begin(115200);

  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_SECRET;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);

  lcd00.begin(16, 2);

  pinMode(PIN_BUTTON, INPUT_PULLDOWN);
}

void loop() 
{
  printlcd00();
  button = digitalRead(PIN_BUTTON);
  // sendData();
}