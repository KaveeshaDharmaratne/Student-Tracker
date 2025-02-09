#include <SoftwareSerial.h>

// Define pins for software serial
#define A9G_TX 8  // Arduino pin connected to A9G RX
#define A9G_RX 9  // Arduino pin connected to A9G TX
#define DEBUG true

//AT comms defined
const char* AT_RESET = "AT+RST=1";
const char* AT_GPS_ON = "AT+GPS=1";
const char* AT_CREG = "AT+CREG=1";
const char* AT_CGATT = "AT+CGATT=1";
const char* AT_CGDCONT = "AT+CGDCONT=1,\"IP\",\"hutch3g\"";
const char* AT_CGACT = "AT+CGACT=1,1";
const char* MQTT_CONNECTION = "AT+MQTTCONN=\"io.adafruit.com\",1883,\"a9g-dev-1\",120,0,\"kaviboyzz\",\"aio_kjcJ23TZRYF5YQhpQMqOSqXnfxhC\"";

SoftwareSerial a9gSerial(A9G_RX, A9G_TX);  // RX, TX

String sendData(const String& command, const int timeout, boolean debug) {
  const int BUFFER_SIZE = 200; // Define a maximum buffer size, adjust as needed
  char buffer[BUFFER_SIZE];      // Use a char array (fixed-size buffer)
  int bufferIndex = 0;           // Index to track position in the buffer
  unsigned long startTime = millis(); // Use unsigned long for millis() comparison

  a9gSerial.println(command);

  while ((millis() - startTime) < timeout) { // More robust timeout check
    while (a9gSerial.available()) {
      if (bufferIndex < BUFFER_SIZE - 1) { // Prevent buffer overflow
        buffer[bufferIndex++] = (char)a9gSerial.read();
      } else {
        // Buffer is full, handle overflow (optional - e.g., break, truncate, error)
        Serial.println("Receive buffer overflow!"); // Debug message
        break; // Stop reading to prevent writing out of bounds
      }
    }
    if (bufferIndex > 0 && !a9gSerial.available()) {

    }
    delay(0); // Add a small delay to yield processor time (optional, but good practice)
  }

  buffer[bufferIndex] = '\0'; // Null-terminate the buffer to make it a C-string
  String temp(buffer);         // Create a String object from the buffer

  if (debug) {
    Serial.print(temp);
  }
  return temp;
}

void setup() {
 Serial.begin(115200);
  while (!Serial) {}

  a9gSerial.begin(115200);
  Serial.println("Initialized");

  // Initialize A9G
  sendData(AT_RESET, 17000, DEBUG);
  sendData("AT", 1000, DEBUG);
  sendData(AT_GPS_ON, 1000, DEBUG);
  sendData(AT_CREG, 3000, DEBUG);
  sendData(AT_CGATT, 5000, DEBUG);
  sendData(AT_CGDCONT, 7000, DEBUG);
  sendData(AT_CGACT, 7000, DEBUG);

  // MQTT Connection
  sendData("AT+MQTTDISCONN", 1000, DEBUG);
  Serial.println("Broker Disonnected!");
  sendData(MQTT_CONNECTION, 5000, DEBUG);
  Serial.println("Broker Successfully Connected!");
  
}

void fetchLocation() {
  a9gSerial.println("AT+LOCATION=2\r\n");
  String res = "";
  if(a9gSerial.available()) {
    res = a9gSerial.readStringUntil('\n');
  if (res.indexOf("GPS NOT") != -1) {
    Serial.println("No Location data");
    return;
  }
  // Parse latitude and longitude
  int commaIndex = res.indexOf(',');
  if (commaIndex != -1) {
    String lats = res.substring(0, commaIndex);
    String longi = res.substring(commaIndex + 1, 18);
    a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/lati\",\"" + lats + "\",0,0,0");
    delay(1000);
    a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/longi\",\"" + longi + "\",0,0,0");
    delay(1000);

    Serial.print("Lat - ");
    Serial.println(lats);
    Serial.print("Longi - ");
    Serial.println(longi);

    // Publish to MQTT
    String coordinates = lats + "," + longi;
    a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/gpsdata.full-data\",\"" + coordinates + "\",0,0,0");
    Serial.println("Aquired: " + coordinates);
    // mqttPub(longi, lats, coordinates);
  }
  }
  
}

void loop() {
  delay(1000);
  fetchLocation();
  delay(1000);
}
