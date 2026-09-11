#include <Arduino.h>

#include "comm.h"
#include "read_imu.h"

uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" HELTEC SENSOR NODE");
    Serial.println("=================================");

    if(!initLoRa())
    {
        while(true)
        {
            delay(1000);
        }
    }

    if(!initIMU())
    {
        while(true)
        {
            delay(1000);
        }
    }

    Serial.println();
    Serial.println("System Ready");
    Serial.println();
}

void loop()
{
    counter++;

    IMUData imu = readIMU();

    Serial.println("--------------------------------");

    Serial.print("AX : ");
    Serial.println(imu.ax);

    Serial.print("AY : ");
    Serial.println(imu.ay);

    Serial.print("AZ : ");
    Serial.println(imu.az);

    Serial.print("GX : ");
    Serial.println(imu.gx);

    Serial.print("GY : ");
    Serial.println(imu.gy);

    Serial.print("GZ : ");
    Serial.println(imu.gz);

    Serial.print("TEMP : ");
    Serial.println(imu.temperature);

    String payload =
        String(counter) + "," +
        String(imu.ax,2) + "," +
        String(imu.ay,2) + "," +
        String(imu.az,2) + "," +
        String(imu.gx,2) + "," +
        String(imu.gy,2) + "," +
        String(imu.gz,2);

    sendLoRa(payload);

    delay(3000);
}