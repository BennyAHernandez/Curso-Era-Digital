#include "BluetoothSerial.h"
#define pin 2
String dispbt = "Benny"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);
  SerialBT.begin(dispbt);
  // put your setup code here, to run once:
  pinMode(pin,OUTPUT);

}

void loop() {
  if(SerialBT.available(){
    Serial.write(SerialBT,read());
  }
  if(Serial.available()){
    SerialBT.write(Serial.read());
  }
}
