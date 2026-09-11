#include "read_imu.h"

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SDA_PIN 41
#define SCL_PIN 42

Adafruit_MPU6050 mpu;

bool initIMU()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    if(!mpu.begin())
    {
        Serial.println("[IMU] MPU6050 NOT FOUND");
        return false;
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("[IMU] READY");

    return true;
}

IMUData readIMU()
{
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    IMUData data;

    mpu.getEvent(&accel, &gyro, &temp);

    data.ax = accel.acceleration.x;
    data.ay = accel.acceleration.y;
    data.az = accel.acceleration.z;

    data.gx = gyro.gyro.x;
    data.gy = gyro.gyro.y;
    data.gz = gyro.gyro.z;

    data.temperature = temp.temperature;

    return data;
}