#include "Wire.h"
#include "Arduino.h"
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX AccelerationSensor; 

// Structs for X,Y,Z data
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

      boolean ISM33_available = true;
      char displaybuffer[200] = {0};
   
      float standstillAccX   = 0;
      float standstillGyroX  = 0;
      float standstillAccY   = 0;
      float standstillGyroY  = 0;
      float standstillAccZ   = 0;
      float standstillGyroZ  = 0;
        
      float showXAccel = 0;
      float showXGyro  = 0;
      float showYAccel = 0;
      float showYGyro  = 0;
      float showZAccel = 0;
      float showZGyro  = 0;

void setup(){
  Wire.begin();
  SerialUSB.begin(115200);

  if(AccelerationSensor.begin()){
    AccelerationSensor.deviceReset();
    while( !AccelerationSensor.getDeviceReset() ){ 
      delay(1);
    } 
    SerialUSB.println("Device reset was successful. Applying settings...");
    delay(100);
  
    AccelerationSensor.setDeviceConfig();
    AccelerationSensor.setBlockDataUpdate();
  
    // Set the output data rate and precision of the accelerometer unit
    AccelerationSensor.setAccelDataRate(ISM_XL_ODR_104Hz);
    AccelerationSensor.setAccelFullScale(ISM_4g); 

    // Set the output data rate and precision of the gyroscope unit
    AccelerationSensor.setGyroDataRate(ISM_GY_ODR_104Hz);
    AccelerationSensor.setGyroFullScale(ISM_500dps); 

    // Turn on the accelerometer's filter and apply settings. 
    AccelerationSensor.setAccelFilterLP2();
    AccelerationSensor.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
    
    // Turn on the gyroscope's filter and apply settings. 
    AccelerationSensor.setGyroFilterLP1();
    AccelerationSensor.setGyroLP1Bandwidth(ISM_MEDIUM);

    // Countdown before setting StandStill values
    for (int i = 0; i < 10; i++) {
       delay(1000);
       SerialUSB.print("Recording initial value set, please leave sensor stand still!");
       SerialUSB.print(" ");
       SerialUSB.println(i);
    }
    
    AccelerationSensor.getAccel(&accelData);
    AccelerationSensor.getGyro(&gyroData);
    standstillAccX   = accelData.xData;
    standstillAccY   = accelData.yData;
    standstillAccZ   = accelData.zData;
    standstillGyroX  = gyroData.xData;
    standstillGyroY  = gyroData.yData;
    standstillGyroZ  = gyroData.zData;  
  } 
  else 
    ISM33_available = false;
}

void loop(){
  if (ISM33_available && AccelerationSensor.checkStatus()){
    AccelerationSensor.getAccel(&accelData);
    AccelerationSensor.getGyro(&gyroData);
    
    showXAccel = 0;
    showXGyro = 0;
    showYAccel = 0;
    showYGyro = 0;
    showZAccel = 0;
    showZGyro = 0;

    // Fine tuning direction sensitivity criteria with experimental values
    // Calculating current module position compared to the original values 
    // recorded in standstill position
    if (abs(accelData.xData-standstillAccX) > 20)  showXAccel = accelData.xData - standstillAccX;
    if (abs(accelData.yData-standstillAccY) > 20)  showYAccel = accelData.yData - standstillAccY;
    if (abs(accelData.zData-standstillAccZ) > 300) showZAccel = accelData.zData - standstillAccZ;

    if (abs(gyroData.xData-standstillGyroX) > 300) showXGyro  = gyroData.xData - standstillGyroX;
    if (abs(gyroData.yData-standstillGyroY) > 300) showYGyro  = gyroData.yData - standstillGyroY;
    if (abs(gyroData.zData-standstillGyroZ) > 300) showZGyro  = gyroData.zData - standstillGyroZ;

    //Dont display anything if hardware is stationary
    if ((showXAccel !=0) || (showYAccel !=0) || (showZAccel !=0) || (showXGyro !=0) || (showYGyro !=0) || (showZGyro !=0)) {
      sprintf (displaybuffer,"AccX=%8d AccY=%8d AccZ=%8d GyroX=%8d GyroY=%8d GyroZ=%8d",(int)showXAccel,(int)showYAccel,(int)showZAccel,(int)showXGyro ,(int)showYGyro ,(int)showZGyro);
      SerialUSB.println(displaybuffer);  
    }  
  } 
  else 
    SerialUSB.println("Acceleration sensor is not available, please check hardware !");
  delay(1000);
}
