#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

const int A_UP   = 2;
const int A_DOWN = 3;
const int B_UP   = 8;
const int B_DOWN = 9;
const int PULSE_MS = 200;  // Pulse width

void buttonPressed(int pin)
{
  digitalWrite(pin, LOW);
  delay(PULSE_MS);
  digitalWrite(pin, HIGH);
}

void setup()
{
  pinMode(A_UP,   OUTPUT);
  pinMode(A_DOWN, OUTPUT);
  pinMode(B_UP,   OUTPUT);
  pinMode(B_DOWN, OUTPUT);
  digitalWrite(A_UP,   HIGH);
  digitalWrite(A_DOWN, HIGH);
  digitalWrite(B_UP,   HIGH);
  digitalWrite(B_DOWN, HIGH);
  
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Error: cannot recognize sensor :( )");
    while (1);
  }
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);
}

void loop()
{
  // Serial incoming
  if (Serial.available() > 0)
  {
    int value = Serial.read();
    if      (value == 1) buttonPressed(A_UP);
    else if (value == 2) buttonPressed(A_DOWN);
    else if (value == 3) buttonPressed(B_UP);
    else if (value == 4) buttonPressed(B_DOWN);
  }
  // Serial outgoing
  int dist = sensor.read();  // non-blocking
  if (dist > 0 && dist < 6000) {
    uint16_t dist = sensor.read();

    Serial.write(highByte(dist));
    Serial.write(lowByte(dist));
    // Serial.println(dist);
  }
}