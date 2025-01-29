#include <SoftwareSerial.h>

// Define pins for software serial
#define A9G_TX 8  // Arduino pin connected to A9G RX
#define A9G_RX 9  // Arduino pin connected to A9G TX
#define DEBUG true

// AT Commands
const char* AT_RESET = "AT+RST=1";
const char* AT_GPS_ON = "AT+GPS=1";
const char* AT_CREG = "AT+CREG=1";
const char* AT_CGATT = "AT+CGATT=1";
const char* AT_CGDCONT = "AT+CGDCONT=1,\"IP\",\"hutch3g\"";
const char* AT_CGACT = "AT+CGACT=1,1";
const char* MQTT_CONNECTION = "AT+MQTTCONN=\"io.adafruit.com\",1883,\"a9g-dev-1\",120,0,\"kaviboyzz\",\"aio_WJVa518zQw2yWvhcWGeYeks1mBmS\"";

SoftwareSerial a9gSerial(A9G_RX, A9G_TX); // RX, TX

String res = "";
String location_data;
String lats;
String longi;

String sendData(const String& command, const int timeout, boolean debug) {
    String temp = "";
    a9gSerial.println(command);
    long int time = millis();
    while ((time + timeout) > millis()) {
        while (a9gSerial.available()) {
            temp += (char)a9gSerial.read();
        }
    }
    if (debug) {
        Serial.print(temp);
    }
    return temp;
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    a9gSerial.begin(115200);
    Serial.println("Initializing A9G communication...");
    
    // Initialize A9G
    sendData(AT_RESET, 15000, DEBUG);
    sendData("AT", 1000, DEBUG);
    sendData(AT_GPS_ON, 1000, DEBUG);
    sendData(AT_CREG, 1000, DEBUG);
    sendData(AT_CGATT, 3000, DEBUG);
    sendData(AT_CGDCONT, 5000, DEBUG);
    sendData(AT_CGACT, 5000, DEBUG);
    
    // MQTT Connection
    sendData("AT+MQTTDISCONN", 1000, DEBUG);
    sendData(MQTT_CONNECTION, 5000, DEBUG);
    Serial.println("Broker Successfully Connected!");
}

void fetchLocation() {
    a9gSerial.println("AT+LOCATION=2\r\n");
    delay(2000);
    
    while (!a9gSerial.available()) {

        delay(10);
    }
    
    res = a9gSerial.readStringUntil('\n');
    if (res.indexOf("GPS NOT") != -1) {
        Serial.println("No Location data");
        return;
    }

    // Parse latitude and longitude
    int commaIndex = res.indexOf(',');
    if (commaIndex != -1) {
        lats = res.substring(0, commaIndex);
        longi = res.substring(commaIndex + 1,17);
        Serial.print("Lat - "); Serial.println(lats);
        Serial.print("Longi - "); Serial.println(longi);
        
        // Publish to MQTT
        String coordinates = lats + "," + longi;
        // a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/location\",\"" + coordinates + "\",0,0,0");
        a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/gpsdata.latitude\",\"" + lats + "\",0,0,0");
        delay(500);
        a9gSerial.println("AT+MQTTPUB=\"kaviboyzz/feeds/gpsdata.longitude\",\"" + longi + "\",0,0,0");
        Serial.println("Published!");
    }
}

void loop() {
    fetchLocation();
    delay(3000);
}