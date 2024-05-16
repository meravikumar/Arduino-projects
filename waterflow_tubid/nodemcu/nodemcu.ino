#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>
// String URL="http://api.thingspeak.com/update?api_key=SIXCYKVXO9J5W53N&field1=";
String URL="http://api.thingspeak.com/update?api_key=L0UVGBRVS52JG27E&field1=";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.print("Start connection");
  WiFi.begin("Ravi","098765432");
  while((!(WiFi.status()== WL_CONNECTED))){
      delay(200);
      Serial.print("..");
    }
  Serial.println("Connected");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String data=Serial.readStringUntil('\n');
    Serial.println(data);
    int comma=data.indexOf(',');
    if(comma!=-1){
      float t=data.substring(0,comma).toFloat();
      float h=data.substring(comma+1).toFloat();
      float tub=data.substring(comma+2).toFloat();
      // Serial.print(t);
      // Serial.print(h);
      // Serial.print(tub);
      //Serial.println("tubidity>flowrate>flowmilliliters");
      sendData(t,h,tub);
    }
  }

}

void sendData(float t,float h,float tub){
 WiFiClient client;
 HTTPClient http;
String newUrl=URL+String(t)+"&field2="+String(h)+"&field3="+String(tub);
  http.begin(client,newUrl);
  int responsecode=http.GET();
  String data=http.getString();
  Serial.println("tubidity>flowrate>flowmilliliters");
  Serial.println(data);
  http.end();

}