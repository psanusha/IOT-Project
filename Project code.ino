#include "HX711_ADC.h"

const int HX711_dout = 4;    // MCU > HX711 dout pin
const int HX711_sck = 5;     // MCU > HX711 sck pin
const int buzzerPin = 6;     // MCU > Buzzer pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

void setup() {
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  LoadCell.setCalFactor(-1356.12); // Set the calibration factor

  unsigned long stabilizingTime = 2000;
  boolean tarePerformed = true;
  LoadCell.start(stabilizingTime, tarePerformed);

  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU > HX711 wiring and pin designations");
    while (1);
  } else {
    Serial.println("Startup is complete");
  }

  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Turn off the buzzer initially
}

void loop() {
  if (LoadCell.update()) {
    float weight = LoadCell.getData();
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");

    if (weight <95) {
      digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
      delay(1000);                    // Beep for 1 second
      digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
    }
  }
}
