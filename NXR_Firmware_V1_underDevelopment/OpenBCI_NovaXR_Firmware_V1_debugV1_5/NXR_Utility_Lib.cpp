/*
   Author:        Ioannis Smanis
   Hardware:      NovaXR - MKR1010 board
   Firmware:      Version 1.0
   Firmware Component: NXR Utility Library source file
*/

#include "Arduino.h"
#include "OpenBCI_NovaXR.h"
#include "NXR_Utility_Lib.h"

WiFiUDP UDP_Server;

#define ADS_DEV0 0
#define ADS_DEV1 1

uint32_t packetNUM = 0;




/*
void getADS_RAWdata(ADS_Data * data, bool ads_id){

    if (ads_id==ADS_DEV0){
      Serial.println("ADS Device #0 data request");
      for(int id=0; id<3; id++)data->EEG_FC[0][id]= 12;
      for(int id=0; id<3; id++)data->EEG_FC[1][id]= 134;                
      for(int id=0; id<3; id++)data->EEG_OL[2][id]=121;       // 8 Channels 
      for(int id=0; id<3; id++)data->EEG_OL[0][id]=21;       // 8 Channels
      for(int id=0; id<3; id++)data->EOG[0][id]=211;
      for(int id=0; id<3; id++)data->EOG[1][id]=141;         // 2 channels
      for(int id=0; id<3; id++)data->EMG[0][id]=191;
      for(int id=0; id<3; id++)data->EMG[1][id]=181;
      for(int id=0; id<3; id++)data->EMG[2][id]=131;
      for(int id=0; id<3; id++)data->EMG[3][id]=151;         // 4 Channels
      Serial.println("ADS0 data: Ready");
      
    } else if (ads_id==ADS_DEV1){
      Serial.println("ADS Device #1 data request");
      for(int id=0; id<3; id++)data->EEG_FC[0][id]= 222;
      for(int id=0; id<3; id++)data->EEG_FC[1][id]= 34;                
      for(int id=0; id<3; id++)data->EEG_OL[2][id]=21;       // 8 Channels 
      for(int id=0; id<3; id++)data->EEG_OL[0][id]=210;       // 8 Channels
      for(int id=0; id<3; id++)data->EOG[0][id]=21;
      for(int id=0; id<3; id++)data->EOG[1][id]=41;         // 2 channels
      for(int id=0; id<3; id++)data->EMG[0][id]=11;
      for(int id=0; id<3; id++)data->EMG[1][id]=31;
      for(int id=0; id<3; id++)data->EMG[2][id]=31;
      for(int id=0; id<3; id++)data->EMG[3][id]=51;         // 4 Channels 
      Serial.println("ADS1 data: Ready");
      
    } else{
      Serial.println("Not Valid ADS ID");
      return;
    }
}
*/

void getADS_RAWdata(ADS_Data * data, bool ads_id){
     
      //--- EEG_FL
      for(int id=0; id<3; id++)data->EEG_FC[0][id]= 12;
      for(int id=0; id<3; id++)data->EEG_FC[1][id]= 134;
      
      //--- EEG_OL
      for(int id=0; id<3; id++)data->EEG_OL[0][id]= 122;        
      for(int id=0; id<3; id++)data->EEG_OL[1][id]= 22; 
      for(int id=0; id<3; id++)data->EEG_OL[2][id]= 31;        
      for(int id=0; id<3; id++)data->EEG_OL[3][id]= 29; 
      for(int id=0; id<3; id++)data->EEG_OL[4][id]= 101;        
      for(int id=0; id<3; id++)data->EEG_OL[5][id]= 21; 
      for(int id=0; id<3; id++)data->EEG_OL[6][id]= 121;        
      for(int id=0; id<3; id++)data->EEG_OL[7][id]= 21;      

      //--- EOG
      for(int id=0; id<3; id++)data->EOG[0][id]=211;
      for(int id=0; id<3; id++)data->EOG[1][id]=141;         

      //--- EMG
      for(int id=0; id<3; id++)data->EMG[0][id]=191;
      for(int id=0; id<3; id++)data->EMG[1][id]=181;
      for(int id=0; id<3; id++)data->EMG[2][id]=131;
      for(int id=0; id<3; id++)data->EMG[3][id]=151;        


}



