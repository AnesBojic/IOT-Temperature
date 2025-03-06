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

#define LED_BUILTIN 2  // Integrisana LED dioda na ESP32

float donja = 0.0;
float gornja = 0.0;

void setup() {
    Serial.begin(115200);
    
    pinMode(LED_BUILTIN, OUTPUT);  // Postavljamo integrisanu LED diodu kao izlaz

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
    // Preuzimanje donje i gornje granice sa Firebase-a (putanje u okviru /postavke)
    if (Firebase.getFloat(firebaseData, "/postavke/donja")) {
        donja = firebaseData.floatData();
        Serial.print("Donja granica: ");
        Serial.println(donja);
    } else {
        Serial.print("❌ Greška pri preuzimanju donje granice: ");
        Serial.println(firebaseData.errorReason());
    }

    if (Firebase.getFloat(firebaseData, "/postavke/gornja")) {
        gornja = firebaseData.floatData();
        Serial.print("Gornja granica: ");
        Serial.println(gornja);
    } else {
        Serial.print("❌ Greška pri preuzimanju gornje granice: ");
        Serial.println(firebaseData.errorReason());
    }

    // Fiksna vrednost temperature (simulacija)
    //float temperature = 13.5;
    //float temperature = random(150, 300) / 10.0;

    //Uzimanje podataka iz DHT11 senzora
    float temperature = dht.readTemperature();

    if (isnan(temperature)) {
        Serial.println("⚠️ Greška pri čitanju temperature!");
        return;
    }

    Serial.print("Temperatura: ");
    Serial.println(temperature);

    // Slanje trenutne temperature u Firebase
    if (Firebase.setFloat(firebaseData, "/temperatura", temperature)) {
        Serial.println("✅ Temperatura uspešno poslata u Firebase!");
    } else {
        Serial.print("❌ Greška pri slanju temperature: ");
        Serial.println(firebaseData.errorReason());
    }

    // Provjera da li je temperatura unutar granica
    if (temperature >= donja && temperature <= gornja) {
        digitalWrite(LED_BUILTIN, HIGH); // Paljenje LED diode
        Serial.println("✅ Temperatura je unutar granica, LED je upaljena.");
    } else {
        digitalWrite(LED_BUILTIN, LOW); // Gašenje LED diode
        Serial.println("❌ Temperatura je van granica, LED je ugašena.");
    }

    delay(3000);  // Čekanje 3 sekunde pre sledećeg merenja
}