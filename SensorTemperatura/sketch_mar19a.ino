#include <HTTPClient.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 23
#define DHTTYPE DHT22

float h=0.0, t=0.0, f=0.0;

//Credenciales de acceso a WiFi
const char* ssid = "Red-Paolo";
const char* cont = "12345678";
DHT dht (DHTPIN, DHTTYPE);
TaskHandle_t Task1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  crearTareas();
  dht.begin();
  conexionWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("La humedad relativa es: ");
  Serial.println(h);
  Serial.print("La temperatura relativa es °C es: ");
  Serial.println(t);
  Serial.print("La temperatura relativa en °F es: ");
  Serial.println(f);
  vTaskDelay( pdMS_TO_TICKS(2000) );

}
void leerSensor(void * parameter){
  for(;;){
    delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);
  }
}

void crearTareas(){
  xTaskCreatePinnedToCore(
    leerSensor, //Función que implementa la tarea
    "Task1", //Nombre de la tarea
    1000, //Tamaño de Bytes en memoria (pila de memoria)
    NULL, //Parámetros de entrada de la función
    0, //Prioridad de la tarea en el programa
    &Task1, //Puntero en memoria
    0); //Núcleo en el que se ejecuta la tarea
}

void conexionWiFi(){
  Serial.println("WiFi Desconectado...");
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid,cont);
    Serial.print(".");
    delay(500);
  }
  Serial.println(".");
}
