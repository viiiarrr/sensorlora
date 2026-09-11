#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

bool initLoRa();

bool sendLoRa(String &message);

#endif