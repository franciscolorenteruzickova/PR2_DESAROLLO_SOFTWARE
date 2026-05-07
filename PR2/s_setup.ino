#include"KeyBoard.h"
#include"Screen.h"
#include"Sensor.h"
#include"Button.h"
#include"Led.h"

void on_setup(){
  screen_init();
  sensor_init();
  button_init();
  led_init();

  xTaskCreate(tarea_lcd, "LCD", 6144, NULL, 1, NULL);
  xTaskCreate(tarea_sensor, "sensor", 6144, NULL, 1, NULL);
  xTaskCreate(tarea_interrupcion, "interrupcion", 6144, NULL, 1, NULL);
}