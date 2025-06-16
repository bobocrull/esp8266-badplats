#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <time.h>

// WiFi-inställningar
const char* ssid = "DittSSID";
const char* password = "DittLosenord";

ESP8266WebServer server(80);

void handleFileRequest(const String& path) {
  if (!SPIFFS.exists(path)) {
    server.send(404, "text/plain", "Fil saknas");
    return;
  }

  File file = SPIFFS.open(path, "r");
  if (file) {
    String contentType = "text/plain";
    if (path.endsWith(".html")) contentType = "text/html";
    else if (path.endsWith(".json")) contentType = "application/json";
    server.streamFile(file, contentType);
    file.close();
  }
}

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float readTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void appendTemperatureToJson() {
  File file = SPIFFS.open("/data.json", "r");
  DynamicJsonDocument doc(2048);

  if (file) {
    DeserializationError err = deserializeJson(doc, file);
    file.close();
    if (err) {
      Serial.println("Fel vid läsning av JSON, skapar ny fil.");
      doc.clear();
    }
  }

  // Skapa nytt temperaturobjekt
  JsonObject newEntry = doc.createNestedObject();
  time_t now = time(nullptr);
  struct tm *tm_info = localtime(&now);
  char date[11], timeStr[6];
  strftime(date, sizeof(date), "%Y-%m-%d", tm_info);
  strftime(timeStr, sizeof(timeStr), "%H:%M", tm_info);

  newEntry["date"] = date;
  newEntry["time"] = timeStr;
  newEntry["temperature"] = readTemperature();

  // Håll bara de senaste 14 posterna
  while (doc.size() > 14) {
    doc.remove(0);
  }

  // Spara till fil
  file = SPIFFS.open("/data.json", "w");
  if (file) {
    serializeJsonPretty(doc, file);
    file.close();
    Serial.println("Temperatur sparad till data.json");
  } else {
    Serial.println("Kunde inte skriva till data.json");
  }
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  sensors.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi ansluten. IP: " + WiFi.localIP().toString());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  delay(2000);

  if (MDNS.begin("badplats")) {
    Serial.println("mDNS startad: http://badplats.local");
  }

  server.on("/", []() { handleFileRequest("/index.html"); });
  server.on("/data.json", []() { handleFileRequest("/data.json"); });
  server.begin();

  appendTemperatureToJson(); // Kör vid uppstart
}

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 60UL * 60UL * 1000UL; // 1 gång/timme

void loop() {
  server.handleClient();

  if (millis() - lastUpdate > updateInterval) {
    appendTemperatureToJson();
    lastUpdate = millis();
  }
}
