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

#define RS 13
#define EN 12
#define D4 14
#define D5 27
#define D6 26
#define D7 25
LiquidCrystal lcd00(RS, EN, D4, D5, D6, D7);
LiquidCrystal lcd01(RS, EN, D4, D5, D6, D7);

// ------------- * PORTS * -------------

const uint8_t PIN_DHT00 = ;
const uint8_t PIN_DHT01 = ;

const uint8_t PIN_LDR00 = ;
const uint8_t PIN_LDR01 = ;

const uint8_t PIN_PIR = ;
const uint8_t PIN_MSENSOR = ;

const uint8_t PIN_LED00R = "";
const uint8_t PIN_LED00G = "";
const uint8_t PIN_LED00B = "";

const uint8_t PIN_LED01R = "";
const uint8_t PIN_LED01G = "";
const uint8_t PIN_LED01B = "";

const uint8_t PIN_LED02R = "";
const uint8_t PIN_LED02G = "";
const uint8_t PIN_LED02B = "";

const uint8_t PIN_LED03R = "";
const uint8_t PIN_LED03G = "";
const uint8_t PIN_LED03B = "";

const uint8_t PIN_LED04R = "";
const uint8_t PIN_LED04G = "";
const uint8_t PIN_LED04B = "";

// ------------- * CONST * -------------

const char WIFI_SSID[] = "yasses redmi";
const char WIFI_PASSWORD[] = "yasmi123456";

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

float moisture;
float moisture_perc;

uint8_t presence;

int flag = 0;

int humor_data;
int health_data;
int thirst_data;
int life_data;
int status_data;

char humor[50];
char health[50];
char thirst[50];
int life;
char status[50];

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
  json.set("luminosity", mluminosity);
  json.set("moisture", moisture_perc);
  json.set("presence", presence);
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

void getData(){
  if (Firebase.getInt(fbdo, "/stats/-NoHk1fddvCjR-pu6nCt/humor")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        humor_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/-NoHk1fddvCjR-pu6nCt/health")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        health_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/-NoHk1fddvCjR-pu6nCt/thirst")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        thirst_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/-NoHk1fddvCjR-pu6nCt/life")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        life_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/-NoHk1fddvCjR-pu6nCt/status")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        status_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }
}

void setColor(int r, int g, int b)
{
  int vermelho = 255 - r; 
  int verde = 255 - g; 
  int azul = 255 - b; 

  analogWrite(pinoR, vermelho);
  analogWrite(pinoG, verde); 
  analogWrite(pinoB, azul); 
}

void setStats()
{
  if(humor_data < 2)
  {
    strcpy(humor, "Triste");
  }
  else if(humor >= 2 and humor > 4)
  {
    strcpy(humor, "Feliz");
  }
  else if(humor >= 4)
  {
    strcpy(humor, "Ok");
  }

  if(health_data < 2)
  {
    strcpy(health, "Doente");
  }
  else if(health >= 2 and health > 4)
  {
    strcpy(health, "Saudavel");
  }
  else if(health >= 4)
  {
    strcpy(health, "Sobrevivendo");
  }

  if(thirst_data < 2)
  {
    strcpy(thirst, "Com sede");
  }
  else if(thirst >= 2 and thirst > 4)
  {
    strcpy(thirst, "Satisfeito");
  }
  else if(thirst >= 4)
  {
    strcpy(thirst, "Encharcado");
  }

  if(status == 0)
  {
    strcpy(status, "Morto");
  }
  else if(status == 1)
  {
    strcpy(status, "Vivo");
  }
}

void printLcd()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("* %s *", &humor);

  lcd.setCursor(0, 1);
  lcd.print("* %s *", &health);

  delay(5000);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(150);
  }

  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("* Life  -");

  lcd.setCursor(11, 0);
  lcd.printf("%d", life);

  lcd.setCursor(14, 0);
  lcd.print(" *");

  lcd.setCursor(0, 1);
  lcd.print("* %s *", &thirst);

  delay(5000);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(150);
  }

  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("* %s *", &status);

  delay(5000);

  for (int posicao = 0; posicao < 16; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(150);
  }

  delay(500);
}

void setup() 
{
  Serial.begin(115200);

  dht00.begin();
  dht01.begin();

  lcd00.begin(16, 2);
  lcd01.begin(16, 2);

  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_SECRET;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);

  pinMode(PIN_LDR00, INPUT);
  pinMode(PIN_LDR01, INPUT);
  pinMode(PIN_MSENSOR, INPUT);
  pinMode(PIN_PIR, INPUT);

  pinMode(PIN_LED00R, OUTPUT);
  pinMode(PIN_LED00G, OUTPUT);
  pinMode(PIN_LED00B, OUTPUT);

  pinMode(PIN_LED01R, OUTPUT);
  pinMode(PIN_LED01G, OUTPUT);
  pinMode(PIN_LED01B, OUTPUT);

  pinMode(PIN_LED02R, OUTPUT);
  pinMode(PIN_LED02G, OUTPUT);
  pinMode(PIN_LED02B, OUTPUT);

  pinMode(PIN_LED03R, OUTPUT);
  pinMode(PIN_LED03G, OUTPUT);
  pinMode(PIN_LED03B, OUTPUT);

  pinMode(PIN_LED04R, OUTPUT);
  pinMode(PIN_LED04G, OUTPUT);
  pinMode(PIN_LED04B, OUTPUT);
}

void loop() 
{
  lcd00.clear();
  lcd01.clear();

  temperature00 = dht00.readTemperature();
  temperature01 = dht01.readTemperature();
  luminosity00 = analogRead(PIN_LDR00);
  luminosity01 = analogRead(PIN_LDR01);
  moisture = analogRead(PIN_MSENSOR);
  presence = digitalRead(PIN_PIR);

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

  getData();

  setStats();

  printLcd();

}
