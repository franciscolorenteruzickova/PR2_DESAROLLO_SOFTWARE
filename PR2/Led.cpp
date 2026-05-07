#include"Led.h"

void led_init(){
  pinMode(cinta_uno.green_pin, OUTPUT);
  pinMode(cinta_uno.red_pin, OUTPUT);
  pinMode(cinta_dos.green_pin, OUTPUT);
  pinMode(cinta_dos.red_pin, OUTPUT);
}