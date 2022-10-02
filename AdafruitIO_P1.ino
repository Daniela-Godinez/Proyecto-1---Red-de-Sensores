// Adafruit IO Group Publish Example
//
  // Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.

#include <SPI.h>
#include "config.h"

/************************ Example Starts Here *******************************/

const int var_slave = 5;
uint8_t seg;
uint8_t color;
unsigned long time_millis = 0; // ME SIRVE PAR ALO DE MILLIS

// CONFIGURAMOS EL SPI:
SPISettings settings(4000000, MSBFIRST, SPI_MODE1);

// SET EL GROUP Y FEEDS:
AdafruitIO_Group *PROYECTO_01 = io.group("proyecto-01");


void setup() {

  // UART
  Serial.begin(115200);

  // WAIT PARA INICIAL
  while(! Serial);

  // CONECTANDO a io.adafruit.com:
  Serial.print("Connecting to Adafruit IO...");
  io.connect();

  // WAIT A ESTABLIZIAR LA CONEXIÓN:
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // MENSAJE APRA SABER SI SE CONCRETÓ LA CONEXIÓN:
  Serial.println();
  Serial.println(io.statusText());
  
  // PINES PARA LO DEL SPI:
  SPI.begin(); 
  
  // SAVE EL PRIMER VALOR DE MILLIS
  time_millis = millis(); 
  
  // DEFINIMOS EL 'CS' COMO OUTPUT:
  pinMode (var_slave, OUTPUT); 
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run(); //En cada repetición se mantiene la conexión con ADAFRUIT IO

  // INICIO EL PROCESO DEL SPI:
  SPI.beginTransaction(settings);
  
  //SLAVE SELECT: ON
  digitalWrite (var_slave, HIGH); 
  delay(10);
  
  //SLAVE SELECT: OFF
  digitalWrite (var_slave, LOW);
  delay(10);
  seg = SPI.transfer(0);
  delay(10);
  
  //SLAVE SELECT: OFF
  digitalWrite (var_slave, HIGH); 
  delay(10);
  //SLAVE SELECT: ON
  digitalWrite (var_slave, LOW); 
  delay(10);
  
  color = SPI.transfer(1); // cuando 'pedir' = 1
  delay(10);
  
  //SLAVE SELECT: OFF
  digitalWrite (var_slave, HIGH);
  delay(10);
  digitalWrite (var_slave, LOW); //SS activo
  delay(10);
  color = SPI.transfer(2); //Req = 2
  delay(10);
  // SLAVE SELECT ACTIVO/DESACTIVO:
  digitalWrite (var_slave, HIGH); 
  delay(10);
  digitalWrite (var_slave, LOW); 
  delay(10);
  // CUANDO PEDIR " 3:
  color = SPI.transfer(3); 
  delay(10);

  digitalWrite (var_slave, HIGH); 
  delay(10);
  digitalWrite (var_slave, LOW); 
  delay(10);
  
  SPI.endTransaction();
  
  if (millis() > (time_millis +1000))
  { // ACTUALIZO CADA 2.5s:
    time_millis = millis();
    // SEND DATA, CUANDO COLOR RECIBIDO: ROJO
    PROYECTO_01->set("proyecto-01.rbg-red", color); 
    // SEND DATA, CUANDO COLOR RECIBIDO: AZUL
    PROYECTO_01->set("proyecto-01.rgb-blue", color);
    // SEND DATA, CUANDO COLOR RECIBIDO: VERDE
    PROYECTO_01->set("proyecto-01.rgb-green", color); 
    // SEND DATA: SEGUNDOS DE FUNCIONAMIENTO DEL RTC
    PROYECTO_01->set("proyecto-01.rtc", seg);
    // SAVE ALL FEEDS:
    PROYECTO_01->save();
  }
  // MUESTRO EN EL SERIAL:
  Serial.print("Color percibido: ");
  Serial.println(color);
  Serial.print("Tiempo de funcionamento: ");
  Serial.print(seg);

}
