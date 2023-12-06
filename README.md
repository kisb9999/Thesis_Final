# Thesis_Final

**- Android Demo Application:** This project is the front-end of the IoT system implementation. It uses the E-Cloud API to get the data measured by a given hardware (based on IMEI number). The temperature and light datas are displayed in graphs.  
**- Windows Demo Application:** This is a Windows Forms application for displaying the data read directly from the Serial Port of the gateway. It can also function as a gateway if used together with the USB NeoCortec module. In this case the application sends the data once every 30 seconds into the E-Cloud using UDP connection.  
**- Acceleration (and Gyroscope) sensor demo:** This code shows how to use the built in ISM330DHCX Accelerometer and Gyroscope.  
**- Analog sensor demo:** This code shows how to use the IoT hardwares analog sensors (ALS and Mic).  
**- Magnetic sensor demo:** This code shows how to use the MMC5983MA sensor for orientation measurement.  
**- NeoCortec gateway:** Implements the NeoMesh network. On the LAN side it collects the data from the NeoCortec module. On the WAN side it implements the Point-Point communication with the cloud server.  
**- Point-Point gateway:** This project implements the Point-Point based communication. It measures temperature and humidity and then sends the data into the E-Cloud server.  
**- Temperature and humidiy sensor demo:** Shows how to use the build in HTU21D sensor for temperature and humidity measurements.  
