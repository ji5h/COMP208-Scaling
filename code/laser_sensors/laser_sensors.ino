#include "Adafruit_VL53L0X.h"

// Addresses for the three sensors
#define LOXL_ADDRESS 0x40
#define LOXR_ADDRESS 0x41
#define LOXFa_ADDRESS 0x42
// #define LOXFb_ADDRESS 0x43

// Pins to control sensor shutdown
#define SHT_LOXL 6
#define SHT_LOXR 8
#define SHT_LOXFa 7
// #define SHT_LOXFb 8

// Objects for the VL53L0X sensors
Adafruit_VL53L0X loxL = Adafruit_VL53L0X();
Adafruit_VL53L0X loxR = Adafruit_VL53L0X();
Adafruit_VL53L0X loxFa = Adafruit_VL53L0X();
// Adafruit_VL53L0X loxFb = Adafruit_VL53L0X();

// Measurement data for the three sensors
VL53L0X_RangingMeasurementData_t measureL;
VL53L0X_RangingMeasurementData_t measureR;
VL53L0X_RangingMeasurementData_t measureFa;
// VL53L0X_RangingMeasurementData_t measureFb;

void setID() {
  // Reset all sensors
  digitalWrite(SHT_LOXL, LOW);
  digitalWrite(SHT_LOXR, LOW);
  digitalWrite(SHT_LOXFa, LOW);
  // digitalWrite(SHT_LOXFb, LOW);
  delay(10);
  Serial.println("pins low");
  // Activate LOX1, reset LOX2 and LOX3
  digitalWrite(SHT_LOXL, HIGH);
  Serial.println("left high");
  delay(100);
  
  // Init LOX1
  if(!loxL.begin(LOXL_ADDRESS)) {
    Serial.println("Failed to boot Third VL53L0X");
    while (1);
  }
  delay(10);
  Serial.println("write to left");

  // Activate LOX2, reset LOX1 and LOX3
  digitalWrite(SHT_LOXR, HIGH);
  delay(10);
  Serial.println("right high");

  // Init LOX2
  loxR.begin(LOXR_ADDRESS);
  delay(10);
  Serial.println("write to right");

  // Activate LOX3, reset LOX1 and LOX2
  digitalWrite(SHT_LOXFa, HIGH);
  delay(10);
  Serial.println("front a high");

  // Init LOX3
  loxFa.begin(LOXFa_ADDRESS);
  delay(10);
  Serial.println("write to front a");

  // // Activate LOX3, reset LOX1 and LOX2
  // digitalWrite(SHT_LOXFb, HIGH);
  // delay(10);
  // Serial.println("front b high");

  // // Init LOX3
  // loxFb.begin(LOXFb_ADDRESS);
  // Serial.println("write to front b");
}

void read_quad_sensors() {
  loxL.rangingTest(&measureL, false);
  loxR.rangingTest(&measureR, false);
  loxFa.rangingTest(&measureFa, false);
  // loxFb.rangingTest(&measureFb, false);

  // Print sensor one reading
  Serial.print(F("Left: "));
  Serial.print(measureL.RangeMilliMeter);
  Serial.print(F(" "));

  // Print sensor two reading
  Serial.print(F("Right: "));
  Serial.print(measureR.RangeMilliMeter);
  Serial.print(F(" "));

  // Print sensor three reading
  Serial.print(F("Front a: "));
  Serial.print(measureFa.RangeMilliMeter);
  Serial.println();

  // Serial.print(F("Front b: "));
  // Serial.print(measureFb.RangeMilliMeter);
  // Serial.println();
}

void setup() {
  Serial.begin(115200);

  // Wait until the serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }

  pinMode(SHT_LOXL, OUTPUT);
  pinMode(SHT_LOXR, OUTPUT);
  pinMode(SHT_LOXFa, OUTPUT);
  // pinMode(SHT_LOXFb, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOXL, LOW);
  digitalWrite(SHT_LOXR, LOW);
  digitalWrite(SHT_LOXFa, LOW);
  // digitalWrite(SHT_LOXFb, LOW);

  Serial.println(F("All in reset mode...(pins are low)"));

  Serial.println(F("Starting..."));
  setID();
  Serial.println("yo");
}

void loop() {
  read_quad_sensors();
  delay(100);
}