#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// ============================================================
//  Pin mapping Heltec WiFi LoRa 32 V3 (ESP32-S3)
//  SPI LoRa (FSPI) : SCK=9, MISO=11, MOSI=10
//  SX1262          : NSS=8, DIO1=14, RESET=12, BUSY=13
// ============================================================
#define DIO1_PIN 14   // DIO1 = HIGH saat paket diterima

SPIClass LoRaSPI(FSPI);
SX1262 radio = new Module(8, DIO1_PIN, 12, 13, LoRaSPI);

unsigned long lastPrintTime = 0;
int waitDots = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=================================");
    Serial.println("  Heltec WiFi LoRa 32 V3 (RX)  ");
    Serial.println("    LoRa RX - Node Penerima     ");
    Serial.println("=================================");

    LoRaSPI.begin(9, 11, 10, 8);

    Serial.print("[LoRa] Initializing... ");
    int state = radio.begin(915.0);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("SUCCESS");
    } else {
        Serial.print("FAILED, code = ");
        Serial.println(state);
        while (true) { delay(500); }
    }

    // Mulai mode receive — DIO1 akan HIGH saat paket masuk
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("[LoRa] Mendengarkan... (DIO1 polling)");
        Serial.println("---------------------------------");
    } else {
        Serial.print("[LoRa] startReceive gagal, code = ");
        Serial.println(state);
        while (true) { delay(500); }
    }
}

void loop() {
    // Tampilkan "Menunggu..." tiap 3 detik supaya tahu board aktif
    if (millis() - lastPrintTime >= 3000) {
        lastPrintTime = millis();
        waitDots = (waitDots % 3) + 1;
        Serial.print("[RX] Menunggu paket");
        for (int i = 0; i < waitDots; i++) Serial.print(".");
        Serial.println();
    }

    // Cek DIO1 — HIGH berarti SX1262 sudah terima paket
    if (digitalRead(DIO1_PIN) == HIGH) {
        String received = "";
        int state = radio.readData(received);

        if (state == RADIOLIB_ERR_NONE) {
            Serial.println();
            Serial.println("========== PAKET DITERIMA ==========");
            Serial.print("[RX] Pesan   : ");
            Serial.println(received);
            Serial.print("[RX] RSSI    : ");
            Serial.print(radio.getRSSI());
            Serial.println(" dBm");
            Serial.print("[RX] SNR     : ");
            Serial.print(radio.getSNR());
            Serial.println(" dB");
            Serial.println("====================================");
            lastPrintTime = millis();
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            Serial.println("[RX] CRC Error - paket rusak!");
        } else {
            Serial.print("[RX] readData error, code = ");
            Serial.println(state);
        }

        // Restart receive untuk paket berikutnya
        radio.startReceive();
    }
}

