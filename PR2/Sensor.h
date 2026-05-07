#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

const int Trigger = 2;
const int Echo = 1;

void sensor_init();
float distance();

#endif