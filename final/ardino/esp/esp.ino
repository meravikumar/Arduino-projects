#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

String URL = "http://api.thingspeak.com/update?api_key=0GQKJYVLRIK58E44&field1=";

void setup() {
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.print("Start connection");
  WiFi.begin("Ravi", "098765432");
  while (!(WiFi.status() == WL_CONNECTED)) {
    delay(200);
    Serial.print("..");
  }
  Serial.println("Connected");
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);

    // Parse the data (assuming it's in the format "t,h,tub,field4,field5,field6,field7")
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);
    int fourthComma = data.indexOf(',', thirdComma + 1);
    int fifthComma = data.indexOf(',', fourthComma + 1);
    int sixthComma = data.indexOf(',', fifthComma + 1);

    if (firstComma != -1 && secondComma != -1 && thirdComma != -1 && fourthComma != -1 &&
        fifthComma != -1 && sixthComma != -1) {
      
      float t = data.substring(0, firstComma).toFloat();
      float h = data.substring(firstComma + 1, secondComma).toFloat();
      float tub = data.substring(secondComma + 1, thirdComma).toFloat();
      float field4 = data.substring(thirdComma + 1, fourthComma).toFloat();
      float field5 = data.substring(fourthComma + 1, fifthComma).toFloat();
      float field6 = data.substring(fifthComma + 1, sixthComma).toFloat();
      float field7 = data.substring(sixthComma + 1).toFloat();

      // Send data to ThingSpeak
      sendData(t, h, tub, field4, field5, field6, field7);
    }
}
}

void sendData(float t, float h, float tub, float field4, float field5, float field6, float field7) {
  WiFiClient client;
  HTTPClient http;
  String newUrl = URL + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(tub)
                 + "&field4=" + String(field4) + "&field5=" + String(field5) + "&field6=" + String(field6)
                 + "&field7=" + String(field7);

  // Make an HTTP request to ThingSpeak
  http.begin(client, newUrl);
  int responsecode = http.GET();
  String data = http.getString();

  // Print the response to the serial monitor
  Serial.println("PH1, Tubidity1, ph2, Turbidity2, ph3, turbidity3, Flowrate");
  Serial.println(data);

  http.end();
}
