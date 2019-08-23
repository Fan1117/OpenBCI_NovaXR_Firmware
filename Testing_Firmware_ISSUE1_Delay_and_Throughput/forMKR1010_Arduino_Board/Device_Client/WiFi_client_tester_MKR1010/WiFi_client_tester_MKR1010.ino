/*
   Ioannis Smanis
   For MKR1010 board only
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>
#include "arduino_secrets.h"

File myFile;
const uint16_t port = 8000;
const char * host = "192.168.43.26";


int status = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;




void setup()
{

  Serial.begin(115200);

  // check for the WiFi module:
  while (!Serial);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Communication with WiFi module failed!");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

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

void loop()
{
  WiFiClient client;

  if (!client.connect(host, port)) {

    Serial.println("Connection to host failed");

    delay(1000);
    return;
  }

  Serial.println("Connected to server successful!");
  int i = 0;
  String val;
  unsigned long StartT, timeP1, timeP2, duration = 000;

  //while(1){
  int packs = 1556;
  myFile = SD.open("NovaXR.txt", FILE_WRITE);
  for (i = 0; i < 650; i++) {
    //while(1){
    StartT = micros();
    for (int a = 0; a < packs; a++) {
      // send 63 raw bytes
      client.print("HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHel");
    }
    timeP1 = micros();

    myFile.print(i);
    myFile.print(": ");
    myFile.print(packs);
    myFile.print("Packets in: ");
    myFile.print(timeP1 - StartT);
    myFile.print(" Micro Secs");

    myFile.print(" === Rate: ");
    myFile.print((timeP1 - StartT) / packs);
    myFile.print("us/Packet ===>  ");

    myFile.print(1000000 / ((timeP1 - StartT) / packs));
    myFile.print(" Packets/Sec ");
    myFile.print((1000000 / ((timeP1 - StartT) / packs)) * 63);
    myFile.println(" Bytes/Sec ");
  }
  myFile.close();
  Serial.println("done.");
  Serial.println("Disconnecting...");
  // client.stop();
  // }
  delay(10000000);
}
