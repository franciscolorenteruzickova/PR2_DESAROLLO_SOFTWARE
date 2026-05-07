#ifndef KEYBOARD_H
#define KEYBOARD_H

#include<Keypad.h>

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte GPIORow[4] = {14, 13, 12, 11};
byte GPIOColumn[4] = {10, 9, 46, 3};

const char password[16] = "0123456789";

Keypad KeyBoard = Keypad(makeKeymap(keys), GPIORow, GPIOColumn, 4, 4);

/*
char key = KeyBoard.getKey();
  if(key){
    Serial.print(key);
  }
*/

#endif