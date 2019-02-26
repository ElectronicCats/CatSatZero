/************************************************************
SPANISH
CatSat Zero
CatSat Zero - Satelite en Lata Educativo
Andres Sabas @ Electronic Cats
Eduardo Contreras @ Electronic Cats
Original Creation Date: Ago 14, 2018
https://github.com/ElectronicsCats/CatSatZero/

Este ejemplos demuestra el funcionamiento basico de los sensores y funcionalidad
basica del CatSatZero Kit Educativo
http://electroniccats.com

Especificaciones del entorno de Desarrollo:
  IDE: Arduino 1.8.4
  Plataforma de Hardware:
  Kit CanSat
  - Arduino MKR1300
  - BMP280
  - CCS811
  - GPS L80


Este código es beerware si tu me ves ( o cualquier otro miembro de Electronic Cats) 
a nivel local, y tu has encontrado nuestro código útil ,
por favor comprar una ronda de cervezas!

Distribuido como; no se da ninguna garantía.
************************************************************/

/************************************************************
ENGLISH
CatSat Zero
CatSat - Satelite en Lata Educativo
Andres Sabas @ Electronic Cats
Eduardo Contreras @ Electronic Cats
Original Creation Date: Agu 14, 2018
https://github.com/ElectronicsCats/CatSatZero/

This example demonstrates how to use CatSat

Development environment specifics:
  IDE: Arduino 1.8.4
  Hardware Platform:
  Kit CanSat
  - Arduino MKR 1300
  - BMP280
  - CCS811
  - GPS L80

This code is beerware; if you see me (or any other Electronic Cats 
member) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.

Library Arduino LoRa
https://github.com/sandeepmistry/arduino-LoRa

*/
//Uncomment for debug
#define DEBUG
#define DEBUG2

#include <SPI.h>
#include <LoRa.h>

#include <NMEAGPS.h>
#include <GPSport.h>

#include <Wire.h>
#include <SparkFunBME280.h>

#include <Adafruit_CCS811.h>

#define CCS811_ADDR 0x5A //0x5B Alternate I2C Address

//Command activation Balloon mode
#define PMTK_SET_NMEA_886_PMTK_FR_MODE  "$PMTK886,3*2B"
// Command turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

//Global sensor objects
Adafruit_CCS811 ccs;
BME280 myBME280;

static NMEAGPS gps;
static gps_fix  fix;
static uint32_t last_rx = 0UL; // The last millis() time a character was
                               // received from GPS.

String Todo; //String a mandar

/************************************************************
*    IMPORTANTE CAMBIAR id_node DEPENDIENDO TU CANSAT      *
************************************************************/

String id_node= "A1"; 

/*******************************************************  
 *Selecciona un canal entre 0 y 12 este debe coincidir *
 *con el canal de tu satelite                          *
 *******************************************************/
int channel = 12;  

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

uint16_t voltage=0;

void enviarInfo(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  #ifdef DEBUG
  SerialUSB.println("Dato enviado");
  #endif
}

void gpsread(void){
  SerialUSB.print("Location: "); 
    if (fix.valid.location)
    { 
      Todo += String(fix.latitude(),6);
      Todo += ",";
      Todo += String(fix.longitude(),6);
      #ifdef DEBUG
      SerialUSB.print(fix.latitude(), 6);
      SerialUSB.print(F(","));
      SerialUSB.print(fix.longitude(), 6);
      #endif
     }
     else
     { 
        Todo += "0";
        Todo += ",";
        Todo += "0";
        #ifdef DEBUG
        SerialUSB.print("INVALID");
        #endif
      }
      #ifdef DEBUG
      SerialUSB.print("  Date/Time: ");
      if (fix.valid.date)
      {
        SerialUSB.print(fix.dateTime.month);
        SerialUSB.print("/");
        SerialUSB.print(fix.dateTime.day);
        SerialUSB.print("/");
        SerialUSB.print(fix.dateTime.year);
        SerialUSB.print(" ");
      }
      else
      {
        SerialUSB.print("INVALID ");
      }

      SerialUSB.print("");
      if (fix.valid.date)
      {
        if (fix.dateTime.hours < 10) SerialUSB.print("0");
        SerialUSB.print(fix.dateTime.hours);
        SerialUSB.print(":");
      if (fix.dateTime.minutes < 10) SerialUSB.print("0");
        SerialUSB.print(fix.dateTime.minutes);
        SerialUSB.print(":");
      if (fix.dateTime.seconds < 10) SerialUSB.print("0");
        SerialUSB.print(fix.dateTime.seconds);
        SerialUSB.print(".");
      }
      else
      {
        SerialUSB.print("INVALID ");
      }
      SerialUSB.println();
      #endif 

}

