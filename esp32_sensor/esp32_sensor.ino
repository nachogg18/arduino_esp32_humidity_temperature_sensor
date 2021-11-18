#include "DHTesp.h"
#include <HTTPClient.h>

const char* SSID = "************";
const char* PASSWORD = "**********";
const String SERVER_URL = "****************";
const String API_KEY = "*******************";
const String HUMIDITY_SENSOR_NAME = "*********************";
const String TEMPERATURE_SENSOR_NAME = "**********************";
const String TEMPERATURE_REFERENCE = "t";
const String HUMIDITY_REFERENCE = "h";
const String SEND_MEASURE_MESSAGE_REQUEST = "Sending measure ";
const String SEND_MEASURE_MESSAGE_RESPONSE = "Response for measure sending: ";
const int p14 = *;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  setup_pin(14);
}

void loop() {
  // put your main code here, to run repeatedly:
  TempAndHumidity measures = getMeasuresFromSensor();
  
  String temperature = getTemperature(measures);
  
  String humidity = getHumidity(measures);

  sendMeasure(temperature, TEMPERATURE_REFERENCE, TEMPERATURE_SENSOR_NAME);
  
  sendMeasure(humidity, HUMIDITY_REFERENCE, HUMIDITY_SENSOR_NAME);
  
  delay(15000);
}


void sendMeasure(String measure, String measureUnit, String sensorName) {
  String endpoint = SERVER_URL + "d?k=" + API_KEY + "&i=" + sensorName;
  
  http.begin(client, endpoint);
  
  http.addHeader("Content-Type", "text/plain"); 
  
  String httpRequestData = measureUnit + "|" + measure;

  Serial.println(SEND_MEASURE_MESSAGE_REQUEST + httpRequestData + " from sensor: " + sensorName );
  
  int returnedCode = http.POST(httpRequestData);
  
  String response = http.getString();  
  
  Serial.println(SEND_MEASURE_MESSAGE_RESPONSE + String(response) + " - " + String(returnedCode));
}


void setup_pin(int pinNumber) {
  dht.setup(pinNumber, DHTesp::DHT11);
}

TempAndHumidity getMeasuresFromSensor(){
  return dht.getTempAndHumidity();
}

String getHumidity(TempAndHumidity measures) {
  return String(measures.humidity,0);
}

String getTemperature(TempAndHumidity measures) {
  return String(measures.temperature,0);
}
