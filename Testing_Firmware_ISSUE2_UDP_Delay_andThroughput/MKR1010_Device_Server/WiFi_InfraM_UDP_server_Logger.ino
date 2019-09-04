/*
   Author:        Ioannis Smanis
   Hardware:      NovaXR - MKR1010 board
   QA Firmware :  UDP Packet Transmission over WiFi
   Firmware:      Version 0.5
   QA Test:       ISSUE 02
   Device Mode:   Infrastructure Mode
*/


// ============= QA Parameters =======================
#define PACKETSNUM    300  // Number of Packets sent per session
#define ITERATIONUM   650  // Number of Iterations
//-- Send Data buffers:
/*63   bytes*/  //const char Buffer[]="HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello063";
/*127  bytes*/  //const char Buffer[]="HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello123";            // a string to send back
/*505  bytes*/  //const char Buffer[]="HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHel";            // a string to send back
/*680  bytes*/  //const char Buffer[]="HelloHelloHelloHelloHelloHellHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHello";
/*1290 bytes*/    const char Buffer[]="HelloHelloHelloHelloHelloHellHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHello";
/*1469 bytes*/  //const char Buffer[]="HelloHelloHelloHelloHelloHellHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHel";
// ====================================================


#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"



const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
unsigned int UDPPort = 8090;
WiFiUDP UDP_Server;




void setup() {
  Serial.begin(9600);
  while(!Serial);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  //WiFi.config(IPAddress(192, 168, 3, 60), IPAddress(192, 168, 3, 1), IPAddress(255, 255, 255, 0));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  UDP_Server.begin(UDPPort);

  Serial.print("Test Buffer Size: ");
  Serial.println(sizeof(Buffer));
  delay(1000);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  myFile = SD.open("NovaXR.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("NovaXR testing...");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening NovaXR_test.txt");
  }

}



void loop() {
  uint32_t test1 = micros();
  //size_t a = handleUDPServer();
    UDP_Server_Benchmarking();
   uint32_t test2 = micros();
}





size_t handleUDPServer() {

    size_t dataSize = sizeof(Buffer);
    UDP_Server.beginPacket(UDP_Server.remoteIP(), UDP_Server.remotePort());
    //UDP_Server.write(Buffer);
    UDP_Server.print(Buffer);
    UDP_Server.endPacket();

  return dataSize;
}



void UDP_Server_Benchmarking(void) {

  int i = 0;
  String val;
  unsigned long StartT, timeP1, timeP2, duration =0;
  int packs = PACKETSNUM;  //1556
  int packetSize = sizeof(Buffer);

      //Serial.println("Connected to server successful!");
      for (i = 0; i < ITERATIONUM; i++) {


          StartT = micros(); // ---------------- Measure Time - Starting Point

          for (int a = 0; a < packs; a++) {
              handleUDPServer();
           }

          timeP1 = micros(); // ---------------- Measure Time - Time Point 1

          Serial.print(i);
          Serial.print(": ");
          Serial.print(packs);
          Serial.print(" Packets in: ");
          Serial.print(timeP1 - StartT);
          Serial.print(" Micro Secs");

          Serial.print(" === Rate: ");
          Serial.print((timeP1 - StartT) / packs);
          Serial.print("us/Packet ===>  ");

          Serial.print(1000000 / ((timeP1 - StartT) / packs));
          Serial.print(" Packets/Sec ");
          Serial.print((1000000 / ((timeP1 - StartT) / packs)) * packetSize);
          Serial.println(" Bytes/Sec ");

   }
}