void ballonModeGPS(){
  Serial1.begin(9600);
  bool flag=0;
  String gpsData="";
  while(!flag){
       if ((Serial1.available())){ 
        while(Serial1.available()){
          char c = Serial1.read();  //Wait for a data on the GPS
          if(c='\n'){
            flag=1;
            Serial1.println(PMTK_SET_NMEA_886_PMTK_FR_MODE);
            while(Serial1.available()){
             gpsData+=(char)Serial1.read();
             }
             if(!gpsData.startsWith("$PMTK001,8"))flag=0;
             gpsData="";
           Serial1.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
          }
        }
       }
    }
   Serial.println("GPS balloon mode configured");  
  }

void setup() {
  SerialUSB.begin(9600);
  #ifdef DEBUG2
  while (!Serial);
  #endif 
  SerialUSB.println();
  SerialUSB.println("Settings...");
    
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
    
  pinMode(A6, OUTPUT); //Wakeup pin CCS811
  digitalWrite(A6,LOW);//Enable CCS811

  analogReference(AR_INTERNAL2V23);
  /*****LoRa init****/

  if (!LoRa.begin(selectBand(channel))) {           // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
      while (true);                       // if failed, do nothing
  }
  LoRa.enableCrc();
  LoRa.setTxPower(17); //Set the max transmition power
  LoRa.setSpreadingFactor(10); //Change the SF to get longer distances

  /******************/

  //This begins the CCS811 sensor and prints error status of .begin()
   if(!ccs.begin(CCS811_ADDR)){
    Serial.println("Failed to start sensor CCS811! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x76;
  
  //Initialize BME280
  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x76;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  int status280 = myBME280.begin();
  if (status280 != 0x60) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
     while (1);
   }

  ballonModeGPS();
  
  SerialUSB.println("CaTSat Zero Ready!");
}

void loop() {
  
  Todo += id_node;  //Add id to String 
  Todo += ",";

  while (gps.available( gpsPort )) {
    last_rx = millis();
    fix = gps.read();
    
    printInfoSerial();

    // read the input on analog pin battery 
    //NOTE: voltage max 1.18v in the pin of chip
    uint16_t sensorValue = analogRead(ADC_BATTERY);
    // Convert the analog reading (0 - 1.18v to 0 - 100%):
    voltage = sensorValue;
  
    gpsread();
    
    Serial.println(Todo);
    enviarInfo(Todo);
    for(int i = 0; i<5;i++){
      digitalWrite(LED_BUILTIN,HIGH); 
      delay(500);
      digitalWrite(LED_BUILTIN,LOW); 
      delay(500); 
      }
    }
  
   Todo = "";
   listenForSomething();
  // parse for a packet, and call onReceive with the result:
  //onReceive(LoRa.parsePacket());
  
}

void printInfoSerial()
{
    if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      #ifdef DEBUG
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      #endif
      Todo += ccs.geteCO2();
      Todo += ","; 
      Todo += ccs.getTVOC();
      Todo += ","; 
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }

  #ifdef DEBUG
  SerialUSB.println();
  SerialUSB.println("BME280 data:");
  SerialUSB.print(" Temperature: ");
  SerialUSB.print(myBME280.readTempC(), 2);
  SerialUSB.println(" degrees C");
  #endif
  Todo += myBME280.readTempC();
  Todo += ","; 

  #ifdef DEBUG
  SerialUSB.print(" Pressure: ");
  SerialUSB.print(myBME280.readFloatPressure(), 2);
  SerialUSB.println(" Pa");
  #endif
  Todo += myBME280.readFloatPressure();
  Todo += ",";  

  #ifdef DEBUG
  SerialUSB.print(" Altitude: ");
  SerialUSB.print(myBME280.readFloatAltitudeMeters(), 2);
  SerialUSB.println("m");
  #endif
  Todo += myBME280.readFloatAltitudeMeters();
  Todo += ","; 

  #ifdef DEBUG
  SerialUSB.print(" %RH: ");
  SerialUSB.print(myBME280.readFloatHumidity(), 2);
  SerialUSB.println(" %");
  #endif
  Todo += myBME280.readFloatHumidity();
  Todo += ","; 

  // print out the value you read:
  #ifdef DEBUG
  Serial.print(voltage);
  Serial.println(" V");
  #endif
  Todo += voltage;
  Todo += ","; 

  SerialUSB.println();
}

long selectBand(int a){    
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

//----------------------------------------------------------------
//  Listen to see if the GPS device is correctly 
//  connected and functioning.

static void listenForSomething()
{
  uint32_t current_ms  =  millis();
  uint32_t ms_since_last_rx   = current_ms - last_rx;
    if ((ms_since_last_rx >7000)) {
      DEBUG_PORT.println( F("\nCheck GPS device and/or connections.  No data received.\n") );
 }
}
