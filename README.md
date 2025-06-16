# 🌊 ESP8266 Väderstation – Badplats

Detta projekt är en enkel, batteridriven väderstation som mäter **vattentemperatur** vid en badplats. Enheten bygger på en **D1 Mini (ESP8266)** och visar temperatur både som **aktuellt värde** och **historik för 14 dagar** via en lokal webbsida på samma nätverk.

## ⚙️ Funktioner

- Mäter vattentemperatur med DS18B20
- Visar senaste mätning + batterinivå (om implementerat)
- Sparar temperaturhistorik lokalt (senaste 14 dagarna)
- Visar allt på en lokal webbsida via ESP8266
- Kräver inget internet – fungerar **offline inom WiFi**

## 🗂 Filstruktur

```
esp8266-badplats/
├── main.ino                # Arduino-kod som kör ESP8266-webservern och SPIFFS
└── data/
    ├── index.html          # Webbsidan som visas på http://badplats.local
    └── data.json           # Temperaturhistorik (14 dagar)
```

## 🌐 Användning

När enheten är igång och ansluten till WiFi:
- Besök `http://badplats.local` från en enhet på samma nätverk
- Där visas aktuell temperatur, senaste mätning och en graf över 14 dagar

## 🔧 Installation

1. Ladda upp `main.ino` till din D1 Mini via Arduino IDE
2. Lägg `index.html` och `data.json` i mappen `data/`
3. Använd **"ESP8266 Sketch Data Upload"** (via Tools-menyn) för att ladda upp SPIFFS-innehållet
4. Besök sedan `http://badplats.local` i webbläsaren

> 🧩 Kräver bibliotek: `ESP8266WiFi`, `ESP8266WebServer`, `ESP8266mDNS`, `FS`

## 🧪 Exempeldata

Filen `data.json` innehåller dummyvärden för test – du kan uppdatera innehållet dynamiskt i framtiden för att spara riktiga mätningar.

## 🧱 Komponenter

- ESP8266 (D1 Mini)
- DS18B20 vattentät temperatursensor
- 4.7kΩ resistor
- 18650 Li-Ion-batteri + batterihållare
- Laddmodul (t.ex. TP4056)
- Plastlåda med hål för sensorn
