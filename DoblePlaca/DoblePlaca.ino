#define pin 2
String clave = "BENI", lec = " ";

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  crearTareas();
  pinMode(pin,OUTPUT);
  Serial.begin(9600);
  vTaskDelay(pdMS_TO_TICKS(2000) );
  Serial.println("Se inició la tarjeta");
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(pdMS_TO_TICKS(10) );
}

void tareaUno(void * parameter){
  for(;;){
    Serial.println("Escribe la palabra clave: ");
    while(true){
      if(Serial.available()){
        lec = Serial.readString();
        lec.trim();
        lec.toUpperCase();
        
        break;
      }else{
        
      }
    }
    while(!Serial.available());
    vTaskDelay(pdMS_TO_TICKS(10) );
  }
}

void tareaDos(void * parameter){
  for(;;){
    if(lec == clave){
      digitalWrite(pin,1);
      vTaskDelay(pdMS_TO_TICKS(1000) );
      digitalWrite(pin,0);
      vTaskDelay(pdMS_TO_TICKS(1000) );
      lec = " ";
    }
    else{
      digitalWrite(pin,0);
    }
    
  } 
}

void crearTareas(){
  xTaskCreatePinnedToCore(
    tareaUno, //Función que implementa la tarea
    "Task1", //Nombre de la tarea
    1000, //Tamaño de Bytes en memoria (pila de memoria)
    NULL, //Parámetros de entrada de la función
    0, //Prioridad de la tarea en el programa
    &Task1, //Puntero en memoria
    0); //Núcleo en el que se ejecuta la tarea

  xTaskCreatePinnedToCore(
    tareaDos, //Función que implementa la tarea
    "Task2", //Nombre de la tarea
    1000, //Tamaño de Bytes en memoria (pila de memoria)
    NULL, //Parámetros de entrada de la función
    1, //Prioridad de la tarea en el programa
    &Task1, //Puntero en memoria
    1); //Núcleo en el que se ejecuta la tarea
}
