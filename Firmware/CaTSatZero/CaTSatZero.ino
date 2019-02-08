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
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5A //0x5B Alternate I2C Address

//Command activation Balloon mode
#define PMTK_SET_NMEA_886_PMTK_FR_MODE  "$PMTK001,886,3*36"
// Command turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

static NMEAGPS gps;
static gps_fix  fix;

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

float voltage=0;

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

  /*****LoRa init****/

  if (!LoRa.begin(selectBand(channel))) {           // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
      while (true);                       // if failed, do nothing
  }

  LoRa.setTxPower(17); //Set the max transmition power
  LoRa.setSpreadingFactor(10); //Change the SF to get longer distances

  /******************/

  //This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  SerialUSB.print("CCS811 begin exited with: ");
  //Pass the error code to a function to print the results
  printDriverError( returnCode );
  SerialUSB.println();

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

  SerialUSB.println("CaTSat Zero Ready!");

  /*
  * Activation Balloon mode: 
  * For high-altitude balloon purpose that the vertical movement will 
  * have more effect on the position calculation
  */
  gpsPort.println(PMTK_SET_NMEA_886_PMTK_FR_MODE);
  
  //Command turn on GPRMC and GGA
  gpsPort.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  gpsPort.begin(9600);
}

void loop() {
  
  Todo += id_node;  //Add id to String 
  Todo += ",";
  
  while (gps.available( gpsPort )) {
    fix = gps.read();
    //Check to see if data is available
    if (myCCS811.dataAvailable())
    {
      //Calling this function updates the global tVOC and eCO2 variables
      myCCS811.readAlgorithmResults();
      //printInfoSerial fetches the values of tVOC and eCO2
      printInfoSerial();

      float BMEtempC = myBME280.readTempC();
      float BMEhumid = myBME280.readFloatHumidity();
    
      #ifdef DEBUG
      SerialUSB.print("Applying new values (deg C, %): ");
      SerialUSB.print(BMEtempC);
      SerialUSB.print(",");
      SerialUSB.println(BMEhumid);
      SerialUSB.println();
      #endif
      //This sends the temperature data to the CCS811
      myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
    }
    else if (myCCS811.checkForStatusError())
    {
      //If the CCS811 found an internal error, print it.
      printSensorError();
    }

    // read the input on analog pin battery 
    //NOTE: voltage max 1.18v in the pin of chip
    int sensorValue = analogRead(ADC_BATTERY);
    // Convert the analog reading (0 - 1.18v to 0 - 100%):
    voltage = map(sensorValue,0,339,0,100);
  
    gpsread();
    
    Serial.println(Todo);
    enviarInfo(Todo);  
    digitalWrite(LED_BUILTIN,HIGH); 
    delay(500);
    digitalWrite(LED_BUILTIN,LOW); 
    delay(500); 
    digitalWrite(LED_BUILTIN,HIGH); 
    delay(500);
    digitalWrite(LED_BUILTIN,LOW); 
    delay(500); 
    digitalWrite(LED_BUILTIN,HIGH); 
    delay(500);
    digitalWrite(LED_BUILTIN,LOW); 
    delay(500); 
    }
  
    Todo = "";
  
  // parse for a packet, and call onReceive with the result:
  //onReceive(LoRa.parsePacket());
  
}

void printInfoSerial()
{
  #ifdef DEBUG
  //getCO2() gets the previously read data from the library
  SerialUSB.println("CCS811 data:");
  SerialUSB.print(" CO2 concentration : ");
  SerialUSB.print(myCCS811.getCO2());
  SerialUSB.println(" ppm");
  #endif
  Todo += myCCS811.getCO2();
  Todo += ","; 
  

  //getTVOC() gets the previously read data from the library
  #ifdef DEBUG
  SerialUSB.print(" TVOC concentration : ");
  SerialUSB.print(myCCS811.getTVOC());
  SerialUSB.println(" ppb");
  #endif
  Todo += myCCS811.getTVOC();
  Todo += ","; 

  #ifdef DEBUG
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
  Serial.println("V");
  #endif
  Todo += voltage;
  Todo += ","; 

  SerialUSB.println();
}

//printDriverError decodes the CCS811Core::status type and prints the
//type of error to the serial terminal.
//
//Save the return value of any function of type CCS811Core::status, then pass
//to this function to see what the output was.
void printDriverError( CCS811Core::status errorCode )
{
  switch ( errorCode )
  {
    case CCS811Core::SENSOR_SUCCESS:
      SerialUSB.println("SUCCESS");
      break;
    case CCS811Core::SENSOR_ID_ERROR:
      SerialUSB.println("ID_ERROR");
      SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
      while (1);
      break;
    case CCS811Core::SENSOR_I2C_ERROR:
      SerialUSB.println("I2C_ERROR");
      SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
      while (1);
      break;
    case CCS811Core::SENSOR_INTERNAL_ERROR:
      SerialUSB.println("INTERNAL_ERROR");
      SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
      while (1);
      break;
    case CCS811Core::SENSOR_GENERIC_ERROR:
      SerialUSB.println("GENERIC_ERROR");
      SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
      while (1);
      break;
    default:
      SerialUSB.println("Unspecified error.");
      SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
      while (1);
  }
}

//printSensorError gets, clears, then prints the errors
//saved within the error register.
void printSensorError()
{
  uint8_t error = myCCS811.getErrorRegister();

  if ( error == 0xFF ) //comm error
  {
    SerialUSB.println("Failed to get ERROR_ID register.");
    SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
    while (1);
  }
  else
  {
    SerialUSB.print("Error: ");
    if (error & 1 << 5) SerialUSB.print("HeaterSupply");
    if (error & 1 << 4) SerialUSB.print("HeaterFault");
    if (error & 1 << 3) SerialUSB.print("MaxResistance");
    if (error & 1 << 2) SerialUSB.print("MeasModeInvalid");
    if (error & 1 << 1) SerialUSB.print("ReadRegInvalid");
    if (error & 1 << 0) SerialUSB.print("MsgInvalid");
    SerialUSB.println("Could not find a valid CCS811 sensor, check wiring!");
    SerialUSB.println();
    while (1);
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
