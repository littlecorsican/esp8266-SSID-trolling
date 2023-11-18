#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
int numOfChar=50;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Set up SSID name</h2>
  <p>
    <form method="POST" action="">
      <input type="text" name="ssid_name" id="ssid_name" />
      <input type="submit"  value="submit" />
    </form>
  </p>
</body>
</html>)rawliteral";


void setup()
{
  Serial.begin(115200);
  Serial.println();

  EEPROM.begin(51);  //Initialize EEPROM

  String strText;   
  for(int i=0;i<50;i++) 
  {
    if (EEPROM.read(0x0F+i) != 255 && EEPROM.read(0x0F+i) != 0) {
      strText = strText + char(EEPROM.read(0x0F+i)); //Read one by one with starting address of 0x0F    
      // Serial.print(EEPROM.read(0x0F+i));
      // Serial.print(" ");
      // Serial.print(char(EEPROM.read(0x0F+i)));
      // Serial.println("");
    }
  }  
 
  Serial.println(strText);
  Serial.println(strText.c_str());
  const char* ssid_ap = "adsfasdfa";

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP(strText, "12345678");
  if(result == true)
  {
    Serial.println("Ready");
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Failed!");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", "hello world");
  });

  server.on("/form", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/form", HTTP_POST, [](AsyncWebServerRequest *request){
    int paramsNr = request->params(); 
    Serial.println(paramsNr);
    Serial.println();
    
    AsyncWebParameter * j = request->getParam(0); 
    Serial.print("Size: ");
    Serial.print(j->value());
    String name = j->value();   
    Serial.println();
    //Write string to eeprom
    for(int i=0;i<name.length();i++)
    {
      EEPROM.write(0x0F+i, name[i]); //Write one by one with starting address of 0x0F
    }
    EEPROM.commit();    //Store data to EEPROM
    request->send_P(200, "text/html", name.c_str());
  });

  server.begin();
}

void loop()
{
  // Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  // delay(3000);

}



