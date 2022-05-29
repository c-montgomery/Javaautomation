/*
  Basic ESP8266 MQTT example
  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.
 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include "pass.h"

#define ledPin 5

#define time 0

// Update these with values suitable for your network.

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = "192.168.0.13";

boolean isOn = false;
WiFiClient espClient;

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived [");
  Serial.print("channel1/data1");
  Serial.print("] ");

  Serial.println();
  
  if (payload[0] == 0 || payload[0] == 1 || payload[0] == 2){
      Serial.println("it's  O'clock");
    }else if(payload[0] == 'l' && payload[1] == 'i') { //Checks first 2 chars of payload
      Serial.println("in else if in callback function");
    toggle(D7);//alter nates state of selected pin
    
  }

  //  // Switch on the LED if an 1 was received as first character
  //  if ((char)payload[0] == '1') {
  //    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  //    // but actually the LED is on; this is because
  //    // it is active low on the ESP-01)
  //  } else {
  //    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  //  }

}



PubSubClient client(mqtt_server, 1883, callback,espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void toggle(int arg) {
  if (isOn) {
    isOn = false;
    digitalWrite(arg, LOW);
  } else {
    isOn = true;
    digitalWrite(arg, HIGH);
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
       client.publish("channel1/data1", "esp8266 functioning ");
      // ... and resubscribe
      client.subscribe("time");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);     // Initialize the ledPin as an output
 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 6000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("channel1/data1", "time");
    client.subscribe("time");
    client.setCallback(callback);
  }
}
