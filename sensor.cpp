#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include "FirebaseESP32.h"
#include "Adafruit_Sensor.h"
#include "LiquidCrystal.h"
#include "String.h"

#define DHT_TYPE DHT11
DHT dht00(PIN_DHT, DHT_TYPE);
DHT dht01(PIN_DHT, DHT_TYPE);

// ------------- * PORTS * -------------

const uint8_t PIN_DHT00 = 32;
const uint8_t PIN_DHT01 = 33;

const uint8_t PIN_LDR00 = 22;
const uint8_t PIN_LDR01 = 23;

// ------------- * CONST * -------------

const char WIFI_SSID[] = "belisarius";
const char WIFI_PASSWORD[] = "senhaaaa";

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

  FirebaseJson json;
  json.set("temperature", mtemperature);
  json.set("luminosity", mluminosity);
  json.set("flag", flag);

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

void setup() 
{
  Serial.begin(115200);

  dht00.begin();
  dht01.begin();

  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_SECRET;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);

  pinMode(PIN_LDR00, INPUT);
  pinMode(PIN_LDR01, INPUT);
}

void loop() 
{
  temperature00 = dht00.readTemperature();
  temperature01 = dht01.readTemperature();
  luminosity00 = analogRead(PIN_LDR00);
  luminosity01 = analogRead(PIN_LDR01);

  if(isnan(temperature00) || isnan(temperature01))
  {
    Serial.println("Failed to read DHT sensors \\:");
  }
  
  if(isnan(luminosity00) || isnan(luminosity01))
  {
    Serial.println("Failed to read LDR sensors \\:");
  }

  sendData();

}