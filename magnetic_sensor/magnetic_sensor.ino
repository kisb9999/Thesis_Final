#include "Wire.h"
#include "Arduino.h"
#include "SparkFun_MMC5983MA_Arduino_Library.h"

SFE_MMC5983MA magneticSensor;
boolean MMC5983_available = true;
int temperature_celsius = 0;
float temperature_fahrenheit = 0;    
uint32_t currentX = 0;
uint32_t currentY = 0;
uint32_t currentZ = 0;
double normalizedX = 0;
double normalizedY = 0;
double normalizedZ = 0;
double directionAngle = 0;
char displaybuffer[200] = {0};
    
void setup()
{
    SerialUSB.begin(115200);
    Serial.begin(115200);
    Wire.begin();

    SerialUSB.println("Program started.");
    if (magneticSensor.begin())
    {
      SerialUSB.println("Successful connection");
      magneticSensor.softReset(); 
      temperature_celsius = magneticSensor.getTemperature();
      temperature_fahrenheit = (temperature_celsius * 9.0f / 5.0f) + 32.0f;  

      SerialUSB.print("Temperature in Celsius: ");
      SerialUSB.print(temperature_celsius);
      SerialUSB.println(" [°C] ");
      SerialUSB.print("Temperature in fahrenheit");
      SerialUSB.print(temperature_fahrenheit);
      SerialUSB.println(" [°F] ");
    } 
    else {
      MMC5983_available = false;
      SerialUSB.println("Unsuccessful connection");
    }
}

void loop()
{
  if (MMC5983_available){
    currentX = 0;
    currentY = 0;
    currentZ = 0;
    normalizedX = 0;
    normalizedY = 0;
    normalizedZ = 0;

    magneticSensor.getMeasurementXYZ(&currentX, &currentY, &currentZ);

    // The magnetic field values are 18-bit unsigned. The approximate zero (mid) point is 2^17 (131072).
    // Normalize each fields to +/- 1.0 
    
    normalizedX = ((double)currentX - 131072)/131072;
    normalizedY = ((double)currentY - 131072)/131072;
    normalizedZ = ((double)currentZ - 131072)/131072;
    
    // Magnetic north is oriented with the Y axis
    // Convert the X and Y fields into directionAngle using atan2 (Arc Tangent 2)
    directionAngle = atan2(normalizedX, 0 - normalizedY);

    // atan2 returns a value between +PI and -PI
    // Convert to degrees
    directionAngle = (directionAngle/PI)*180+180;

    // The sensor has +/- 8G sensitivity, so the normalized +/-1 values are converted back by 8* multipication to Gauss
    // Magnetic Field strenght Gauss X,Y,Z (-8..+8 Gauss) and Direction (°) values
    SerialUSB.print(normalizedX * 8,8);
    SerialUSB.print(" ");
    SerialUSB.print(normalizedY * 8,8);
    SerialUSB.print(" ");
    SerialUSB.print(normalizedZ * 8,8);
    SerialUSB.print(" ");
    SerialUSB.print(directionAngle, 5);
    SerialUSB.println();
   
    delay(1000);
  } 
  else 
    SerialUSB.print("Sensor cannot be detected, check hardware please.");
}
