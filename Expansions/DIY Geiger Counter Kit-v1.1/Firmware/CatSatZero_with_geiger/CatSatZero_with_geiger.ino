/************************************************************
  SPANISH
  CatSat Zero con contador Geiger
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
  - Geiger Counter


  Este código es beerware si tu me ves ( o cualquier otro miembro de Electronic Cats)
  a nivel local, y tu has encontrado nuestro código útil ,
  por favor comprar una ronda de cervezas!

  Distribuido como; no se da ninguna garantía.
************************************************************/

/************************************************************
  ENGLISH
  CatSat Zero with Geiger Counter
  CatSat - Satelite en Lata Educativo
  Andres Sabas @ Electronic Cats
  Eduardo Contreras @ Electronic Cats
  Original Creation Date: Agu 14, 2018
  https://github.com/ElectronicsCats/CatSatZero/

  This example demonstrates how to use CatSat with a counter geiger

  Development environment specifics:
  IDE: Arduino 1.8.4
  Hardware Platform:
  Kit CanSat
  - Arduino MKR 1300
  - BMP280
  - CCS811
  - GPS L80
  - Counter Geiger

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.

  Library Arduino LoRa
  https://github.com/sandeepmistry/arduino-LoRa

  Radiation Detector Compatible DIY Kit ver 2.01 or higher

  CPM counting algorithm is very simple, it just collect GM Tube events during presettable log period.
  For radiation monitoring station it's recommended to use 30-60 seconds logging period. Feel free to modify
  or add functions to this sketch. This Arduino software is an example only for education purpose without any
  warranty for precision radiation measurements. You are fully responsible for your safety in high
  radiation area!!
  --------------------------------------------------------------------------------------
  WHAT IS CPM?
  CPM (or counts per minute) is events quantity from Geiger Tube you get during one minute. Usually it used to
  calculate a radiation level. Different GM Tubes has different quantity of CPM for background. Some tubes can produce
  about 10-50 CPM for normal background, other GM Tube models produce 50-100 CPM or 0-5 CPM for same radiation level.
  Please refer your GM Tube datasheet for more information. Just for reference here, SBM-20 can generate
  about 10-50 CPM for normal background.
  --------------------------------------------------------------------------------------
  HOW TO CONNECT GEIGER KIT?
  The kit 3 wires that should be connected to CatSat board: VCC, GND and INT. PullUp resistor is included on
  kit PCB. Connect INT wire to Digital Pin (Interrupt), 5V to VCC, GND to GND. Then connect the Arduino with
  USB cable to the computer and upload this sketch.
*/
//Uncomment for debug
//#define DEBUG

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
     IMPORTANTE CAMBIAR id_node DEPENDIENDO TU CANSAT
************************************************************/

String id_node = "A1";

/*******************************************************
  Selecciona un canal entre 0 y 12 este debe coincidir
  con el canal de tu satelite
 *******************************************************/
int channel = 12;

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

float voltage = 0;

//**************Geiger**************//
#define LOG_PERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch

//CPM to uSV/h
//The GM tube M4011 conversion index is 151, which means that: 151CPM=1μSv/h.
//So if the counter number is 30 CPM, the radiation is also:
//(30/151)μSv/h=0.1987μSv/h
#define index_conversion 151.00

unsigned long counts = 0;     //variable for GM Tube events
unsigned long cpm = 0;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
float radiationValue = 0.0;

#define radationpin 4

void enviarInfo(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
#ifdef DEBUG
  SerialUSB.println("Dato enviado");
#endif
}

void gpsread(void) {
  SerialUSB.print("Location: ");
  if (fix.valid.location)
  {
    Todo += String(fix.latitude(), 6);
    Todo += ",";
    Todo += String(fix.longitude(), 6);
    Todo += ",";
    Todo += String(fix.altitude(), 6);
    Todo += ",";
    Todo += String(fix.speed_kph(), 6);
    SerialUSB.print(fix.latitude(), 6);
    SerialUSB.print(F(","));
    SerialUSB.print(fix.longitude(), 6);
  }
  else
  {
    Todo += "0";
    Todo += ",";
    Todo += "0";
    Todo += ",";
    Todo += "0";
    Todo += ",";
    Todo += "0";
    SerialUSB.print("INVALID");
  }

  SerialUSB.print("  Date/Time: ");
  if (fix.valid.date)
  {
    SerialUSB.print(fix.dateTime.month);
    SerialUSB.print("/");
    SerialUSB.print(fix.dateTime.day);
    SerialUSB.print("/");
    SerialUSB.print(fix.dateTime.year);
  }
  else
  {
    SerialUSB.print("INVALID");
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
    SerialUSB.print("INVALID");
  }

  SerialUSB.println();
}

