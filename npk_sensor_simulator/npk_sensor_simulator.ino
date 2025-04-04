#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "html_content.h"

#define BUTTON_PIN D2
#define ESP_RESET D3

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";
ESP8266WebServer server(80);

String nitrogenValue = "";
String phosphorusValue = "";
String potassiumValue = "";
String humidityValue = "";
String pHValue = "7.0";
String rainfallValue = "";
String temperatureValue = "";

const int m = 22, n = 7;
float dataMatrix[m][n] = {
    {90,42,43,20.87974371,82.00274423,6.502985292,202.9355362},
    {77,58,19,22.8056033,56.50768935,5.791649933,101.5952794},
    {57,60,84,19.1034283,17.26184541,6.586777189,75.49101167},
    {24,67,22,20.120043,22.89845607,5.618844277,104.6252153},
    {40,70,20,31.80130272,45.03186173,5.623490043,147.0361442},
    {39,36,22,29.34317422,60.50320928,9.072011412,34.03335472},
    {34,59,23,28.56212158,83.24855855,6.935804256,56.48265193},
    {25,62,21,26.73433965,68.13999721,7.040056094,67.15096376},
    {8,58,17,28.75273118,69.15640149,7.286049978,35.15426171},
    {25,27,41,19.20090378,94.27659596,6.923509371,108.0423555},
    {101,87,54,29.07311132,76.50045221,6.376756633,100.1692639},
    {12,37,30,31.09779147,47.41196659,4.546466109,90.28624348},
    {25,121,201,30.50734778,82.71775569,5.594240603,70.08200379},
    {99,5,47,24.13078816,84.84494575,6.649086972,51.19470197},
    {106,10,49,27.72653142,92.00687531,6.350623739,20.21126747},
    {28,123,202,22.76643029,92.12438519,6.442289294,120.4359949},
    {31,25,12,18.05142392,90.03969587,7.016482298,111.7793889},
    {68,62,50,33.20258348,92.76437927,6.977700268,197.5282582},
    {11,6,25,28.69164799,96.65248672,6.081568052,178.9635457},
    {135,43,16,23.47986888,81.73049149,6.720449769,86.76287924},
    {82,40,45,26.21312799,81.70476368,6.667633355,180.1237765},
    {104,20,26,27.22783677,52.95261751,7.493191968,175.7260273}
};

//N	P	K	temperature	humidity	ph	rainfall	label


void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

void handleSubmit() {
  nitrogenValue = server.arg("nitrogenValue");
  phosphorusValue = server.arg("phosphorusValue");
  potassiumValue = server.arg("potassiumValue");
  humidityValue = server.arg("humidityValue");
  pHValue = server.arg("pHValue");
  rainfallValue = server.arg("rainfallValue");
  temperatureValue = server.arg("temperatureValue");
  digitalWrite(ESP_RESET,0);
  delay(100);
  digitalWrite(ESP_RESET,1);

  Serial.println(nitrogenValue + "," + phosphorusValue + "," + potassiumValue + "," + temperatureValue + "," + humidityValue +","+ pHValue + "," + rainfallValue);
  server.send(200, "text/plain", "Data received");
}

void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ESP_RESET,OUTPUT);
}

void loop() {
  server.handleClient();
  if (digitalRead(BUTTON_PIN) == LOW) {  // Button Pressed
        digitalWrite(ESP_RESET,0);
        delay(100);
        digitalWrite(ESP_RESET,1);
        if (digitalRead(BUTTON_PIN) == LOW) { 
            
            int randomRow = random(0, m);  // Select a random row

            // Construct data packet
            String dataPacket = "";
            for (int j = 0; j < n; j++) {
                dataPacket += String(dataMatrix[randomRow][j], 6); // Keep 6 decimal places
                if (j < n - 1) {
                    dataPacket += ",";  // Add comma between values
                }
            }

            Serial.println(dataPacket); // Send row to ESP32
            delay(500); // Prevent multiple presses
        }
    }
}