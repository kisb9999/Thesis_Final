#include "Arduino.h"
#include "MA510.h"
#include "Adafruit_NeoPixel.h"
#include "wiring_private.h"

  const String DEFAULT_IMEI = "100000000000000";
  const String DEFAULT_IP = "1.0.0.0";
  const String DEFAULT_CCID = "10000000000000000000"; 
  const float DEFAULT_VCC = 0.1;
  const String DEFAULT_TEMP = "-51";

  const String firmware_version = "Bence IoT NeoCortec 2023";
  
  Adafruit_NeoPixel pixels(1, 35, NEO_RGB + NEO_KHZ800);

  MA510 modem;
  String ip;
  String imei;
  String sim;
  float vcc;
  String temp;
  float temperature;
  float humidity;

  Uart ncSerial(&sercom2, 10, 9, SERCOM_RX_PAD_1, UART_TX_PAD_0);

  //NeoCortec variables
  char neo_message[100];
  byte node_id = 0;
  int loop_counter = 0;

  float node11_temp = 0;
  float node11_hum = 0;
  float node11_temp_avg = 0;
  float node11_hum_avg = 0;
  int node11_ctr = 0;

  float node16_temp = 0;
  float node16_hum = 0;
  float node16_temp_avg = 0;
  float node16_hum_avg = 0;
  int node16_ctr = 0;

  float node15_adc1 = 0;
  float node15_adc2 = 0;
  float node15_adc1_avg = 0;
  float node15_adc2_avg = 0;
  int node15_ctr = 0;

  float node19_adc1 = 0;
  float node19_adc2 = 0;
  float node19_adc1_avg = 0;
  float node19_adc2_avg = 0;
  int node19_ctr = 0;

void SERCOM2_Handler(){
  ncSerial.IrqHandler();
}
  
void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  
  for(int i = 10; i>0; i--){
    delay(1000);
    SerialUSB.print("Countdown: ");
    SerialUSB.println(i);
  }

  SerialUSB.println("The program has started!");


  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);
  delay(500);
  ncSerial.begin(115200);
  pinPeripheral(10, PIO_SERCOM_ALT);
  pinPeripheral(9, PIO_SERCOM_ALT);
  ncSerial.end();
  ncSerial.begin(115200);

  SerialUSB.println("Init modem...");
  Serial.begin(115200);
  
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

}

