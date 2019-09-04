/*
   Author:        Ioannis Smanis
   Hardware:      NovaXR - MKR1010 board
   QA Firmware :  UDP Packet Transmission over WiFi
   Firmware:      Version 0.5
   QA Test:       ISSUE 02
   Device Mode:   Access Point Mode
*/



#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>


#define SECRET_SSID "NovaXR_D1"
#define SECRET_PASS "novaxr1!nova"

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


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;

unsigned int UDPPort = 8090;
WiFiUDP UDP_Server;

size_t SendUDP_PacketBuff(void);
void UDP_Server_Benchmarking(void);
void CreateAccessPoint(void);
void printWiFiStatus(void);
bool ReadCommand(void);




void setup() {
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
 
  while (!Serial) ; // wait for serial port to connect. Needed for native USB port only
  
  // --- Create WiFi Device in Access Point Mode ----
  CreateAccessPoint();

  // you're connected now, so print out the status
  printWiFiStatus();

  UDP_Server.begin(UDPPort);
  
  Serial.print("Test Buffer Size: ");
  Serial.println(sizeof(Buffer));
  delay(1000); 
  
}







void loop() {
  
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    
    // it has changed update the variable
    status = WiFi.status();
    
    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }


   if(ReadCommand()){
        while(1)UDP_Server_Benchmarking();
    }    


}



size_t SendUDP_PacketBuff(void) {

    size_t dataSize = sizeof(Buffer);
    UDP_Server.beginPacket(UDP_Server.remoteIP(), UDP_Server.remotePort());
    //UDP_Server.print(Buffer);
    UDP_Server.write(Buffer);
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
              SendUDP_PacketBuff();
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





void CreateAccessPoint(void){
  
  Serial.println("Access Point Device");
  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  
  // wait 10 seconds for connection:
  delay(10000);
  
   // you're connected now, so print out the status
  printWiFiStatus();
  
 }

 

bool ReadCommand(void){
   
   bool cmdOK = false;
   char packetBuffer[3]; //buffer to hold incoming packet
  // if there's data available, read a packet
  int packetSize = UDP_Server.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = UDP_Server.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(UDP_Server.remotePort());

    // read the packet into packetBufffer
    int len = UDP_Server.read(packetBuffer, 3);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    if( packetBuffer[0] =='S') {
        cmdOK = true;
    }else{
        cmdOK = false;
    }
 
   // Serial.println("Contents:");
   // Serial.println(packetBuffer);
  }
  
  return cmdOK;
}




void printWiFiStatus(void) {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
