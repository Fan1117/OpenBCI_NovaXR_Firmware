/*
 * Ioannis Smanis
 * For ESP32 board only
 */

#include <WiFi.h>


const char* ssid = "yourSSID";
const char* password =  "yourPASSWORD";
 
const uint16_t port = 8090;
const char * host = "yourSERVER_IP";



void setup()
{
 
  Serial.begin(115200);
   while (!Serial);
    

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
 
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
   int i=0;
   String val;
   unsigned long StartT, timeP1, timeP2, duration=000;
    
   //while(1){
   int packs =10000;

while(1){
  StartT = micros();
  for(int a=0; a<packs; a++){
  
    client.print("HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHel");

  }
   timeP1 = micros();

   
   Serial.print("10000 Packets in: ");
   Serial.print(timeP1-StartT);
   Serial.print(" Micro Secs");
   
   Serial.print(" === Rate: ");
   Serial.print((timeP1-StartT)/packs);
   Serial.print("us/Packet ===>  "); 
    
   Serial.print(1000000/((timeP1-StartT)/packs));
   Serial.print(" Packets/Sec ");
   Serial.print((1000000/((timeP1-StartT)/packs))*63);
   Serial.println(" Bytes/Sec ");
   
   // Serial.println("Disconnecting...");
   // client.stop();
  }
    delay(10000);
}