void loop() {
  // put your main code here, to run repeatedly:

  float temperature = 0;
  float humidity = 0;
  float magnet_ctr = 0;
  int adc1 = 0;
  int adc2 = 0;
  int adc3 = 0;

  long adc1_l = 0;
  long adc2_l = 0;
  long adc3_l = 0;
  memset(neo_message, 0, sizeof(neo_message));
  int counter = 0;
  
  if(ncSerial.available() > 0){
    SerialUSB.print("Data received from the NeoCortec module: ");
    delay(200);
    while(ncSerial.available() > 0){
      byte c = ncSerial.read();
      if(counter < 100){
        neo_message[counter++] = c;                     //could be an error
        if(c < 0x10) SerialUSB.print('0');
        SerialUSB.print(String(c, HEX));  
      }
    }
    SerialUSB.print(" [");
    SerialUSB.print(counter);
    SerialUSB.println(" bytes]");

    for(int i = 0; i < counter; i++){
      if(neo_message[i] == 0x52){

        pixels.begin();
        pixels.clear();
        pixels.setBrightness(100);
        pixels.setPixelColor(0, 255, 0, 0);
        pixels.show();
        
        node_id = neo_message[i + 3];


        //I2C sensor module
        if(neo_message[i + 1] == 0x0a){
          temperature = (float)((neo_message[i + 8] << 8 | neo_message[i + 9]) & 0xFFFC);
          temperature = 175.72 * (temperature / 65536) - 46.85;

          humidity = (float)((neo_message[i + 10] << 8 | neo_message[i + 11]) & 0xFFFC);
          humidity = 125 * (humidity / 65536) - 6;
          if(humidity < 0) humidity = 0;
          if(humidity > 100) humidity = 100;

          SerialUSB.print("NeoCortec I2C sensor data: ID=");
          SerialUSB.print(node_id, HEX);
          SerialUSB.print("---");
          SerialUSB.print("Temperature: ");
          SerialUSB.print(temperature);
          SerialUSB.print(" [°C] ");
          SerialUSB.print("Humidity: ");
          SerialUSB.print(humidity);
          SerialUSB.println(" [%] ");
          SerialUSB.println();

          if(node_id == 0x11){
            node11_ctr++;
            node11_temp += temperature;
            node11_hum += humidity;
            node11_temp_avg = node11_temp / node11_ctr;
            node11_hum_avg = node11_hum / node11_ctr;
          }
          else if(node_id == 0x16){
            node16_ctr++;
            node16_temp += temperature;
            node16_hum += humidity;
            node16_temp_avg = node16_temp / node16_ctr;
            node16_hum_avg = node16_hum / node16_ctr;
          }
        }


        //GPIO sensor module
        if(neo_message[i + 1] == 0x12){
          magnet_ctr = neo_message[i + 10] * 256 + neo_message[i + 11];
          adc1 = (float)(neo_message[i + 12] << 4 | neo_message[i + 13] >> 4);
          adc2 = (float)(neo_message[i + 14] << 4 | neo_message[i + 15] >> 4);
          adc3 = (float)(neo_message[i + 16] << 4 | neo_message[i + 17] >> 4);
          adc1_l = (float)adc1;
          adc2_l = (float)adc2;
          adc3_l = (float)adc3;
          if(adc1 > 2047) adc1_l = -1 * (float)((~adc1)+1);
          if(adc2 > 2047) adc2_l = -1 * (float)((~adc2)+1);
          if(adc3 > 2047) adc3_l = -1 * (float)((~adc3)+1);
          
          SerialUSB.print("NeoCortec GPIO sensor data: ID=");
          SerialUSB.print(node_id, HEX);
          SerialUSB.print("---");
          SerialUSB.print("Magnet counter: ");
          SerialUSB.print(magnet_ctr);
          SerialUSB.print(" ADC1 data: ");
          SerialUSB.print(adc1);
          SerialUSB.print(" ADC2 data: ");
          SerialUSB.print(adc2);
          SerialUSB.print(" ADC3 data: ");
          SerialUSB.print(adc3);
          SerialUSB.println();
          SerialUSB.println();

          if(node_id == 0x15){
            node15_ctr++;
            node15_adc1 += adc1;
            node15_adc2 += adc2;
            node15_adc1_avg = node15_adc1 / node15_ctr;
            node15_adc2_avg = node15_adc1 / node15_ctr;
          }
          else if(node_id == 0x19){
            node19_ctr++;
            node19_adc1 += adc1;
            node19_adc2 += adc2;
            node19_adc1_avg = node19_adc1 / node19_ctr;
            node19_adc2_avg = node19_adc1 / node19_ctr;
          }
        }
        pixels.begin();
        pixels.clear();
        pixels.setBrightness(50);
        pixels.setPixelColor(0, 50, 50, 50);
        pixels.show();        
      }
    }
    ncSerial.end();
    ncSerial.begin(115200); //clear the buffer
  }

  //Sending the data to the cloud for every 100 iterations
  loop_counter++;
  if(loop_counter == 100){

    pixels.begin();     
    pixels.clear();
    pixels.setBrightness(50);
    pixels.setPixelColor(0, 0, 255, 0);
    pixels.show();
    
    float data_array[] = {vcc, node11_temp_avg, node11_hum_avg, node16_temp_avg, node16_hum_avg, node15_adc1_avg, node15_adc2_avg, node19_adc1_avg, node19_adc2_avg};
    modem.sendData(imei, sim, ip, firmware_version, temp, data_array);

    node11_ctr = 0;
    node11_temp = 0;
    node11_hum = 0;
    node16_ctr = 0;
    node16_temp = 0;
    node16_hum = 0;

    node15_ctr = 0;
    node15_adc1 = 0;
    node15_adc2 = 0;
    node19_ctr = 0;
    node19_adc1 = 0;
    node19_adc2 = 0;

    pixels.begin();
    pixels.clear();
    pixels.setBrightness(50);
    pixels.setPixelColor(0, 50, 50, 50);
    pixels.show();
  } 
  delay(1000);
}
