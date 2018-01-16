/*
  Gebruik een knop om realtime een slack-bericht te versturen
  Copryright 2018 Dirk de Man (Twitter: @dirktheman)
  Gebruik onder MIT license

  INSTRUCTIES
  - Maak in Slack via Manage - Custom Integrations - Incoming Webhooks een nieuwe Incoming Webhook aan
  - Kies het kanaal waarin je wilt posten, een naam en eventueel een icoon
  - Sla op en noteer de Webhook URL

  - In de code hieronder vul je onder 'Wifi gegevens' de SSID en het wachtwoord van je wifi in
  - Bij de Slack configuratie vul je je webhook URL, eventueel een link naar een icoontje, de tekst en de gebruikersnaam van de custom integration in die je zojuist hebt gemaakt

  - Ten slotte sluit je je knop aan op de A0-pin van je NodeMCU en upload je de code
*/



/***************************** Includes **************************************/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>



/*************************** WiFi gegevens ***********************************/
char SSID[] = "Your SSID";
char pwd[] = "Your Wifi Password";



/************************* Slack configuratie **********************************/
const String slack_hook_url = "Your Slack incoming webhook URL";
const String slack_icon_url = "A nice icon (jpg/png) for your notification";
const String slack_message = "Your awesome message here";
const String slack_username = "Username of your custom integration here";



/****************************** Knop ******************************************/
#define Buttons       A0  // analog 0

int ButtonRead = 0;
int current = 0;
int last = -1;




void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(SSID, pwd);

  Serial.print("Verbinding maken...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Verbonden met IP-adres ");
  Serial.println(WiFi.localIP());
}



bool postMessageToSlack(String msg)
{
  const char* host = "hooks.slack.com";
  Serial.print("Verbinding maken met ");
  Serial.println(host);

  //Gebruik WiFiClient class om een TCP-verbinding te maken
  WiFiClientSecure client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("Verbinding maken mislukt :-(");
    return false;
  }

  //Verbinding? Dan kunnen we een POST-request naar de Slack-API maken!
  Serial.print("Posting to URL: ");
  Serial.println(slack_hook_url);

  String postData="payload={\"link_names\": 1, \"icon_url\": \"" + slack_icon_url + "\", \"username\": \"" + slack_username + "\", \"text\": \"" + msg + "\"}";

  client.print(String("POST ") + slack_hook_url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Connection: close" + "\r\n" +
               "Content-Length:" + postData.length() + "\r\n" +
               "\r\n" + postData);
  Serial.println("Request sent");
  String line = client.readStringUntil('\n');
  Serial.printf("Response code was: ");
  Serial.println(line);
  if (line.startsWith("HTTP/1.1 200 OK")) {
    return true;
  } else {
    return false;
  }
}


void loop()

{
  //Knopwaarde uitlezen en een kleine vertraging om bouncebacks uit te sluiten
  ButtonRead = analogRead(Buttons);
  delay(100);
    
  if (ButtonRead > 200 && ButtonRead < 1000) {
    postMessageToSlack(slack_message);
    delay(1000);
  }
  
}
