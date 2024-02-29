#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <ESPDash.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 2
#define ONE_WIRE_BUS D3

bool led1IsOn = false;
bool led2IsOn = false;

const int LED_1 = LED_BUILTIN; //D0
const int LED_2 = D1; //D1


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

/* Your WiFi Credentials */
const char* ssid = "iphoneA"; // SSID
const char* password = "qaz15243wsx"; // Password
float temperatureC = 0.0f;
/* Start Webserver */
ESP8266WebServer server(80);
/* Attach ESP-DASH to AsyncWebServer */
// ESPDash dashboard(&server); 

// Card tempC(&dashboard, TEMPERATURE_CARD, "Temperature in C", "°C");
// Card tempF(&dashboard, TEMPERATURE_CARD, "Temperature in F", "°F");

  void setLedState(int led, bool state){
    if(!state){
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);}
  }

  void controller(){
    server.on ("/temperature", [](){
      sensors.requestTemperatures();
      temperatureC = sensors.getTempCByIndex(0);
      Serial.println("Showing temperature");
      Serial.println(temperatureC);
      server.send(200, "text/html", String(temperatureC));
    });
    server.on ("/led1", [](){
      setLedState(LED_1, led1IsOn);
      led1IsOn = !led1IsOn;
    });
    server.on ("/led2", [](){
      setLedState(LED_2, led2IsOn);
      led2IsOn = !led2IsOn;
    });
  }

void setup() {
  Serial.begin(9600);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  sensors.begin();

  /* Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
      Serial.printf("WiFi Connection Failed!\n");
      return;
  }
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
  
  controller();
}

void loop() 
{

  server.handleClient();

  // float temperatureC = sensors.getTempCByIndex(0);
  //   Serial.print("Temp. in Celsius: ");
  //   Serial.println(temperatureC);
  
  // float temperatureF = sensors.getTempFByIndex(0);
  //   Serial.print("Temp. in Fahrenheit: ");
  //   Serial.println(temperatureF);
  
  // /* Update Card Values */
  // tempC.update(temperatureC,"°C");
  // tempF.update(temperatureF,"°F");
  // /* Send Updates to our Dashboard (realtime) */
  // dashboard.sendUpdates();
  // delay(3000);
}