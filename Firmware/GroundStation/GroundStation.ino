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

String buff;

const int csPin = 17;          // LoRa radio chip select
const int resetPin = 16;       // LoRa radio reset
const int irqPin = 4;         // change for your board; must be a hardware interrupt pin
 
long selectBand(int);
 
void setup() 
{     
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  
    // Sobrescribe los pines CS, reset, y IRQ 
  LoRa.setPins(csPin, resetPin, irqPin); // CS, reset, int pin

  if (!LoRa.begin(selectBand(channel))) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setTxPower(17); //Set the max transmition power
  LoRa.setSpreadingFactor(10); //Change the SF to get longer distances
}
 
void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(14,HIGH);
    // read packet
    while (LoRa.available()) {
      //Serial.print(LoRa.available());
      buff+=(char)LoRa.read();
      //Serial.write(LoRa.read());
    }
    buff+=",";
    buff+=String(LoRa.packetRssi());
    //if(buff.startsWith(ID)){
    Serial.println(buff);
      //    }
    buff="";

    digitalWrite(14,LOW);
  }
}

long selectBand(int a)
{    
  switch(a){ 
    case 0:
    return 903080000; //903.08Mhz
  break;
    case 1:
    return 905240000; //905.24
  break;
    case 2:
    return 907400000; //907.40
  break;
    case 3:
    return 909560000; //909.56
  break;
    case 4:
    return 911720000; //911.72
  break;
    case 5:
    return 913880000; //913.88
  break;
    case 6:
    return 916040000; //916.04
  break;
    case 7:
    return 918200000; // 918.20
  break;
    case 8:
    return 920360000; //920.36
  break;
    case 9:
    return 922520000; //922.52
  break;
    case 10:
    return 924680000; //924.68
  break;
    case 11:
    return 926840000; //926.84
  break;
    case 12:
    return 915000000; //915
  break;
  }
}
