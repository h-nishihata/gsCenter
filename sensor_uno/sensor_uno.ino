#include <Wire.h>
#include <VL53L1X.h>
#include <SoftwareSerial.h>

VL53L1X sensor;

const int RS485_DIR = 2;
SoftwareSerial rs485(11, 10);

void setup()
{
  Serial.begin(9600);
  pinMode(RS485_DIR, OUTPUT);

  Wire.begin();

  sensor.init();
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);

  rs485.begin(9600);
}

void loop()
{
  uint16_t d = sensor.read();

  digitalWrite(RS485_DIR, HIGH);

  rs485.print("B,");
  rs485.println(d);
  Serial.println(d);

  delay(50);

  digitalWrite(RS485_DIR, LOW);

  delay(100);
}