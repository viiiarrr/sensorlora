#include "read_soil.h"

#include <Arduino.h>

#define SOIL_PIN 1

// ===========================
// need kalibrasi
// ===========================
const int ADC_DRY = 3200;
const int ADC_WET = 1500;

bool initSoil()
{
    pinMode(SOIL_PIN, INPUT);

    Serial.println("[SOIL] READY");

    return true;
}

int readSoilADC()
{
    return analogRead(SOIL_PIN);
}

float readSoilPercent()
{
    int adc = readSoilADC();

    float moisture = map(adc,
                         ADC_DRY,
                         ADC_WET,
                         0,
                         100);

    moisture = constrain(moisture, 0, 100);

    return moisture;
}