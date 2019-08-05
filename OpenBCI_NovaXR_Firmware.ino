#include <SPI.h>
#include <WiFiNINA.h> // EXTERNAL DEPENDENCY NEED TO INSTALL FROM LIBRARY MANAGER
#include <WiFiUdp.h>
/////////////////////////////////
WiFiServer server(80);
char ssid[20] = ""; // for new wifi setup
char pass[20] = ""; // for new wifi setup
char linebuf[80];   // for new wifi setup
int charcount = 0;  // for new wifi setup
bool new_wifi_isconnected = false; // indicate new wifi setup
int status = WL_IDLE_STATUS;
#include "APwifi.h"
////////////////////////////////

#include "commands.h"
#include "package.h"

#define MAX_INCOM_PACKAGE_SIZE 5
#define DEBUG // temp, I have no idea how to pass it using arduino ide

WiFiUDP Udp;

State state = WAIT;
IPAddress client_ip;
int client_port = 0;

void setup ()
{
  int status = WL_IDLE_STATUS;
  unsigned int local_port = 2390;
  Serial.begin (9600);
  #ifdef DEBUG
  AP_wifi_setup();
  print_wifi_status();
  Udp.begin(local_port);
}

void loop ()
{
  if (!new_wifi_isconnected) {
    register_new_wifi();

    if (strlen(ssid) > 0) {
      new_wifi_setup();
      print_wifi_status();
      new_wifi_isconnected = true;
      strcpy(ssid, "");
      strcpy(pass, "");
    }
  }
  else {
    int package_size = Udp.parsePacket ();
    char package_buffer[MAX_INCOM_PACKAGE_SIZE];
    if (package_size) {
      if ((client_port) && (client_port != Udp.remotePort ()))
      {
        // TODO: HANDLE IT SOMEHOW AND ADD IP ADDR TO CHECK ABOVE
        Serial.println ("Port/Ip was changed");
      }
      client_ip = Udp.remoteIP ();
      client_port = Udp.remotePort ();
      Serial.print ("Received from Ip:");
      Serial.print (client_ip);
      Serial.print (" Port:");
      Serial.println (client_port);

      // read the packet into packetBufffer
      int len = Udp.read (package_buffer, MAX_INCOM_PACKAGE_SIZE);
      if (len > 0)
      {
        if (package_buffer[0] == START_STREAM)
        {
          // start streaming
          state = STREAMING;
          Serial.println ("start streaming session");
        }
        else if (package_buffer[0] == STOP_STREAM)
        {
          // stop streaming
          state = WAIT;
          Serial.println ("stop streaming session");
        }
        else
        {
          Serial.print ("Unknown command:");
          Serial.println (package_buffer);
        }
      }
    }
    if (state == STREAMING)
    {
      // TODO: CHECK THAT PACKAGE WAS DELIVERED
      // TODO: ADD DYNAMIC DELAY TO FREEZE SAMPLING RATE?
      unsigned char package[PACKAGE_LENGTH_BYTES];
      Package::get_package (package);
      Udp.beginPacket (client_ip, client_port);
      Udp.write (package, PACKAGE_LENGTH_BYTES);
      Udp.endPacket ();
    }
  }
}

void print_wifi_status()
{
  Serial.print ("SSID: ");
  Serial.println (WiFi.SSID ());

  IPAddress ip = WiFi.localIP ();
  Serial.print ("IP Address: ");
  Serial.println (ip);

  long rssi = WiFi.RSSI ();
  Serial.print ("signal strength (RSSI):");
  Serial.print (rssi);
  Serial.println (" dBm");
}
