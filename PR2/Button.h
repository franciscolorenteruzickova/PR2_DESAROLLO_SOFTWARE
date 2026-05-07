#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

const int button_pin = 42;

void button_init();
void IRAM_ATTR isr_boton();


#endif