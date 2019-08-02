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
      if (counter == 2 && strlen(ssid) == 0) {
        strcpy(ssid, p);
        Serial.print("ssid is ");
        Serial.println(ssid);
      }
      if (counter == 4 && strlen(pass) == 0) {
        strcpy(pass, p);
        Serial.print("pass is ");
        Serial.println(pass);
      }
      p = strtok(NULL, "&= ");
    }
  }
}
void register_new_wifi () {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1) charcount++;
        if (c == '\n') {
          parseHTML(linebuf, ssid, pass);
          Serial.print("ssid is ");
          Serial.println(ssid);
          Serial.print("pass is ");
          Serial.println(pass);
          fillForm(client);
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
          client.stop();
        }
      }
    }
  }
}
void AP_wifi_setup ()
{
  char AP_ssid[] = "";
  char AP_pass[] = "";
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Access Point Web Server");
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  Serial.print("Creating access point named: ");
  Serial.println(AP_ssid);
  status = WiFi.beginAP(AP_ssid, AP_pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }
  delay(10000);
  server.begin();
}

void new_wifi_setup() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.print("pass: ");
    Serial.println(pass);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(100);
  }
  Serial.println("Connected to wifi");
  // if you get a connection, report back via serial:
}
