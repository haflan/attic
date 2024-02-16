#include <ESP8266WiFi.h>
#include "d1-mini-test-credentials.ino"

// Sends a request to Hue bridge on the push of a button.
// Credentails should be put in the file 'd1-mini-test-credentials.ino' like this:
//    const char* ssid = "secret";
//    const char* password = "secret";
//    const char* host = "secret";
//    const char* url = "secret";

const char* bodyOn  = "{\"bri\":254,\"on\":true}";
const char* bodyOff = "{\"on\":false}";
const char* cLengthOn = "21";
const char* cLengthOff = "12";

const int buttonInput = 14;

bool stateOn = true;
volatile bool processing = false;

ICACHE_RAM_ATTR void buttonClicked() {
  if (processing) return;
  processing = true;
  Serial.println("Button clicked");
}

void setup() {
  pinMode(buttonInput, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonInput), buttonClicked, FALLING);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.println("Attempting connection...");
  }
  Serial.println("Connected!");
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.println("Attempting reconnection...");
  }
  delay(100);
  if (!processing) return;
  
  WiFiClient client;
  String request = String("PUT ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + (stateOn ? cLengthOn : cLengthOff) + "\r\n" +
                 "\r\n" + (stateOn ? bodyOn : bodyOff);
  if (client.connect(host, 80)) {
    client.print(request);
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
  } else {
    Serial.println("Communication failure :(");
    client.stop();
  }
  stateOn = !stateOn;
  processing = false;
}
