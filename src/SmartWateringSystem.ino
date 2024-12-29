#include <WiFi.h>
#include <ThingSpeak.h>

// WiFi configuration
const char *ssid = "Wokwi-GUEST";
const char *password = "";
unsigned long channelID = 2792440;  // Replace with your ThingSpeak channel ID
const char *writeAPIKey = "V80DG7746KGWOZAB"; // Replace with your Write API Key

// Pin Definitions
int soilMoisturePin = 34;  // Soil moisture sensor pin
int pumpPin = 2;           // Water pump control pin (GPIO 2)
int moistureThreshold = 400;  // Threshold for turning the pump on/off

// ThingSpeak Setup
WiFiClient client;

void setup() {
  // Serial Setup
  Serial.begin(115200);

  // WiFi Setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Pin Setup
  pinMode(pumpPin, OUTPUT);
  pinMode(soilMoisturePin, INPUT);
}

void loop() {
  // Read Soil Moisture Sensor Value
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);

  // Control the pump (LED)
  if (soilMoistureValue < moistureThreshold) {
    digitalWrite(pumpPin, HIGH); // Turn LED (pump) ON
    Serial.println("Pump ON (LED ON)");
  } else {
    digitalWrite(pumpPin, LOW);  // Turn LED (pump) OFF
    Serial.println("Pump OFF (LED OFF)");
  }

  // Log Data to ThingSpeak
  ThingSpeak.setField(1, soilMoistureValue);
  ThingSpeak.writeFields(channelID, writeAPIKey);  // Send data to ThingSpeak
  Serial.println("Data sent to ThingSpeak!");

  // Wait 15 seconds before the next iteration (ThingSpeak API rate limit)
  delay(15000);
}
