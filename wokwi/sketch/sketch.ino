#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ---------- Brochage (cf. table de câblage) ----------
#define DHTPIN     15   // DHT22 DATA
#define DHTTYPE    DHT22
#define POT_PIN    34   // Potentiomètre SIG (ADC, input only)
#define BUZZER_PIN 25   // Buzzer +
#define LED_PIN     2   // LED d'état via 220 Ω

// ---------- Réseau (simulateur Wokwi) ----------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";
const char* MQTT_HOST = "broker.hivemq.com";
const int   MQTT_PORT = 1883;

// Remplacez ETUDIANT par votre nom (sans espaces ni accents)
const char* TOPIC_TELE  = "esprim/smartvest/dupont/telemetry";
const char* TOPIC_ALERT = "esprim/smartvest/dupont/alert";
// ---------- Seuils d'alerte ----------
const float BPM_MAX  = 140.0;
const float TEMP_MIN = 35.0;
const float TEMP_MAX = 39.5;

// ---------- Objets ----------
DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
const unsigned long PERIOD_MS = 2000;   // publication toutes les 2 s

// Potentiomètre (0-4095) -> pseudo-fréquence cardiaque (40-200 bpm)
float readBpm() {
  int raw = analogRead(POT_PIN);
  return 40.0 + (raw / 4095.0) * 160.0;
}

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));   // clignotement pendant la connexion
    delay(300);
    Serial.print(".");
  }
  Serial.println(" connecté, IP = " + WiFi.localIP().toString());
}

void connectMqtt() {
  while (!mqtt.connected()) {
    String clientId = "smartvest-" + String(random(0xffff), HEX);
    Serial.print("MQTT...");
    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" OK");
    } else {
      Serial.print(" échec, code=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  connectWifi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWifi();
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();

  if (millis() - lastPublish < PERIOD_MS) return;
  lastPublish = millis();

  float bpm  = readBpm();
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Erreur lecture DHT22");
    return;
  }

  // ---------- Logique d'alerte ----------
  bool alert = false;
  String reason = "";
  if (bpm > BPM_MAX) { alert = true; reason += "pouls_eleve;"; }
  if (temp < TEMP_MIN || temp > TEMP_MAX) { alert = true; reason += "temperature_anormale;"; }

  digitalWrite(BUZZER_PIN, alert ? HIGH : LOW);
  digitalWrite(LED_PIN, alert ? LOW : HIGH);   // LED éteinte = alerte active

  // ---------- Télémétrie JSON ----------
  char payload[200];
  snprintf(payload, sizeof(payload),
           "{\"bpm\":%.1f,\"temp\":%.2f,\"hum\":%.2f,\"alert\":%s}",
           bpm, temp, hum, alert ? "true" : "false");
  mqtt.publish(TOPIC_TELE, payload);
  Serial.println(payload);

  // ---------- Alerte dédiée ----------
  if (alert) {
    char alertPayload[160];
    snprintf(alertPayload, sizeof(alertPayload),
             "{\"reason\":\"%s\",\"bpm\":%.1f,\"temp\":%.2f}",
             reason.c_str(), bpm, temp);
    mqtt.publish(TOPIC_ALERT, alertPayload);
    Serial.println("ALERTE : " + reason);
  }
}