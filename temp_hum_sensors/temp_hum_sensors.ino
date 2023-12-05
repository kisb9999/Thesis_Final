#include "Wire.h"
#include "Arduino.h"
#include "SparkFunHTU21D.h"

HTU21D htu21d;
float temperature;
float humidity;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialUSB.begin(115200);
  Wire.begin();
  htu21d.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  htu21d.begin();
  humidity = htu21d.readHumidity();
  temperature = htu21d.readTemperature();

  SerialUSB.print("Temperature: ");
  SerialUSB.print(temperature);
  SerialUSB.println(" [°C] ");

  SerialUSB.print("Humidity: ");
  SerialUSB.print(humidity);
  SerialUSB.println(" [%] ");
  SerialUSB.println();

  delay(1000);
}
