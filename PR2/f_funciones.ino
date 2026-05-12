

static void tarea_lcd(void *pv){

  for(;;){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(current_state.line_one);
    lcd.setCursor(0, 1);
    lcd.print(current_state.line_two);

    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

static void tarea_sensor(void *pv){

  for(;;){
    if(!PARAR){
      if(distance()<20){
        current_state = box_allert;
      }
      else{
        current_state = idle;
      }
    }

    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

static void tarea_interrupcion(void *pv){
  int i = 0;
  for(;;){
    if(PARAR){
      current_state = superuser;
      char key = KeyBoard.getKey();
      if(key){
        superuser.line_two[i] = key; 
        ++i;
      }
      if(i == 10){
        if(strcmp(password, superuser.line_two) == 0){
          current_state = password_correct;
          enviarMensajePorTopic(TOPIC_PUB, "Parar simulación");
          delay(6000);
        }
        else{
          current_state = password_incorrect;
          delay(6000);
        }
        strcpy(superuser.line_two, "          ");
        current_state = idle;
        i = 0;
        PARAR = false;
      }
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}