void getIMU_RAWdata(IMU_Data * data){
   data->AXL_X=23;
   data->AXL_Y=923;
   data->AXL_Z=244;
   data->GYR_X=293;
   data->GYR_Y=664;
   data->GYR_Z=34;
}



void getAux_RAWdata(AuxSensor_Data * data){
    data->EDA=743;
    data->PPG=89;
}



void UDP_Server_Benchmarking(char DBuffer[]) {
   
  int i = 0;
  
  String val;
  unsigned long StartT, timeP1, timeP2, duration =0;
  int packs = PACKETSNUM;  //1556
  int packetSize = sizeof(DBuffer);

      //Serial.println("Connected to server successful!");
      for (i = 0; i < ITERATIONUM; i++) {
        

          StartT = micros(); // ---------------- Measure Time - Starting Point 
          
          for (int a = 0; a < packs; a++) {
              Send_Data_PktBuffer(DBuffer);
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









bool checkCommand(char CMD[]){
    bool Valid=false;

    if( CMD[0] =='S') {
       Valid = true;
    }else{
       Valid = false;
    }
    return Valid;
  
}




bool Receive_Command(void){
   
   bool validCMD = false;
   char packetBuffer[3]; //buffer to hold incoming packet

  // if there's data available, read a packet
  int packetSize = UDP_Server.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);

    /*
    Serial.print("From ");
    IPAddress remoteIp = UDP_Server.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(UDP_Server.remotePort());
   */

    // read the packet into packetBufffer
    int len = UDP_Server.read(packetBuffer, 3);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
  
   Serial.print("Command: ");
   Serial.println(packetBuffer);
   validCMD = processChar(packetBuffer[0]);    
  }
  return validCMD;
}


void getUpdatedSensoryData(Sensors_Data * data){
      getADS_RAWdata(&data->ADS,1);
      getIMU_RAWdata(&data->IMU);   
      getAux_RAWdata(&data->AUX); 
}

  
void start_DStream(Sensors_Data * data, DataPacket *packet){
   Serial.print("StreammingFlag: ");
   getUpdatedSensoryData(data);
   assemble_DataBuffer(data,1,packet);
   sendUDP_DataPkt(packet);
  
}


uint32_t getTimeStamp(void){
  return 343564545;
}

void assemble_DataBuffer(Sensors_Data *payload, uint8_t packetType, DataPacket *packet){
    packet->PacketNum = packetNUM;
    packet->Payload.AUX = payload->AUX;
    packet->Payload.ADS = payload->ADS;
    packet->Payload.IMU = payload->IMU;
    packet->T_Stamp.timeStamp = getTimeStamp();
}




void sendUDP_DataPkt (DataPacket *packet){

    char Pkt[sizeof(struct DataPacket)];
    memcpy(Pkt,&packet,sizeof(DataPacket));
    //size_t dataSize = sizeof(Pkt);
    //Serial.print("Send: ");Serial.println(dataSize);
    UDP_Server.beginPacket(UDP_Server.remoteIP(), UDP_Server.remotePort());
    UDP_Server.write(Pkt);
    UDP_Server.endPacket();
    //Serial.println("Packet Sent");
}



size_t Send_Data_PktBuffer(char DBuffer[]){
   
    size_t dataSize = sizeof(DBuffer);
    UDP_Server.beginPacket(UDP_Server.remoteIP(), UDP_Server.remotePort());
    UDP_Server.write(DBuffer);
    UDP_Server.endPacket();
    
  return dataSize;
}






void CreateAccessPoint(char APssid[], char APpass[]){
  int APstatus = WL_IDLE_STATUS;
  Serial.println("Access Point Device");
  
  //pinMode(led, OUTPUT);      // set the LED pin mode

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
  Serial.println(APssid);

  // Create open network. Change this line if you want to create an WEP network:
  APstatus = WiFi.beginAP(APssid, APpass);
  if (APstatus != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  
  // wait 10 seconds for connection:
  delay(10000);
  
   // you're connected now, so print out the status
  printWiFiStatus();
  
 }



void Connect_to_AP(void){
  
    int status = WL_IDLE_STATUS;
    int keyIndex = 0;            // your network key Index number (needed only for WEP)
    const char* ssid = WIFISSID;
    const char* pass = PASSWRD;
    unsigned int UDPPort = NETPORT;
  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
   
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

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

    Serial.println("");
    Serial.println("WiFi connected"); 
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


 
void startUDP_Server( unsigned int Port){
  UDP_Server.begin(Port);
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
