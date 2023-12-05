#include "Wire.h"
#include "Arduino.h"

int als = 0;
float mic_max;
float mic_min;
float mic_raw;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  //---------------------------------------------
  // Enable analog sensors
  //---------------------------------------------

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  delay(500);

  //---------------------------------------------
  // ALS sensor code snippet:
  //---------------------------------------------

  mic_min = 0;
  mic_max = 0;
  pinMode(25, INPUT);
  for(int i = 0; i < 20; i++){
    als += analogRead(25);
    delay(100);
  }

  als = (int)(als / 20);
  delay(1000);
  
  //---------------------------------------------
  // Microphone code snippet:
  //---------------------------------------------

  pinMode(26, INPUT);
  for(int i = 0; i < 20; i++){
    mic_raw = analogRead(26);
    if(mic_max < mic_raw)
      mic_max = mic_raw;
    if(mic_min > mic_raw)
      mic_min = mic_raw;
    delay(100);
  }

  mic_raw = mic_max - mic_min;

  SerialUSB.print("ALS value: ");
  SerialUSB.print(als);
  SerialUSB.println(" [Lux]");
  

  SerialUSB.print("Sound: ");
  SerialUSB.print(mic_raw);
  SerialUSB.println(" [dB]");
  
  SerialUSB.println();

  delay(1000);

  //---------------------------------------------
  // Disable sensorts
  //---------------------------------------------

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  delay(500);
}
