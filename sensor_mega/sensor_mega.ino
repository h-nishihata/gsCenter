#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensorA;

const int DIR = 2;

String rxLine = "";
int distB = -1;

void setup()
{
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);   // MAX485受信

  Serial.begin(9600);
  Serial1.begin(9600);

  Wire.begin();

  if (!sensorA.init()) {
    Serial.println("Sensor A FAIL");
    while (1);
  }

  sensorA.setDistanceMode(VL53L1X::Long);
  sensorA.setMeasurementTimingBudget(50000);
  sensorA.startContinuous(50);

  Serial.println("Sensor A OK");
}

void loop()
{
  // ===== Bセンサー受信 =====
  while (Serial1.available())
  {
    char c = Serial1.read();

    if (c == '\n')
    {
        Serial.print("RX=[");
        Serial.print(rxLine);
        Serial.println("]");

        if (rxLine.startsWith("B,"))
        {
            distB = rxLine.substring(2).toInt();

            Serial.print("distB=");
            Serial.println(distB);
        }

        rxLine = "";
    }
    else if (c != '\r')
    {
        rxLine += c;
    }
  }

  // ===== Aセンサー読取 =====
  int distA = sensorA.read();

  // ===== 表示 =====
  static unsigned long lastPrint = 0;

  if (millis() - lastPrint > 500)
  {
    lastPrint = millis();

    Serial.print("A=");
    Serial.print(distA);

    Serial.print("  B=");
    Serial.println(distB);
  }
}