/* ===================================
 * Author          :  Ioannis Smanis
 * Firmware Version:  0.2
 * Project Name    :  Test NovaXR V1 Board - HW Test
 * 
 * --------- I2C Devices -------------
 * 1. MPU6050 6-Axis IMU - 0x68
 * 2. Crypto             - 0x60
 * 4. uBlox Module       - N/A
 *  
 * --------- SPI Devices -------------
 * 1. ADC #0 ADS1299 - 0x1E - Main Board
 * 2. ADC #1 ADS1299 - 0x1E - Sister Board
 * 
 * --------- Direct Analog Sensors ---
 * 1. EDA Impedance 
 * 2. Battery Level 
 */

// ---- Debugging Options ---------------
#define WIFI    // comment it out if you dont want to involve uBlox in your test



// ---- Imported Libraries ---------------
#include "Pin_Table_Defs.h"
#include "SAMD_AnalogCorrection.h"
#include <Wire.h>
#include <SPI.h>

#ifdef WIFI
   #include <WiFiNINA.h>
   #include <utility/wifi_drv.h>
   WiFiDrv uBLox;
#endif

// ---- ADS1299 Registers -----------------
//SPI Command Definitions (pg. 35)
const byte WAKEUP  = 0b00000010;   // Wake-up from standby mode
const byte STANDBY = 0b00000100;   // Enter Standby mode
const byte RESET   = 0b00000110;   // Reset the device
const byte START   = 0b00001000;   // Start and restart (synchronize) conversions
const byte STOP    = 0b00001010;   // Stop conversion
const byte RDATAC  = 0b00010000;   // Enable Read Data Continuous mode (default mode at power-up) 
const byte SDATAC  = 0b00010001;   // Stop Read Data Continuous mode
const byte RDATA   = 0b00010010;   // Read data by command; supports multiple read back

//Register Read Commands
const byte RREG = 0b00000000;
const byte WRET = 0b00000000;

const int CS1 = PA21; //chip select pin
const int CS2 = PA20; //chip select pin
const int DRDY = PA10; //data ready pin
const int EDApin = PA02; // EDA read
const int BATT_Sense_pin = PA05; // EDA read

const float tCLK = 0.000666;

// --------------- Prototype Functions ---------------
void SAMD21_Tester(void);
void SAMD21_UBLOX_Pin_Tester(void);
void SAMD21_GPIO_Pin_Tester(void);
void uBlox_RGB_LED_Tester(void);
void CheckPinStatus(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start);
void EDA_BATT_LEVEL_Tester(void);
void Analog_Check(String Array_pinNames[], int Array_pinNumber[], int TotalPins, int start);
void checkI2C_devices(void);
void ADCs_Test(const int ChipSel_1, const int ChipSel_2 );
bool ADS1299_detected(const int Chip_Select);
byte getDeviceID(const int chip_select);








// ============================================================================================
void setup() {

  Serial.begin(9600); 
  while(!Serial);

  #ifdef WIFI
    uBLox.pinMode(LED_GREEN, OUTPUT); 
    uBLox.pinMode(LED_BLUE,  OUTPUT);
    uBLox.pinMode(LED_RED,   OUTPUT);
  #endif

  
   Serial.println("\n ==== NovaXR V1 QA Test Started ==== ");
   
  #ifdef WIFI
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
  #endif
  
     
   // --- If not, go the next level of reading all the GPIOs of SAMD21 in order to diagnose a wrong pulled-up or pulled-down signal ---
   SAMD21_Tester();
   delay(100);
   // --- Detect if any ADC ADS1299 is alive on the board ------ 
   ADCs_Test(CS1,CS2);
   Serial.println("\n ==== QA Test Ended ==== \n");
  
}


void loop(){

  delay(1000000);
}

// ============================================================================================









