#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 23
#define DHTTYPE DHT22

float h = 0.0, t = 0.0, f = 0.0;

//Credenciales de acceso a WiFi
const char* ssid = "117496";  // Tu SSID de WiFi
const char* cont = "11749612";  // Tu contraseña de WiFi

//Dirección HTTP de Thingspeak
const char* nombreServidor = "http://api.thingspeak.com/update?api_key=HW3NYD1CU40TWXC3";

DHT dht(DHTPIN, DHTTYPE);
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  Serial.begin(9600);
  dht.begin();
  conexionWiFi();
  crearTareas();
}

void loop() {
  // El loop ya no tiene código relevante porque las tareas gestionan todo
}

void leerSensor(void *parameter) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(4000));  // Lectura cada 4 segundos
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);  // Fahrenheit
    // Verifica si la lectura es válida, si no, ignora la lectura
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Error al leer del sensor DHT");
      continue;
    }
  }
}

// Envío de datos por POST HTTP
void posteoDatos(void *parameter) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      respuestaHTTP();
      vTaskDelay(pdMS_TO_TICKS(15000));  // Envío cada 15 segundos
    } else {
      conexionWiFi();
    }
  }
}

void respuestaHTTP() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, nombreServidor);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Información a enviar al servidor
  String httpResp = "&field1=" + String(h) + "&field2=" + String(t) + "&field3=" + String(f);
  int httpCodigo = http.POST(httpResp);

  // Se valida la respuesta
  if (httpCodigo == 200) {
    String cuerpo_respuesta = http.getString();
    Serial.print("El servidor respondió: ");
    Serial.println(cuerpo_respuesta);
  } else {
    Serial.print("Código de respuesta HTTP: ");
    Serial.println(httpCodigo);
  }
  http.end();  // Se liberan recursos del cliente HTTP
}

void crearTareas() {
  xTaskCreatePinnedToCore(
    leerSensor,    // Función que implementa la tarea
    "Task1",       // Nombre de la tarea
    1000,          // Tamaño de Bytes en memoria (pila de memoria)
    NULL,          // Parámetros de entrada de la función
    0,             // Prioridad de la tarea en el programa
    &Task1,        // Puntero en memoria
    0);            // Núcleo en el que se ejecuta la tarea

  xTaskCreatePinnedToCore(
    posteoDatos,   // Función que implementa la tarea
    "Task2",       // Nombre de la tarea
    3000,          // Tamaño de Bytes en memoria (pila de memoria)
    NULL,          // Parámetros de entrada de la función
    1,             // Prioridad de la tarea en el programa
    &Task2,        // Puntero en memoria
    1);            // Núcleo en el que se ejecuta la tarea
}

void conexionWiFi() {
  Serial.println("WiFi Desconectado...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, cont);
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado a WiFi");
}
