#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "settings.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = STAHOST;
const char* location = STALOCATION;
const uint16_t port = STAPORT;

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
void setup()
{
  Serial.begin(115200);
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  dht.setup(16, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
   // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Other settings:");
  Serial.println(String(host));
  Serial.println(String(port));
  Serial.println(String(location));

  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  delay(500);
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

  if (!client.connect(host, 54321)) {
    Serial.println("Connection failed");
    return;
  }
  
  String url = "/log/" + String(location) + "/" + String(humidity) + "/" + String(dht.toFahrenheit(temperature));
Serial.println("sending request");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  Serial.println("request sent");
  delay(150000); // should be 2 min
}

