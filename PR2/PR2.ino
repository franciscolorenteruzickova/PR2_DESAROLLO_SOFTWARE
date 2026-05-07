#include"KeyBoard.h"
#include"Screen.h"
#include"Sensor.h"
#include"Button.h"
#include"Led.h"

void setup(){
  screen_init();
  sensor_init();
  button_init();
  led_init();
}

void loop(){

  
  delay(500);
}
