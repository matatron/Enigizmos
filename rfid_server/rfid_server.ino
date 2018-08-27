#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Arcana";
const char* password = "2.718281";

ESP8266WebServer server(80);

const int led = 2;
const int buzzer = 16; 

constexpr uint8_t RST_PIN = 5;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte readCard[4] = {0, 0, 0, 0}; // Stores scanned ID read from RFID Module


void handleRoot() {
  digitalWrite(led, LOW);
  String message = "ID : ";
  int suma = 0;
  mfrc522.PICC_IsNewCardPresent();

  if (mfrc522.PICC_ReadCardSerial()) {
    for ( uint8_t i = 0; i < 4; i++) {
      readCard[i] = mfrc522.uid.uidByte[i];
    }
    mfrc522.PICC_HaltA(); // Stop reading
  }


  for ( uint8_t i = 0; i < 4; i++) {
    message += String(readCard[i]) + " ";
    suma += readCard[i];
    Serial.print(readCard[i]);
    Serial.print(" ");
  }

  message += " Suma: " + String(suma);
  server.send(200, "text/plain", message);
  Serial.println("");
  tone(buzzer, 450);
  delay(100);
  noTone(buzzer);
  digitalWrite(led, HIGH);
}


void setup() {
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(led, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.begin();
  tone(buzzer, 450);
  delay(100);
  noTone(buzzer);
  
  Serial.println("HTTP server started");
  digitalWrite(led, HIGH);

}

void loop() {
  server.handleClient();
}
