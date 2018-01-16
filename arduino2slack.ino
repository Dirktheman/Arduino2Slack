/*
  Create a physical button to send a realtime Slack notification
  Copryright 2018 Dirk de Man (Twitter: @dirktheman)
  Use under MIT license

  INSTRUCTIONS
  - Create a new Incoming Webhook in Slack: Manage - Custom Integrations - Incoming Webhooks
  - Choose the Slack channel where you want to post your message in
  - Save and copy the webhook URL
  
  - Fill in your Wifi SSID (network name) and password in the Arduino code 
  - Fill in your webhook URL, a link to a nice icon, your text and the username of your custom integration
  
  - Wire your button to the analog pin of your NodeMCU (A0) and upload the code
*/



/***************************** Includes **************************************/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>



/*************************** WiFi credentials ***********************************/
char SSID[] = "Your SSID";
char pwd[] = "Your Wifi Password";



/************************* Slack configuration **********************************/
const String slack_hook_url = "Your Slack incoming webhook URL";
const String slack_icon_url = "A nice icon (jpg/png) for your notification";
const String slack_message = "Your awesome message here!";
const String slack_username = "Username of your custom integration here";



/****************************** Button ******************************************/
#define Buttons       A0  // analog 0

int ButtonRead = 0;
int current = 0;
int last = -1;




void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(SSID, pwd);

  Serial.print("Connecting to wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to IP address ");
  Serial.println(WiFi.localIP());
}



bool postMessageToSlack(String msg)
{
  const char* host = "hooks.slack.com";
  Serial.print("Connecting to ");
  Serial.println(host);

  //Use WiFiClient class to set up a TCP-connection
  WiFiClientSecure client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("Failed to connect :-(");
    return false;
  }

  //Was your connection successful? Great! Than we can make a POST-request to the Slack API!
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
  //Read button value, when between 200 and 1000 make API call to Slack.
  //Also: a small delay after the readout will mitigate button bounceback
  //A larger delay after the POST request will deal with those pesky people that double-click a button, thus sending the request twice
  ButtonRead = analogRead(Buttons);
  delay(100);
    
  if (ButtonRead > 200 && ButtonRead < 1000) {
    postMessageToSlack(slack_message);
    delay(1000);
  }
  
}
