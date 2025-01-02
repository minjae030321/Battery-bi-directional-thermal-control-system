import processing.serial.*;

Serial myPort; // Serial port object

int waterLevel = 0; // Water level value
boolean isThermistorHigh = false; // Thermistor state

boolean motorAOn = false; // Motor A status
boolean motorBOn = false; // Motor B status
boolean manualOverride = false; // Manual override status

void setup() {
  size(400, 400); // Set window size
  println(Serial.list()); // List available serial ports
  String portName = Serial.list()[2]; // Use COM12 (index 2)
  myPort = new Serial(this, portName, 9600); // Connect to serial port
}

void draw() {
  background(200);

  // Display water level value
  fill(0);
  textSize(16);
  text("Water Level: " + waterLevel, 50, 50);

  // Display water detection status
  if (waterLevel > 200) {
    fill(0, 255, 0);
    text("Water Level Detected", 50, 80);
  } else {
    fill(255, 0, 0);
    text("Water Level Not Detected", 50, 80);
  }

  // Display thermistor state
  if (isThermistorHigh) {
    fill(0, 255, 0);
    text("Thermistor: Normal Temperature", 50, 110);
  } else {
    fill(255, 0, 0);
    text("Thermistor: High Temperature", 50, 110);
  }

  // Manual override status
  fill(0);
  text(manualOverride ? "Manual Override: ON" : "Manual Override: OFF", 50, 140);

  // Motor A button
  fill(motorAOn ? 0 : 255, motorAOn ? 255 : 0, 0);
  rect(50, 180, 100, 50);
  fill(0);
  text("Motor A", 75, 210);

  // Motor B button
  fill(motorBOn ? 0 : 255, motorBOn ? 255 : 0, 0);
  rect(250, 180, 100, 50);
  fill(0);
  text("Motor B", 275, 210);

  // Manual override button
  fill(100);
  rect(150, 300, 100, 50);
  fill(0);
  text("Toggle Override", 165, 330);
}

void mousePressed() {
  // Motor A button toggle
  if (mouseX > 50 && mouseX < 150 && mouseY > 180 && mouseY < 230) {
    motorAOn = !motorAOn;
    myPort.write(motorAOn ? 'A' : 'a'); // Send motor A command
  }

  // Motor B button toggle
  if (mouseX > 250 && mouseX < 350 && mouseY > 180 && mouseY < 230) {
    motorBOn = !motorBOn;
    myPort.write(motorBOn ? 'B' : 'b'); // Send motor B command
  }

  // Manual override button
  if (mouseX > 150 && mouseX < 250 && mouseY > 300 && mouseY < 350) {
    manualOverride = !manualOverride;
    myPort.write(manualOverride ? 'M' : 'm'); // Send override command
  }
}

void serialEvent(Serial myPort) {
  String data = myPort.readStringUntil('\n'); // Read data until newline
  if (data != null) {
    data = trim(data); // Remove whitespace
    String[] values = data.split(" "); // Split data into water and thermistor values

    if (values.length == 2) {
      // Extract water level value
      if (values[0].startsWith("W:")) {
        waterLevel = int(values[0].substring(2));
      }

      // Extract thermistor state
      if (values[1].startsWith("T:")) {
        isThermistorHigh = values[1].substring(2).equals("1");
      }
    }
  }
}
