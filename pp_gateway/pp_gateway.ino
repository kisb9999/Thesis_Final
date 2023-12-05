#include "Wire.h"
#include "Arduino.h"
#include "MA510.h"
#include "Adafruit_NeoPixel.h"
#include "wiring_private.h"
#include "SparkFunHTU21D.h"
#include "SparkFun_ISM330DHCX.h"
#include "SparkFun_MMC5983MA_Arduino_Library.h"

  //Default variables in case something goes wrong
  const String DEFAULT_IMEI = "100000000000000";
  const String DEFAULT_IP = "1.0.0.0";
  const String DEFAULT_CCID = "10000000000000000000"; 
  const float DEFAULT_VCC = 0.1;
  const String DEFAULT_TEMP = "-51";

  const String firmware_version = "E-IoTMini-SAMD21 2023-11-08";
  
  Adafruit_NeoPixel pixels(1, 13, NEO_RGB + NEO_KHZ800);
  HTU21D temp_hum_sensor;
  SparkFun_ISM330DHCX ism;
  SFE_MMC5983MA magometer;

  MA510 modem;
  String ip;
  String imei;
  String sim;
  float vcc;
  String temp;
  float temperature;
  float humidity;

  sfe_ism_data_t accelData;
  sfe_ism_data_t gyroData;
  float totalAccelX = 0;
  float totalAccelY = 0;
  float totalAccelZ = 0;
  float read_counter_accel = 0;
  float totalGyroX = 0;
  float totalGyroY = 0;
  float totalGyroZ = 0;
  float read_counter_gyro = 0;

  float accelX = 0;
  float accelY = 0;
  float accelZ = 0;
  float gyroX = 0;
  float gyroY = 0;
  float gyroZ = 0;

  uint32_t currentX = 0;
  uint32_t currentY = 0;
  uint32_t currentZ = 0;
  double normalizedX = 0;
  double normalizedY = 0;
  double normalizedZ = 0;

  //NoeCortec variables
  Uart uart (&sercom2, 10, 9, SERCOM_RX_PAD_1, UART_TX_PAD_0);
  Adafruit_NeoPixel neo_pixels(1, 35, NEO_RGB + NEO_KHZ800);

  char neo_msg[150] = {0};

  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialUSB.begin(115200);
  Wire.begin();

  for(int i = 10; i>0; i--){
    delay(1000);
    SerialUSB.print("Countdown: ");
    SerialUSB.println(i);
  }

  SerialUSB.println("The program has started!");

  if(modem.turnOn(19, 18))
    SerialUSB.println("Turning Modem on");
  else
    SerialUSB.println("Modem off");
  if(modem.isOn()){
    SerialUSB.println("The modem is on");
  }

  ip = modem.getIP("cloud.e-iot.info", 2000, 3, DEFAULT_IP);

  SerialUSB.println("--------------------");
  SerialUSB.print("The returned ip is: ");
  SerialUSB.println(ip);
  SerialUSB.println("--------------------");

  imei = modem.getIMEI(2, DEFAULT_IMEI);

  SerialUSB.println("--------------------");
  SerialUSB.print("The returned imei is: ");
  SerialUSB.println(imei);
  SerialUSB.println("--------------------");

  sim = modem.getCCID(2);
  
  SerialUSB.println("--------------------");
  SerialUSB.print("The returned ccid is: ");
  SerialUSB.println(sim);
  SerialUSB.println("--------------------");

  vcc = modem.getModemVoltage(2000, 2);
  //float final = vcc.toFloat()*10;
  SerialUSB.println("--------------------");
  SerialUSB.print("The returned vcc is: ");
  SerialUSB.println(vcc);
  SerialUSB.println("--------------------");

  temp = modem.getModemInternalTemp(2000, 2);

  SerialUSB.println("--------------------");
  SerialUSB.print("The returned temp is: ");
  SerialUSB.println(temp);
  SerialUSB.println("--------------------");
  
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(50);
  pixels.setPixelColor(0,50,50,50);
  pixels.show();

  temp_hum_sensor.begin();
  temperature = round(temp_hum_sensor.readTemperature() * 100);
  humidity = round(temp_hum_sensor.readHumidity() * 100);
  SerialUSB.println("--------------------");
  SerialUSB.print("Temp and hum: ");
  SerialUSB.println(temperature);
  SerialUSB.println(humidity);
  SerialUSB.println("--------------------");

  SerialUSB.println("Initializing magometer...");
  if(!magometer.begin())
    SerialUSB.println("Magometer not detected.");
  else{
    SerialUSB.println("Magometer detected.");
    magometer.softReset();
    int magCelsius = magometer.getTemperature();
    SerialUSB.print("Mag temp: ");
    SerialUSB.println(magCelsius);
  }

  SerialUSB.println("Initializing acceleration sensor...");
  if(!ism.begin())
    SerialUSB.println("Accelerometer not detected.");
  else{
    SerialUSB.println("Accelerometer detected.");
    ism.deviceReset();
    while(!ism.deviceReset()){
      delay(1);
    }
    ism.setDeviceConfig();
    ism.setBlockDataUpdate();

    ism.setAccelDataRate(ISM_XL_ODR_208Hz);
    ism.setAccelFullScale(ISM_4g);

    ism.setGyroDataRate(ISM_GY_ODR_104Hz);
    ism.setGyroFullScale(ISM_250dps);

    ism.setAccelFilterLP2();
    ism.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
    
    ism.setGyroFilterLP1();
    ism.setGyroLP1Bandwidth(ISM_MEDIUM);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(30);
  pixels.setPixelColor(0,255,255,255);
  pixels.show();

  delay(1000);

  pixels.begin();
  pixels.clear();
  pixels.setBrightness(30);
  pixels.setPixelColor(0,255,0,0);
  pixels.show();

  delay(1000);

   pixels.begin();
  pixels.clear();
  pixels.setBrightness(30);
  pixels.setPixelColor(0,0,255,0);
  pixels.show();

  delay(1000);
  
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(30);
  pixels.setPixelColor(0,0,0,255);
  pixels.show();

  delay(1000);

  int c = 10;
  while(c>0){
    if(ism.checkAccelStatus()){
      ism.getAccel(&accelData);
      totalAccelX += accelData.xData;
      totalAccelY += accelData.yData;
      totalAccelZ += accelData.zData; 
      read_counter_accel++;     
    }
    if(ism.checkGyroStatus()){
      ism.getGyro(&gyroData);
      totalGyroX += gyroData.xData;
      totalGyroY += gyroData.yData;
      totalGyroZ += gyroData.zData; 
      read_counter_gyro++;  
    }
    c--;
    delay(1000);
  }

  accelX = totalAccelX / read_counter_accel;
  accelY = totalAccelY / read_counter_accel;
  accelZ = totalAccelZ / read_counter_accel;

  gyroX = totalGyroX / read_counter_gyro;
  gyroY = totalGyroY / read_counter_gyro;
  gyroZ = totalGyroZ / read_counter_gyro;

  SerialUSB.println("***************************************************************************************************");
  SerialUSB.println(imei);
  SerialUSB.println(sim);
  SerialUSB.println(ip);
  SerialUSB.println(vcc);
  SerialUSB.println(temp);
  SerialUSB.println("Accel and gyro: ");
  SerialUSB.println(accelX);
  SerialUSB.println(accelY);
  SerialUSB.println(accelZ);
  SerialUSB.println(gyroX);
  SerialUSB.println(gyroY);
  SerialUSB.println(gyroZ);
  SerialUSB.println("***************************************************************************************************");

  
  
  modem.sendData(imei, sim, ip, firmware_version, temp, temperature, humidity);
  delay(10000);
}
