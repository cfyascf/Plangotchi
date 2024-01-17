#include "FirebaseESP32.h"
#include <WiFi.h>

const char WIFI_SSID[] = "yasses redmi";
const char WIFI_PASSWORD[] = "yasmi123456";

const char FB_HOST[] = "https://plangotchi-default-rtdb.firebaseio.com/";
const char FB_SECRET[] = "H8faDHZoKN2657IZLqz3O7ARvnx2IDKZkWI7oC22";

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

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

void setup(){
    Serial.begin(115200);
    connectWiFi(WIFI_SSID, WIFI_PASSWORD);

    fbconfig.database_url = FB_HOST;
    fbconfig.signer.tokens.legacy_token = FB_KEY;
    fbdo.setBSSLBufferSize(4096, 1024);
    Firebase.reconnectWiFi(true);
    Firebase.begin(&fbconfig, &fbauth);
}

void loop(){
    FirebaseJson json;
    json.set("temperature", 1);
    json.set("luminosity", 1);

    bool status = Firebase.updateNode(fbdo, "/sensors", json);

    if (!status)
    {
        Serial.print("Failed to send data to Firebase: ");
        Serial.println(fbdo.errorReason().c_str());
    }
}