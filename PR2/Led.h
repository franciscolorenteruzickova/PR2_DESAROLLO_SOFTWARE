#ifndef LED_H
#define LED_H

#include <Arduino.h>

struct RGB{
  const int green_pin;
  const int red_pin;
};

const RGB cinta_uno = {38, 39};
const RGB cinta_dos = {40, 41};

void led_init();

#endif