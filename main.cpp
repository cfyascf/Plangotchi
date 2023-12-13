#include <Arduino.h>
#include <WiFi.h>
#include <WiFi.h>
#include "DHT.h"
#include "FirebaseESP32.h"
#include "Adafruit_Sensor.h"
#include "LiquidCrystal.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define DHT_TYPE DHT11
DHT dht(PIN_DHT, DHT_TYPE);

#define RS 12
#define EN 11
#define D4  4
#define D5  5
#define D6  6
#define D7  7
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// ------------- * PORTS * -------------

const uint8_t PIN_DHT = ;
const uint8_t PIN_LDR = ;
const uint8_t PIN_PIR = ;
const uint8_t PIN_MSENSOR = ;

// ------------- * CONST * -------------

const char WIFI_SSID[] = "belisarius";
const char WIFI_PASSWORD[] = "senhaaaa";

const char FB_HOST[] = "https://plangotchi-default-rtdb.firebaseio.com/";
const char FB_SECRET[] = "H8faDHZoKN2657IZLqz3O7ARvnx2IDKZkWI7oC22";

const char DEVICE_NAME[] = "yasjao_esp32"

// ------------- * VARIABLES * -------------

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

int flag = 0;

// ------------- * FUNCTIONS * -------------

void connectWiFi(const char* ssid, const char* password)
{
    Serial.print("Conectando");
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.write('.');
        delay(500);
    }

    Serial.print("\nConectado com o IP: ");
    Serial.println(WiFi.localIP());
}

void sendData()
{
    FirebaseJson json;
    json.set("temperature", temperature);
    json.set("moisture", moisture_perc);
    json.set("luminosity", luminosity);
    json.set("presence", presence);

    bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_00", json);
    if (!status)
    {
        Serial.print("Falha ao enviar ao Firebase: ");
        Serial.println(fbdo.errorReason().c_str());
    }

    if(presence == HIGH){
      flag++;
    }
}

void setup() 
{
  Serial.begin(115200);

  dht.begin();

  lcd.begin(16, 2);

  SerialBT.begin(DEVICE_NAME);

  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_MSENSOR, INPUT);
}

void loop() 
{
  lcd.clear();
  lcd.setCursor(6, 0);

  temperature = dht.readTemperature();
  luminosity = analogRead(PIN_LDR);
  presence = digitalRead(PIN_PIR);
  moisture = analogRead(PIN_MSENSOR);

  moisture_perc = map(moisture, 0, 4095, 0, 100);

  
}
