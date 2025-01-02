// Arduino Code

#define IN1 9   // Motor A direction control pin 1
#define IN2 8   // Motor A direction control pin 2
#define IN3 4   // Motor B direction control pin 1
#define IN4 3   // Motor B direction control pin 2

int sensorPin = A0;       // Water level sensor connected to analog pin
int sensorValue = 0;      // Variable to store water level sensor value

const int thermistorPin = 6; // Digital pin for thermistor
int thermistorState = 0;     // Variable to store thermistor state (HIGH or LOW)

char command = '0'; // Command received from Processing
boolean manualOverride = false; // Manual override flag

void setup() {
  // Motor Pins Setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor Pin Setup
  pinMode(thermistorPin, INPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read sensor values
  sensorValue = analogRead(sensorPin);
  thermistorState = digitalRead(thermistorPin);

  // Check for commands from Processing
  if (Serial.available() > 0) {
    command = Serial.read();

    if (command == 'M') { // Enable manual override
      manualOverride = true;
    } else if (command == 'm') { // Disable manual override
      manualOverride = false;
    } else if (manualOverride) {
      // Manual motor controls
      if (command == 'A') {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
      } else if (command == 'a') {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      } else if (command == 'B') {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      } else if (command == 'b') {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }
    }
  }

  if (!manualOverride) {
    // Automatic control logic
    if (sensorValue < 200) { // Situation 1: Water not detected
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH); // Turn on Motor B to fill water
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW); // Ensure Motor A is off
    } else { // Water detected
      if (thermistorState == LOW) { // Situation 2: Thermistor detects high temperature (reversed logic)
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH); // Turn on Motor A
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); // Turn on Motor B
      } else { // Situation 3: Thermistor detects normal temperature (reversed logic)
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW); // Turn off Motor A

        if (sensorValue < 200) {
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH); // Turn on Motor B to fill water
        } else {
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW); // Turn off Motor B
        }
      }
    }
  }

  // Send sensor values to Processing
  Serial.print("W:");
  Serial.print(sensorValue);
  Serial.print(" T:");
  Serial.println(thermistorState);

  delay(500); // Wait 500ms between readings
}
