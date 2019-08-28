/* ===================================
 * Author          :  Ioannis Smanis
 * Firmware Version:  0.1
 * Project Name    :  Test NovaXR V1 Board - HW Test
 * 
 * --------- I2C Devices -------------
 *  1. MPU6050 6-Axis IMU - 0x68
 *  2. Crypto/TI Battery Monitor or NINA  - 0x60 and 0x6B
 */

// ---- Debugging Options ---------------
#define WIFI    // comment it out if you dont want to involve uBlox in your test
#define MKR1010 // comment it out if you work with NovaXR board




// ---- Imported Libraries ---------------
#include "Pin_Table_Defs.h"
#include "SAMD_AnalogCorrection.h"
#include <Wire.h>


#ifdef WIFI
   #include <WiFiNINA.h>
   #include <utility/wifi_drv.h>
   WiFiDrv uBLox;
#endif


// ---- Prototype Functions ---------------
void SAMD21_Tester(void);
void SAMD21_UBLOX_Pin_Tester(void);
void SAMD21_GPIO_Pin_Tester(void);
void uBlox_RGB_LED_Tester(void);
void CheckPinStatus(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start);
void EDA_BATT_LEVEL_Tester(void);
void Analog_Check(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start);
void checkI2C_devices(void);





// ============================================================================================
void setup() {


  Serial.begin(9600); 
  while(!Serial);

 
#ifdef WIFI
  uBLox.pinMode(LED_GREEN, OUTPUT); 
  uBLox.pinMode(LED_BLUE,  OUTPUT);
  uBLox.pinMode(LED_RED,   OUTPUT);
#endif


   Serial.println("\n ==== NovaXR QA Test Started ==== ");

   // --- Check uBlox communication with SAMD21 and the RGB-LED 
   // -- If the LEDs blink RGB colors, SAMD21 communicates well with uBlox module
    uBLox.digitalWrite(LED_BLUE, HIGH); // for full brightness
     delay(100);
    uBLox.digitalWrite(LED_BLUE, LOW); // for full brightness
     delay(100);
    uBLox.digitalWrite(LED_GREEN, HIGH); // for full brightness
     delay(100);
    uBLox.digitalWrite(LED_GREEN, LOW); // for full brightness
     delay(100);
    uBLox.digitalWrite(LED_RED, HIGH); // for full brightness
     delay(100);
    uBLox.digitalWrite(LED_RED, LOW); // for full brightness
     delay(100);

     
   // -- If not, go the next level of reading all the GPIOs of SAMD21 in order to diagnose a wrong pulled-up or pulled-down signal
   SAMD21_Tester();
  
  Serial.println("\n ==== QA Test Ended ==== \n");
  
}


void loop(){

  delay(100000);
}

// ============================================================================================









// --- Get the state of GPIOs of SAMD21 which are connected to uBlox module ---
void SAMD21_UBLOX_Pin_Tester(void){
  #define TOTALPINS 11
  String PinNames[TOTALPINS] = {"PA13", "PB08", "PA08", "PA09", "PA12", "PA14", "PA15", "PA27", "PA28", "PB22", "PB23"};
     int PinTable[TOTALPINS] = {PA13, PB08, PA08, PA09, PA12, PA14, PA15, PA27, PA28, PB22, PB23};
  
  Serial.print("\n ---------- SAMD21 - UBLOX Connections Status ---------- ");
  Serial.print("Total Pins: ");
  Serial.println(TOTALPINS);
  CheckPinStatus(PinNames, PinTable,TOTALPINS,0);
   Analog_Check(PinNames, PinTable, TOTALPINS,0);
}



