#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

#define sampleTime 1000
#define port 1883
#define  LED  2
#define ToSecond(x) x/1000

const char* ssid = "Figueroa39_2.4GHz";
const char* password = "Nahuvsnacho97*";
const char* mqtt_server = "test.mosquitto.org";
const char* Topic = "Invap/Temperatura";
float Temperatura = 0, lastMsg = 0, now = 0;
int ID = 1, muestra = 0;

void setup_wifi();
void callback(char* topic, byte* payload, int length);

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, int length) {
  Serial.println();
  if ((char)payload[0] == '1') {
    digitalWrite(LED, LOW);  
  } else {
    digitalWrite(LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  now = millis();
  if (now - lastMsg > sampleTime) {
    Temperatura = analogRead(A0);
    lastMsg = now;
    sprintf (msg,"%d,%d,%.2f,%.2f", muestra, ID, Temperatura, float(ToSecond(lastMsg)));
    Serial.println(msg);
    client.publish(Topic, msg);
    muestra++;
  }
}