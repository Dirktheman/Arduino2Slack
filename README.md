
  Create a physical button to send a realtime Slack notification
  Copryright 2018 Dirk de Man (Twitter: @dirktheman)
  Use under MIT license
  =========================================================================
  
  BILL OF MATERIALS
  - NodeMCU
  - A big red button/Easy button, or, well, any nice button will do I guess...
   =========================================================================
  
  INSTRUCTIONS
  - Screw open the button
  - Locate the negative (-) on the battery side and the positive (+) on the sound module side. Use your multimeter to check if there's a current flowing on the positive side when you push the button (and only then!)
  - Solder to wires to the connections
  - Connect the negative wire to the GND pin of your NodeMCU
  - Connect the positive wire to the analog pin (A0) of your NodeMCU
 =========================================================================
 
  - Create a new Incoming Webhook in Slack: Manage - Custom Integrations - Incoming Webhooks
  - Choose the Slack channel where you want to post your message in
  - Save and copy the webhook URL
   =========================================================================
   
  - Fill in your Wifi SSID (network name) and password in the Arduino code 
  - Fill in your webhook URL, a link to a nice icon, your text and the username of your custom integration
   =========================================================================
   
  - Upload the code
