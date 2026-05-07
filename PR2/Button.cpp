#include"Button.h"

bool PARAR = false;

void button_init(){
  attachInterrupt(button_pin, isr_boton, FALLING);
}

void IRAM_ATTR isr_boton(){
  PARAR = true;
}