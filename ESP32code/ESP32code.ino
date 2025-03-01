#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

#define WIFI_SSID "anesb"
#define WIFI_PASSWORD "1234567890"

#define FIREBASE_HOST "https://iot-temp-75625-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "pJq2w188nVMTtBvCmnVRlYkSAjwdB6VXCcFzSX1Q"

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

#define DHTPIN 4  // Pin na koji je povezan DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Povezivanje na WiFi...");

    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ Povezano!");

    // Konfiguracija Firebase-a
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    dht.begin();
}

void loop() {
    float temperature = dht.readTemperature();
    

    if (isnan(temperature)) {
        Serial.println("⚠️ Greška pri čitanju temperature!");
        return;
    }

    Serial.print("Temperatura: ");
    Serial.println(temperature);

    // Ispravan način za slanje temperature u Firebase
    if (Firebase.setFloat(firebaseData, "/temperatura", temperature)) {
        Serial.println("✅ Podatak uspešno poslat!");
    } else {
        Serial.print("❌ Greška pri slanju: ");
        Serial.println(firebaseData.errorReason());
    }

    delay(5000);
}