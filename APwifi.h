void login_webpage(WiFiClient &client) {
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

void parse_html(char *line_buffer, char *ssid, char *pass) {
  if (strstr(line_buffer, "&") > 0) {
    char* substring;
    substring = strtok(line_buffer, "&= ");
    for (int counter = 0; counter < 5; counter++) {
      if (counter == 2) {
        if (strlen(ssid) == 0) {
          strcpy(ssid, substring);
          Serial.print("ssid is ");
          Serial.println(ssid);
        }
      }
      if (counter == 4) {
        if (strlen(pass) == 0) {
          strcpy(pass, substring);
          Serial.print("pass is ");
          Serial.println(pass);
        }
      }
      substring = strtok(NULL, "&= ");
    }
  }
}

void AP_wifi_setup ()
{
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
  Serial.println(ssid);
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }
  delay(10000);
  server.begin();
}
