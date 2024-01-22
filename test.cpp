#include <Arduino.h>
#include "FirebaseESP32.h"
#include <WiFi.h>

const char WIFI_SSID[] = "belisarius";
const char WIFI_PASSWORD[] = "senhaaaa";

const char FB_HOST[] = "https://plangotchi-default-rtdb.firebaseio.com/";
const char FB_SECRET[] = "H8faDHZoKN2657IZLqz3O7ARvnx2IDKZkWI7oC22";

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

int humor_data;
int health_data;
int thirst_data;
int life_data;
int status_data;

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

// void sendData()
// {

//   FirebaseJson json;
//   json.set("temperature", 1);
//   json.set("luminosity", 1);
//   json.set("moisture", 1);
//   json.set("presence", 1);
//   json.set("flag", 1);

//   bool status = Firebase.updateNode(fbdo, "/sensors", json);

//   if (!status)
//   {
//       Serial.print("Failed to send data to Firebase: ");
//       Serial.println(fbdo.errorReason().c_str());
//   }
//   else
//   {
//     Serial.println("Data sent with success!");
//     flag++;
//   }
// }

void setup(){
    Serial.begin(115200);
    connectWiFi(WIFI_SSID, WIFI_PASSWORD);

    fbconfig.database_url = FB_HOST;
    fbconfig.signer.tokens.legacy_token = FB_SECRET;
    fbdo.setBSSLBufferSize(4096, 1024);
    Firebase.reconnectWiFi(true);
    Firebase.begin(&fbconfig, &fbauth);
}

void getData(){
  if (Firebase.getInt(fbdo, "/stats/humor")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        humor_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/health")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        health_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/thirst")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        thirst_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/life")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        life_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, "/stats/status")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        status_data = fbdo.to<int>();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }
}

void loop(){
    // sendData();
    getData();

    Serial.println(health_data)
    Serial.println(thirst_data)
    Serial.println(life_data)
    Serial.println(status_data)
}