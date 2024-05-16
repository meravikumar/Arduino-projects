#include <OneWire.h>
#include <DallasTemperature.h>

const int pH_pins[] = {A0, A1, A2}; // Analog pins for the pH sensors
const int turbidity_pins[] = {A3, A4, A5}; // Analog pins for the turbidity sensors
const int waterFlowPin = 2;
volatile int pulseCount = 0; // Digital pin for the water flow sensor

float flowRate;                // Variable to store the flow rate
unsigned int flowMilliLiters = 0; // Variable to store the total flow

unsigned long oldTime = 0;

#define ONE_WIRE_BUS 5 // Data pin DQ pin of DS18B20 connected to digital pin D5

OneWire oneWire(ONE_WIRE_BUS); // Set up one wire instance
DallasTemperature sensors(&oneWire); // pass one wire reference to DS18B20 library

long phTot;
float phAvg;
int x;
const float C = 21.34;     // Constant of straight line (Y = mx + C)
const float m = -5.70;     // Slope of straight line (Y = mx + C)
const float calibrationOffset = 10.0; // Calibration offset to adjust pH readings

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(waterFlowPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(waterFlowPin), pulseCounter, FALLING); // Set water flow sensor pin as input
  sensors.begin(); // Start the DS18B20 Library
}

void loop() {
  // Read pH, turbidity, and flow values
  float pHValues[3];
  float turbidityValues[3];

  for (int i = 0; i < 3; i++) {
    pHValues[i] = readPHValue(pH_pins[i]);
    turbidityValues[i] = readTurbidityValue(turbidity_pins[i]);
  }

  // Calculate the time since the last pulse
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - oldTime;

  if (elapsedTime >= 1000) { // Update every 1 second
    detachInterrupt(digitalPinToInterrupt(waterFlowPin)); // Disable interrupts while calculating flow rate

    // Calculate the flow rate in liters per minute
    flowRate = (pulseCount / 7.5); // YF-S201 sensor outputs approximately 7.5 pulses per liter

    // Calculate the total flow in milliliters
    flowMilliLiters = (pulseCount / 7.5) * 1000;

    // Send data through serial
    for (int i = 0; i < 3; i++) {
      Serial.print(pHValues[i]);
      Serial.print(",");
      Serial.print(turbidityValues[i]);
      Serial.print(",");
    }
    Serial.println(flowRate);

    pulseCount = 0;   // Reset the pulse count
    oldTime = currentTime; // Update the last pulse time

    attachInterrupt(digitalPinToInterrupt(waterFlowPin), pulseCounter, FALLING); // Enable interrupts
  }

  delay(1000); // Adjust the delay as needed for your application
}

// Function to convert voltage to pH
float readPHValue(int pin) {
  phTot = 0;
  phAvg = 0;

  // taking 10 samples and adding with 10 milliseconds delay
  for (x = 0; x < 10; x++) {
    int analogValue = analogRead(pin);
    phTot += analogValue;
    delay(10);
  }

  phAvg = phTot / 10;
  float phVoltage = phAvg * (5.0 / 1023.0); // convert sensor reading into millivolt
  float pHValue = phVoltage * m + C + calibrationOffset; // Adjusted for calibration

  return pHValue;
}

// Function to read turbidity value
float readTurbidityValue(int pin) {
  int sensorValue = analogRead(pin);
  // Replace this with the actual conversion formula for your turbidity sensor
  // Refer to your sensor's documentation for the correct conversion formula
  float turbidityValue = map(sensorValue, 0, 1023, 0, 100); // Example mapping for illustration

  return turbidityValue;
}

void pulseCounter() {
  pulseCount++; // Increment the pulse count when a pulse is detected
}