/************************************************************
SPANISH
Ground station
Ground station - Estacion Terrena para CatSat
Eduardo Contreras @ Electronic Cats
Original Creation Date: Oct 8, 2018
https://github.com/ElectronicCats/CatSatZero

Este ejemplo demuestra el funcionamiento basico de la estacion terrena
la cual filtra los Satelites

http://electroniccats.com

Especificaciones del entorno de Desarrollo:
  IDE: Arduino 1.8.4
  Plataforma de Hardware:
  Estacion terrena CanSat
  - USB Stick CatWAN

Este código es beerware si tu me ves ( o cualquier otro miembro de Electronic Cats) 
a nivel local, y tu has encontrado nuestro código útil ,
por favor comprar una ronda de cervezas!

Distribuido como; no se da ninguna garantía.
************************************************************/

/************************************************************
ENGLISH
Ground Station
Ground Statios - Earth-Station for CatSat
Eduardo contreras @ Electronic Cats
Original Creation Date: Oct 8, 2018
https://github.com/ElectronicCats/CatSatZero

This example demonstrates how to use the earth-station for CatSat

Development environment specifics:
  IDE: Arduino 1.6.8
  Hardware Platform:
  Kit CanSat
  - USB Stick CatWAN
  

This code is beerware; if you see me (or any other Electronic Cats 
member) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.
**********************************************************
*IMPORTANTE CAMBIA TU ID DEPENDIENDO DE TU CANSAT         *
**********************************************************/

#include <SPI.h>
#include <LoRa.h>


/*******************************************************  
 *Selecciona un canal entre 0 y 12 este debe coincidir *
 *con el canal de tu satelite                          *
 *******************************************************/
int channel = 12;

String ID = "A1";

float chann;
String buff;

 
float selectBand(int);
 
void setup() 
{     
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(14,HIGH);
    // read packet
    while (LoRa.available()) {
      //Serial.print((char)LoRa.read());
      buff+=(char)LoRa.read();
    }
    buff+=",";
    buff+=String(LoRa.packetRssi());
    if(buff.startsWith(ID)){
          Serial.println(buff);
          }
    buff="";

    digitalWrite(14,LOW);
  }
}

float selectBand(int a)
{    
  switch(a){ 
    case 0:
    return 903.08;
  break;
    case 1:
    return 905.24;
  break;
    case 2:
    return 907.40;
  break;
    case 3:
    return 909.56;
  break;
    case 4:
    return 911.72;
  break;
    case 5:
    return 913.88;
  break;
    case 6:
    return 916.04;
  break;
    case 7:
    return 918.20;
  break;
    case 8:
    return 920.36;
  break;
    case 9:
    return 922.52;
  break;
    case 10:
    return 924.68;
  break;
    case 11:
    return 926.84;
  break;
    case 12:
    return 915;
  break;
  }
  
 }