void ballonModeGPS() {
  Serial1.begin(9600);
  bool flag = 0;
  String gpsData = "";
  while (!flag) {
    if ((Serial1.available())) {
      while (Serial1.available()) {
        char c = Serial1.read();  //Wait for a data on the GPS
        if (c = '\n') {
          flag = 1;
          Serial1.println(PMTK_SET_NMEA_886_PMTK_FR_MODE);
          while (Serial1.available()) {
            gpsData += (char)Serial1.read();
          }
          if (!gpsData.startsWith("$PMTK001,8"))flag = 0;
          gpsData = "";
          Serial1.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        }
      }
    }
  }
  Serial.println("GPS balloon mode configured");
}

void tube_impulse() {      //subprocedure for capturing events from Geiger Kit
  detachInterrupt(radationpin);
  counts++;
  while (digitalRead(radationpin) == 0) {
  }
  attachInterrupt(digitalPinToInterrupt(radationpin), tube_impulse, FALLING);
}

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.println();
  SerialUSB.println("Settings...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(A6, OUTPUT); //Wakeup pin CCS811
  digitalWrite(A6, LOW); //Enable CCS811

  /*****LoRa init****/

  if (!LoRa.begin(selectBand(channel))) {           // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  LoRa.setTxPower(17); //Set the max transmition power
  LoRa.setSpreadingFactor(10); //Change the SF to get longer distances

  /******************/

  //This begins the CCS811 sensor and prints error status of .begin()
  if (!ccs.begin(CCS811_ADDR)) {
    Serial.println("Failed to start sensor CCS811! Please check your wiring.");
    while (1);
  }

  //calibrate temperature sensor
  while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

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
  myBME280.begin();

  /*
    Activation Balloon mode:
    For high-altitude balloon purpose that the vertical movement will
    have more effect on the position calculation
  */
  ballonModeGPS();
  
  SerialUSB.println("CaTSat Zero Ready!");
  
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  attachInterrupt(digitalPinToInterrupt(radationpin), tube_impulse, FALLING); //define external interrupts
}

void loop() {
  Todo += id_node;  //Add id to String
  Todo += ",";

  //unsigned long currentMillis = millis();
  if (millis() - previousMillis > LOG_PERIOD) {
    previousMillis = millis();
    cpm = counts * multiplier;
    radiationValue = cpm / index_conversion;
#ifdef DEBUG
    Serial.print("cpm = ");
    Serial.println(cpm, DEC);
    Serial.print("uSv/h = ");
    Serial.println(radiationValue, 4);
#endif

    counts = 0;
  }

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
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }

  Todo = "";
  listenForSomething();
}

void printInfoSerial()
{
  if (ccs.available()) {
    float temp = ccs.calculateTemperature();
    if (!ccs.readData()) {
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
    else {
      Serial.println("ERROR!");
      while (1);
    }
  }

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
  SerialUSB.print(" Pressure: ");
  SerialUSB.print((myBME280.readFloatPressure() * 0.0002953), 2);
  SerialUSB.println(" InHg");
#endif

#ifdef DEBUG
  SerialUSB.print(" Altitude: ");
  SerialUSB.print(myBME280.readFloatAltitudeMeters(), 2);
  SerialUSB.println("m");
#endif
  Todo += myBME280.readFloatAltitudeMeters();
  Todo += ",";

#ifdef DEBUG
  SerialUSB.print(" Altitude: ");
  SerialUSB.print(myBME280.readFloatAltitudeFeet(), 2);
  SerialUSB.println("ft");

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

  Todo += radiationValue;
  Todo += ",";

  SerialUSB.println();

}

long selectBand(int a)
{
  switch (a) {
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
  if ((ms_since_last_rx > 7000)) {
    DEBUG_PORT.println( F("\nCheck GPS device and/or connections.  No data received.\n") );
  }
}
