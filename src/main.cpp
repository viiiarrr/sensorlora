#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// ============================================================
//  Pin mapping untuk Heltec Wireless Stick Lite V3 (ESP32-S3)
//  SPI LoRa (FSPI) : SCK=9, MISO=11, MOSI=10
//  SX1262          : NSS=8, DIO1=14, RESET=12, BUSY=13
// ============================================================
SPIClass LoRaSPI(FSPI);

SX1262 radio = new Module(
    8,       // NSS  (CS)
    14,      // DIO1
    12,      // RESET
    13,      // BUSY
    LoRaSPI  // SPI instance
);

int counter = 0;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=================================");
    Serial.println(" Heltec Wireless Stick Lite V3  ");
    Serial.println("   LoRa TX Test (Single Node)   ");
    Serial.println("=================================");

    // Inisialisasi SPI LoRa dengan pin yang benar
    LoRaSPI.begin(
        9,   // SCK
        11,  // MISO
        10,  // MOSI
        8    // SS  (opsional, RadioLib yang kontrol NSS)
    );

    Serial.print("[LoRa] Initializing... ");

    // radio.begin(freq, bw, sf, cr, syncWord, power, preamble, tcxoVoltage)
    // Gunakan 915.0 untuk Asia/Indonesia, atau 868.0 untuk Eropa
    int state = radio.begin(915.0);

    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("SUCCESS");
    }
    else
    {
        Serial.print("FAILED, code = ");
        Serial.println(state);
        Serial.println(">> Cek koneksi SPI / pin mapping!");
        while (true) { delay(500); }
    }

    // Output power maksimum SX1262 = 22 dBm
    radio.setOutputPower(22);

    Serial.println("[LoRa] Node siap mengirim pesan.");
    Serial.println("---------------------------------");
}

void loop()
{
    counter++;
    String msg = "Hello LoRa #" + String(counter);

    Serial.print("[TX] Mengirim: ");
    Serial.print(msg);
    Serial.print(" ... ");

    int state = radio.transmit(msg);

    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("OK");
    }
    else
    {
        Serial.print("GAGAL, kode = ");
        Serial.println(state);
    }

    delay(3000);
}