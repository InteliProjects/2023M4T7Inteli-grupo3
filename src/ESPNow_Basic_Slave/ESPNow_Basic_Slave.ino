#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço I2C do LCD (pode variar)
#define I2C_ADDRESS 0x27

// Cria uma instância do objeto LiquidCrystal_I2C
LiquidCrystal_I2C lcd(I2C_ADDRESS, 16, 2);

#define CHANNEL 1
#define led 25
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
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  // Inicializa o LCD
  lcd.init();
  // Liga o backlight (opcional)
  lcd.backlight();

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
}
// callback when data is recv from Master
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
  // Control the LED based on the received data
  if (receivedData.equals("on")) {
    digitalWrite(led, HIGH);  // turn the LED on
    // Define o cursor na posição 0,0
    lcd.setCursor(0, 0);
    lcd.clear();
    // Escreve "Hello, World!" no LCD
    lcd.printf("STATUS: %s", receivedData);
  } else if (receivedData.equals("off")) {
    digitalWrite(led, LOW);   // turn the LED off
    lcd.clear();
    lcd.printf("STATUS: %s", receivedData);
  }
}
void loop() {
}