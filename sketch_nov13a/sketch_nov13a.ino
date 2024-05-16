#include<DHT.h>

#define dhtPin 2

#define dhtType DHT11 
DHT dht(dhtPin, dhtType);

void setup() {

// put your setup code here, to run once:

Serial.begin(9600);

dht.begin();

}

void loop() {

// put your main code here, to run repeatedly:

float t=dht.readTemperature();

float h=dht.readHumidity();

if (isnan(t) || isnan(h)) {

Serial.println("No data Available");
}
Serial.println(t);
Serial.println(h);

}