// --- Get the state of GPIOs of SAMD21 which are connected to uBlox module ---
void SAMD21_UBLOX_Pin_Tester(void){
  #define MCUTOTALPINS 11
  String PinNames[MCUTOTALPINS] = {"PA13", "PB08", "PA08", "PA09", "PA12", "PA14", "PA15", "PA27", "PA28", "PB22", "PB23"};
     int PinTable[MCUTOTALPINS] = {PA13, PB08, PA08, PA09, PA12, PA14, PA15, PA27, PA28, PB22, PB23};
  
  Serial.print("\n ---------- SAMD21 - UBLOX Connections Status ---------- ");
  Serial.print("Total Pins: ");
  Serial.println(MCUTOTALPINS);
  CheckPinStatus(PinNames, PinTable,MCUTOTALPINS,0);
   Analog_Check(PinNames,  PinTable,MCUTOTALPINS,0);
}



// --- Get the state of GPIOs of SAMD21 which are connected to the rest modules ---
void SAMD21_GPIO_Pin_Tester(void){
  #define WIFITOTALPINS 10
  String PinNames[WIFITOTALPINS] = {"PA21", "PA16", "PA02", "PA04", "PA05", "PA10",  "PA17", "PA18", "PA19", "PA20"};
     int PinTable[WIFITOTALPINS] = {PA21, PA16, PA02, PA04, PA05, PA10,  PA17, PA18, PA19, PA20};

  Serial.print("\n ---------- SAMD21 GPIO Connections Status ---------- ");
  Serial.print("Total Pins: ");
  Serial.println(WIFITOTALPINS);
  CheckPinStatus(PinNames, PinTable, WIFITOTALPINS,0);
    Analog_Check(PinNames, PinTable, WIFITOTALPINS,2); //pa21 , Pa16
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
  readAccumulator = analogRead(EDApin); // NovaXR EDA Level
  Serial.println(readAccumulator);
  Serial.print("Battery Level:  ");
  readAccumulator = analogRead(BATT_Sense_pin); // NovaXR BATT Level
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






void ADCs_Test(const int ChipSel_1, const int ChipSel_2 ){
  Serial.print("\n ---------- SAMD21 SPI Peripherals ---------- \n");
  Serial.println ("- Detect ADC Components: \n");
  bool ADC1,ADC2=false;

  ADC1 = ADS1299_detected(ChipSel_1);
  delay(100);
  ADC2 = ADS1299_detected(ChipSel_2);
  
  if(ADC1){
    Serial.println("ADC ID#0 - AS1299 Detected"); 
  }
  
  if(ADC2){
    Serial.println("ADC ID#1 - AS1299 Detected"); 
  }

  if((ADC2=false) && (ADC1=false)){
    Serial.println("No ADS1299 Device Detected");
  }
 
  
}





bool ADS1299_detected(const int Chip_Select){

  pinMode(DRDY, INPUT);
  pinMode(Chip_Select, OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);  //Divides 16MHz clock by 16 to set CLK speed to 1MHz
  SPI.setDataMode(SPI_MODE1);            //clock polarity = 0; clock phase = 1 (pg. 8)
  SPI.setBitOrder(MSBFIRST);             //data format is MSB (pg. 25)
  delay(10);                             //delay to ensure connection
  
  digitalWrite(Chip_Select, LOW);  //Low to communicate
  SPI.transfer(RESET); 
  digitalWrite(Chip_Select, HIGH); //Low to communicate
  delay(10); 

  byte ExpectedAddress = ADS1299_ID;
  byte receivedByte = getDeviceID(Chip_Select);
  bool Detected = false;
  
  if(ExpectedAddress == receivedByte ){
    Detected = true;
  } 
  else {
    Detected = false;
  }
  return Detected;
}






byte getDeviceID(const int chip_select){

  digitalWrite(chip_select, LOW); //Low to communicated
  SPI.transfer(SDATAC);
  SPI.transfer(0x20); //RREG
  SPI.transfer(0x00); //Asking for 1 byte (hopefully 0b???11110)
  byte temp = SPI.transfer(0x00);
  digitalWrite(chip_select, HIGH); //Low to communicated
  byte mask = MASKADC_ADR;
  byte Address = temp & mask;

  return Address;
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
