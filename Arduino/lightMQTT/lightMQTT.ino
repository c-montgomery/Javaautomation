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
#include "include/pass.h"

#define pin1 5
#define pin2 4

int currentTime = 0;

boolean isLightOn = false;

// Update these with values suitable for your network.

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = "192.168.0.25";

WiFiClient espClient;

//////Function performed when subscribed topic publishes message
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print("channel1/data1");
  Serial.print("] ");
  int myInts[] = {(payload[0]-'0'),(payload[1]-'0')};
  Serial.print(payload[0]);
  int tensPlace =(myInts[0]*10);
  int onesPlace = (myInts[1]);
  int total = tensPlace + onesPlace;
  currentTime = total;
  Serial.println(total);  
  if (payload[0] == '0' || payload[0] == '1' || payload[0] == '2') {
    
    //Serial.println("it's " + total + "  O'clock");
   
  } else if (payload[0] == 'l' && payload[1] == 'i') {  //Checks first 2 chars of payload
    Serial.println("in else if in callback function");
  }
}



PubSubClient client(mqtt_server, 1883, callback, espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("WIFI STATIS");
    Serial.println(WiFi.status() );
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266-";
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
  pinMode(pin1, OUTPUT);  // Initialize pin1 as an output

  Serial.begin(74880);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.subscribe("time");
  client.setCallback(callback);
}

void loop() {

  unsigned long now = millis();
  if (now - lastMsg > 6000) {
    lastMsg = now;
    ++value;
    snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
   
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("channel1/data1", "time");
    
    
  }
 
  if (!client.connected()) {
    reconnect();
  } else if (currentTime >= 18 && currentTime <= 23) {
    digitalWrite(pin1, HIGH);
  }
  client.loop();

}
