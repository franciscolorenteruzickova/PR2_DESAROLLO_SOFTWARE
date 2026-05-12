#include"Screen.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

screen_message superuser = {"Introduce clave:", ""};
screen_message current_state = {"", ""};

void screen_init(){
  Wire.begin(21, 20);
  lcd.init();
  lcd.backlight();
  current_state = idle;
}