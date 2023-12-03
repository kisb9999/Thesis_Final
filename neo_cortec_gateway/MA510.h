#ifndef MA510_H
#define MA510_H

#include "Arduino.h"

class MA510{
  public:
    bool turnOn(int netLightPIN, int modemPIN);
    void turnOff();
    bool isOn();
    String sendCommand(String command, unsigned int timeout);
    String sendAT();
    bool checkResponse(String response);

    void emptyBuffer();
    String reader();
    String getUsefulPart(char* expected_answer, char* response, const char* value);
    
    String getIP(char* domain, unsigned int timeout, int recursion_counter = 5, String default_value = "0.0.0.0");
    String getIMEI(int recursion_counter = 5, String default_value = "000000000000000");
    String getCCID(int recursion_counter = 5, String default_value = "00000000000000000000");
    float getModemVoltage(unsigned int timeout, int recursion_counter = 5, float default_value = 0.0);
    String getModemInternalTemp(unsigned int timeout, int recursion_counter = 5, String default_value = "-50");

    void sendData(String imei, String ccid, String remote_ip, String firmware_version, String temp, float data_array[]);
    //String createTelegram();
    //String MA510:convertToPayloadData(String c, int length);
    void sendHexString(String data);
};

#endif