// --- Get the state of GPIOs of SAMD21 which are connected to the rest modules ---
void SAMD21_GPIO_Pin_Tester(void){
  #define TOTALPINS 10
  String PinNames[TOTALPINS] = {"PA21", "PA16", "PA02", "PA04", "PA05", "PA10",  "PA17", "PA18", "PA19", "PA20"};
     int PinTable[TOTALPINS] = {PA21, PA16, PA02, PA04, PA05, PA10,  PA17, PA18, PA19, PA20};

  Serial.print("\n ---------- SAMD21 GPIO Connections Status ---------- ");
  Serial.print("Total Pins: ");
  Serial.println(TOTALPINS);
  CheckPinStatus(PinNames, PinTable, TOTALPINS,0);
    Analog_Check(PinNames, PinTable, TOTALPINS,2); //pa21 , Pa16
}


// --- Generic Function that checks a range of GPIOs ---
void CheckPinStatus(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start){
  
  for (int pinNum=start; pinNum<TotalPins; pinNum++){
    pinMode(Array_pinNumber[pinNum],INPUT);
    delay(100);
  }
  Serial.println();

  for (int pinNum=0; pinNum<TotalPins; pinNum++){
      int PIN_STATE = digitalRead(Array_pinNumber[pinNum]);
      delay(100); 
      Serial.print("Pin #");
      Serial.print(Array_pinNames[pinNum]);
      
      if (PIN_STATE == HIGH) {
        Serial.println("\tTonggled: HIGH");
      } else {
        Serial.println("\tTonggled: LOW");
      }
      
  }  
  Serial.println();
 
}


// --- Map all possible GPIOs as Analog input pins and measure the input value ---
void Analog_Check(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start){
    for (int pinNum=start; pinNum<TotalPins; pinNum++){
      int val = analogRead(Array_pinNumber[pinNum]);
      Serial.print("Pin #");
      Serial.print(Array_pinNames[pinNum]);
      Serial.print("\tAnalog Val: ");
      Serial.println(val);
  }
 Serial.println();
}


// --- Check the Analog Front End from EDA circuit and Battery level circuit ---
void EDA_BATT_LEVEL_Tester(void){
  Serial.println(" ---------- SAMD21 - EDA & Battery Level Sense ----------\n");
  uint32_t readAccumulator =0;
  Serial.print("EDA value:      ");
  readAccumulator = analogRead(PA02); // NovaXR EDA Level
  Serial.println(readAccumulator);
  Serial.print("Battery Level:  ");
  readAccumulator = analogRead(PB09); // NovaXR BATT Level
  Serial.print(readAccumulator);
  Serial.println();
}





void SAMD21_Tester(void){
  
  // --- Test 1. UBLOX connenctions ------------------
  SAMD21_UBLOX_Pin_Tester();
  delay(1000);
 
  // --- Test 2. SAMD21 GPIO Peripherals Connection --
  SAMD21_GPIO_Pin_Tester();
  delay(1000);
  
  EDA_BATT_LEVEL_Tester();
  delay(1000);  
  
  // --- Test 3. Connected I2C Peripherals  ----------
  checkI2C_devices();
  delay(1000); 
}




// --- Search for all the connected I2C devices --------------
void checkI2C_devices(void){

 Wire.begin();
  Serial.print("\n ---------- SAMD21 I2C Peripherals ---------- \n");
  byte error, address;
  int nDevices;

  Serial.println("Devices:");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C Device Address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
}



#ifdef WIFI

  void uBlox_RGB_LED_Tester(void){
    
    Serial.println("\n --- uBlox RGB-LED Test ---");
    
    uBLox.digitalWrite(LED_BLUE, HIGH); // for full brightness
     delay(50);
    uBLox.digitalWrite(LED_BLUE, LOW); // for full brightness
     delay(50);
    uBLox.digitalWrite(LED_GREEN, HIGH); // for full brightness
     delay(50);
    uBLox.digitalWrite(LED_GREEN, LOW); // for full brightness
     delay(50);
    uBLox.digitalWrite(LED_RED, HIGH); // for full brightness
     delay(50);
    uBLox.digitalWrite(LED_RED, LOW); // for full brightness
     delay(50);
  }
#endif
