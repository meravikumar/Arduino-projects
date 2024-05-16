const int analogInputPin = A0;  // Analog pin for the pH sensor
const float referenceVoltage = 5.0;  // The reference voltage of your Arduino (usually 5V)

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Read the analog voltage from the pH sensor
  int sensorValue = analogRead(analogInputPin);

  // Convert the analog voltage to pH
  float voltage = (sensorValue / 1024.0) * referenceVoltage;
  float pHValue = getPHFromVoltage(voltage);

  // Print the pH value
  Serial.print("pH Value: ");
  Serial.println(pHValue);

  delay(1000);  // Adjust the delay as needed for your application
}

// Function to convert voltage to pH (adjust this based on your sensor's characteristics)

  float getPHFromVoltage(float voltage) {
  // Replace the following constants with your calibration coefficients
  float slope = 3.5;  // Example slope (replace with actual calibration value)
  float intercept = -1.5;  // Example intercept (replace with actual calibration value)

  // Calculate pH using the linear calibration formula: pH = m * voltage + b
  float pHValue = (slope * voltage) + intercept;

  return pHValue;
}
  
