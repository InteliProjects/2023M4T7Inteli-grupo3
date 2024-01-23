// Include libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "UbidotsEsp32Mqtt.h"

#define CHANNEL 11 // Channel for the WiFi config

// Define constants
const char *UBIDOTS_TOKEN = "BBUS-ibsXHCYycj9ENZXkfwxJOQ4AOCkxgR";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Inteli-welcome";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "consultorio-001";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "consultorio-001"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 1000; // Update rate in milliseconds

unsigned long timer; // Timer to define how much time has passed
int flag = 0; // State variable to define witch wifi connection is being used
String message = ""; // Message I want to send to Ubidots by context
int value = 0; // value that is being send with the context

// Instantiate Ubidots
Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}
// config AP SSID
void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
}

void setupEspNow() {
  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);

  flag = 2;
}

// function to setup the MQTT with Ubidots
void setupUbidots() {
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback); 
  ubidots.setup();
  ubidots.reconnect();
}

void setup() {
  Serial.begin(115200);
  setupUbidots();
}

// callback when data is received from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  Serial.print("Last Packet Recv Data: ");
  Serial.println((char*)data);
  Serial.println("");
  // Convert data to string
  String receivedData = String((char*)data);
  message = receivedData;

  esp_now_deinit(); // Stop esp now
  WiFi.disconnect(); // Disconnect from Wifi

  flag = 0; // Change the state for 0, so Ubidots
}
void loop() {
  if (flag == 0) { // Run Ubidots loop
    if (!ubidots.connected())
  {
    setupUbidots();
    ubidots.reconnect();
  }
  char* character = (char*)malloc(sizeof(char) * 10);

  sprintf(character, "%s", message);

  char* context = (char*)malloc(sizeof(char) * 30);

  ubidots.addContext("id", character);

  ubidots.getContext(context);

  ubidots.add(VARIABLE_LABEL, value, context);
  
  ubidots.publish(DEVICE_LABEL);
  timer = millis();

  flag = 1;
  ubidots.disconnect();
  } else if (flag == 1) { // Run setup Esp Now
    setupEspNow();
  }
  // Run Esp Now. It's a callback function
}