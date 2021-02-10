/*
   DEEPSLEEP
   En esta version se despierta desde sleep por medio del bang del attiny85 que lee interrupcion de HW.
   Envia el json parametrizado
*/

//#define LED 2
#define SENSOR 2
#define DEBUG
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
byte mac[6];
String ID = "";
WiFiClient  client;




//Your Domain name with URL path or IP address with path
const char* serverName = "http://data.sector7gp.com/save.php/";

void setup() {

#ifdef LED
  pinMode(LED, OUTPUT);
#endif

#ifdef SENSOR
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(SENSOR, INPUT_PULLUP);
#endif


  stat(2);

    IPAddress ip(192, 168 , 1 , 249);
    IPAddress dns(8, 8, 8, 8);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress gateway(192, 168, 1, 1);
    WiFi.config(ip, dns, gateway, subnet);

  WiFi.macAddress(mac);
  Serial.begin(115200);  // Initialize serial


  Serial.println("");
  //Sets the chip ID
  Serial.print("ID: ");
  if(mac[3] < 10) ID = "0";
  ID = ID + String (mac[3],HEX) + String( + mac[4], HEX) + String(mac[5], HEX);
  ID.toUpperCase();
  Serial.println(ID);

}

//Reset function
//void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {
  stat(3);
  if (1) {
    checkOnline();

    HTTPClient http;

    String toPost = "{\"clientId\":\"1\",\"deviceId\":\"" + ID + "\",\"value\":\"" + digitalRead(SENSOR) + "\"}";

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(toPost);

#ifdef DEBUG
    Serial.println(toPost);
#endif

    if (httpResponseCode != 200) {
      stat(3);
    } else {
      stat(1);
    }

    // Free resources
    http.end();

#ifdef DEBUG
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Sensor: ");
    Serial.println(digitalRead(SENSOR));
    Serial.println("Deep Sleep");
#endif

    ESP.deepSleep(0);
    delay(100);
  }
}


void checkOnline() {
  // Connect or reconnect to WiFi
  char timeOut = 0;
  //WiFi.setOutputPower(20.5); //reduces the output power radio
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      stat(0);
      delay(5000);
      if (++timeOut > 5) {
        WiFi.disconnect();
        Serial.print("Fail to connect");
        Serial.println("Deep Sleep");
        ESP.deepSleep(0);
        delay(100);
      }
    }
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
  }

}
/* DOS destellos cortos, intentando conectar al WiFi
   DOS LARGOS error al envio
   TRES destellos cortos envio correcto
   CUATRO CORTOS secuencia de enciendido
   DOS LARGOS error al envio
*/
void stat(byte _s) {
#ifdef LED
  int t = 50;
  switch (_s) {
    case 0:
      for (int i = 0; i < 2 ; i++) {

        digitalWrite(LED, HIGH);
        delay(t);
        digitalWrite(LED, LOW);
        delay(t);
      }
      break;
    case 1:
      for (int i = 0; i < 3 ; i++) {
        digitalWrite(LED, HIGH);
        delay(t);
        digitalWrite(LED, LOW);
        delay(t);
      }
      break;
    case 2:
      for (int i = 0; i < 4 ; i++) {
        digitalWrite(LED, HIGH);
        delay(t);
        digitalWrite(LED, LOW);
        delay(t);
      }
      break;
    case 3:
      for (int i = 0; i < 2 ; i++) {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
      }
      break;
    default:
      //nothing
      break;
  }
#endif

}
