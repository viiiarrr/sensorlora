#include "read_rain.h"

#define RAIN_PIN 6

// Atribut penahan data saat Deep Sleep
RTC_DATA_ATTR volatile int hitunganJungkit = 0;

volatile unsigned long waktuJungkitTerakhir = 0;
const unsigned long jedaBouncing = 200;

void IRAM_ATTR sensorHujanTerpicu() {
    unsigned long waktuSekarang = millis();
    // Logika debouncing
    if (waktuSekarang - waktuJungkitTerakhir > jedaBouncing) {
        hitunganJungkit++;
        waktuJungkitTerakhir = waktuSekarang;
    }
}

void setupRain() {
    pinMode(RAIN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RAIN_PIN), sensorHujanTerpicu, FALLING);
}

int getRainTips() {
    return hitunganJungkit;
}