#include <SPI.h>
#include <WiFiNINA.h>
//############################initial
int status = WL_IDLE_STATUS;
WiFiServer server(80);
char ssid[] = "";
char pass[] = "";
char linebuf[80];
int charcount = 0;
//############################html
void fillForm(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("<html><head></head><body>");
  client.println();
  client.print("<form method=get>");
  client.print("<input type='text' name=IP value='''> IP<br>");
  client.print("<input type='text' name=password value=''> password<br>");
  client.print("<input type=submit value=submit></form>");
  client.print("</body></html>");
}

void parseHTML(char *linebuf, char *ssid, char *pass) {
  if (strstr(linebuf, "&") > 0) {
    char* p;
    p = strtok(linebuf, "&= ");
    for (int counter = 0; counter < 5; counter++) {
      if (counter == 2) {
        if (strlen(ssid) == 0) {
          strcpy(ssid, p);
          Serial.print("ssid is ");
          Serial.println(ssid);
        }
      }
      if (counter == 4) {
        if (strlen(pass) == 0) {
          strcpy(pass, p);
          Serial.print("pass is ");
          Serial.println(pass);
        }
      }
      p = strtok(NULL, "&= ");
    }
  }
}
//#############################setup
void setup() {
  // put your setup code here, to run once:
  AP_wifi_setup();
  printWiFiStatus();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1) charcount++;
        if (c == '\n') {
          parseHTML(linebuf, ssid, pass);
          fillForm(client);
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
          client.stop();
        }
      }
    }
  }
}
//#############################
void AP_wifi_setup ()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Access Point Web Server");
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
  // start the web server on port 80
  server.begin();
}

void printWiFiStatus() {
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

