#include <Arduino.h>
#include <WiFi.h>
#include <WiFi.h>
#include "DHT.h"
#include "FirebaseESP32.h"
#include "Adafruit_Sensor.h"
#include "LiquidCrystal.h"

#define DHT_TYPE DHT11
DHT dht00(PIN_DHT, DHT_TYPE);
DHT dht01(PIN_DHT, DHT_TYPE);

#define RS 12
#define EN 11
#define D4  4
#define D5  5
#define D6  6
#define D7  7
LiquidCrystal lcd00(RS, EN, D4, D5, D6, D7);
LiquidCrystal lcd01(RS, EN, D4, D5, D6, D7);

// ------------- * PORTS * -------------

const uint8_t PIN_DHT00 = ;
const uint8_t PIN_DHT01 = ;

const uint8_t PIN_LDR00 = ;
const uint8_t PIN_LDR01 = ;

const uint8_t PIN_PIR = ;
const uint8_t PIN_MSENSOR = ;

const uint8_t PIN_LED00 = "";
const uint8_t PIN_LED01 = "";
const uint8_t PIN_LED02 = "";
const uint8_t PIN_LED03 = "";
const uint8_t PIN_LED04 = "";

// ------------- * CONST * -------------

const char WIFI_SSID[] = "";
const char WIFI_PASSWORD[] = "";

const char FB_HOST[] = "https://plangotchi-default-rtdb.firebaseio.com/";
const char FB_SECRET[] = "H8faDHZoKN2657IZLqz3O7ARvnx2IDKZkWI7oC22";

// ------------- * VARIABLES * -------------

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

float temperature00;
float temperature01;
float mtemperature;

float luminosity00;
float luminosity01;
float mluminosity;

uint8_t presence;

float moisture;
float moisture_perc;

int flag = 0;

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
  mtemperature = (temperature00 + temperature01) / 2;
  mluminosity = (luminosity00 + luminosity01) / 2;
  moisture_perc = map(moisture, 0, 4095, 0, 100);

  FirebaseJson json;
  json.set("temperature", mtemperature);
  json.set("moisture", moisture_perc);
  json.set("luminosity", mluminosity);
  json.set("presence", presence);

  bool status = Firebase.updateNode(fbdo, "/sensors", json);
  if (!status)
  {
      Serial.print("Failed to send data to Firebase: ");
      Serial.println(fbdo.errorReason().c_str());
  }
  else if(status)
  {
    Serial.println("Data sent with success!");
  }
}

void setup() 
{
  Serial.begin(115200);

  dht00.begin();
  dht01.begin();

  lcd00.begin(16, 2);
  lcd01.begin(16, 2);

  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  pinMode(PIN_LDR00, INPUT);
  pinMode(PIN_LDR01, INPUT);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_MSENSOR, INPUT);
}

void loop() 
{
  lcd00.clear();
  lcd00.setCursor(6, 0);
  lcd01.clear();
  lcd01.setCursor(6, 0);

  temperature00 = dht00.readTemperature();
  temperature01 = dht01.readTemperature();
  luminosity00 = analogRead(PIN_LDR00);
  luminosity01 = analogRead(PIN_LDR01);
  presence = digitalRead(PIN_PIR);
  moisture = analogRead(PIN_MSENSOR);

  if(isnan(temperature00) || isnan(temperature01))
  {
    Serial.println("Failed to read DHT sensors \\:");
  }
  
  if(isnan(luminosity00) || isnan(luminosity01))
  {
    Serial.println("Failed to read LDR sensors \\:");
  }
  
  if(isnan(moisture))
  {
    Serial.println("Failed to read PT100 sensor \\:");
  }
  
  if(isnan(presence))
  {
    Serial.println("Failed to read PIR sensor \\:");
  }

  sendData();
}
