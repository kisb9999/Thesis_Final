#include "Arduino.h"
#include "stddef.h"
#include "MA510.h"

bool MA510::turnOn(int netLightPIN, int modemPIN){
  pinMode(netLightPIN, OUTPUT);
  digitalWrite(netLightPIN, HIGH);
  delay(1000);
  
  digitalWrite(modemPIN, HIGH);
  delay(700);
  digitalWrite(modemPIN, LOW);
  delay(3000);

  return isOn();
}

void MA510::turnOff(){
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  delay(3000);
  digitalWrite(12, LOW);
}

bool MA510::isOn(){
  //SerialUSB.println("isOn was called.");
  String res = sendAT();
  //SerialUSB.print("isOn-ba kapott valasz a sendCommandtol: ");
  //SerialUSB.print("isOn-ba kapott valasz a sendATtol: ");
  //SerialUSB.println(res);
  if(MA510::checkResponse(res))
    return true;
  return false;
}

String MA510::sendCommand(String command, unsigned int timeout){
  //SerialUSB.println("sendCommand was called.");
  String response = "";
  Serial.println(command);
  unsigned int previous = millis();

  delay(3000);
  
  while(Serial.available() > 0 && (millis()-previous) < timeout){
    //SerialUSB.println("sendCommand loop was called.");
    response += (char)Serial.read();
  }
  /*
  SerialUSB.println("----------------------------");
  SerialUSB.println("SendCommand");
  SerialUSB.println(response);
  SerialUSB.println("----------------------------");*/
  return response;
}

String MA510::sendAT(){
  //SerialUSB.println("Send at was called");
  String s = "";
  Serial.println("AT");
  
  delay(1000);
  
  while(Serial.available() > 0){
    //SerialUSB.println("sendAT loop");
    if(Serial.available() > 0){
      s += (char)Serial.read();
    }
  }

  //This print is only for testing purposes
  /*
  SerialUSB.println("----------------------------");
  SerialUSB.println("SendAT");
  SerialUSB.println(s);
  SerialUSB.println("----------------------------");*/
  
  return s;
}

String MA510::getIP(char* domain, unsigned int timeout, int recursion_counter, String default_value){
  SerialUSB.println("GetIP was called");
  char response[150];
  char expected_answer[50];
  char command_buffer[50];
  unsigned int previous_time;
  int counter = 0;
  String command_response = "";
  char value[50];

  memset(response, '\0', 150);

  //To empty the input buffer
  emptyBuffer();
  command_response = sendCommand("AT+MIPCALL=1", 1000);

  sprintf(command_buffer, "AT+MIPDNS=\"%s\"", domain);
  sprintf(value, "+MIPDNS: \"%s\",", domain);

  Serial.println(command_buffer);
  delay(1000);

  previous_time = millis();
  while((millis() - previous_time) < timeout){
    if(Serial.available() > 0){
      response[counter] = Serial.read();
      counter++;
    }
  }

  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("THis is the response: ");
  SerialUSB.println(response);
  SerialUSB.println("----------------------------------------------------------------");
  emptyBuffer();

  command_response = sendCommand("AT+MIPCALL=0", 1000);
  delay(1000);
  SerialUSB.println("Modem disconnected.");
  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("Recursion Counter: ");
  SerialUSB.println(recursion_counter);
  SerialUSB.println("----------------------------------------------------------------");

  String final = getUsefulPart(expected_answer, response, value);
  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("Final ip: ");
  SerialUSB.println(final);
  SerialUSB.println("----------------------------------------------------------------");
  if(final == "Error!"){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getIP(domain, timeout, recursion_counter, default_value);
  }
  return final;
}

String MA510::getIMEI(int recursion_counter, String default_value){
  SerialUSB.println("GetIMEI was called");
  Serial.println("AT+CGSN");
  delay(500);

  String imei = reader();
  int pos = imei.indexOf("AT+CGSN");

  SerialUSB.print("imei recursion_counter: ");
  SerialUSB.println(recursion_counter);
  
  if(pos == -1){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getIMEI(recursion_counter, default_value);
  }
  String final = imei.substring(pos+10, pos+25); 
  return final;
}

String MA510::getCCID(int recursion_counter, String default_value){
  SerialUSB.println("GetCCID was called");
  Serial.println("AT+CCID");
  delay(500);

  String sim = reader();
  int pos = sim.indexOf("AT+CCID");

  SerialUSB.print("ccid recursion counter: ");
  SerialUSB.println(recursion_counter);
  
  if(pos == -1){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getCCID(recursion_counter, default_value);
  }
  String final = sim.substring(pos+17, pos+37);
  return final;
}

float MA510::getModemVoltage(unsigned int timeout, int recursion_counter, float default_value){
  SerialUSB.println("GetVCC was called");
  String vcc = "";
  char response[100];
  char expected_answer[100];
  unsigned int previous_time;
  int counter = 0;

  memset(expected_answer, '\0', 100);
  memset(response, '\0', 100);

  emptyBuffer();

  Serial.println("AT+CBC");

  previous_time = millis();
  while((millis() - previous_time) < timeout){
    if(Serial.available() > 0){
      response[counter] = Serial.read();
      counter++;
    }
  }

  SerialUSB.print("vcc recursion_counter: ");
  SerialUSB.println(recursion_counter);
  
  vcc = getUsefulPart(expected_answer, response, "+CBC: ");
  if(vcc == "Error!"){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getModemVoltage(timeout, recursion_counter, default_value);
  }
  vcc.replace(",", ".");
  float final = vcc.toFloat() * 10;
  return final;
}

String MA510::getModemInternalTemp(unsigned int timeout, int recursion_counter, String default_value){
  SerialUSB.println("GetTemp was called");
  String temp = "";
  char response[100];
  char expected_answer[100];
  unsigned int previous_time;
  int counter = 0;

  memset(expected_answer, '\0', 100);
  memset(response, '\0', 100);

  emptyBuffer();

  Serial.println("AT+MTSM=1");

  previous_time = millis();
  while((millis() - previous_time) < timeout){
    if(Serial.available() > 0){
      response[counter] = Serial.read();
      counter++;
    }
  }

  SerialUSB.print("Temp recursion counter: ");
  SerialUSB.println(recursion_counter);

  String final = getUsefulPart(expected_answer, response, "+MTSM: ");
  if(final == "Error!"){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getModemInternalTemp(timeout, recursion_counter, default_value);
  }
  return final;
}

//-----------------------------------
//THIS IS THE HELPER METHOD SECTION |
//-----------------------------------

void MA510::emptyBuffer(){
  //SerialUSB.println("empybuffer was called");
  while(Serial.available() > 0){
    Serial.read();
  }
  delay(500);
}

bool MA510::checkResponse(String s){
  /*
  SerialUSB.println("---------------------------");
  SerialUSB.print("Check Response Gets this string: ");
  SerialUSB.println(s);
  SerialUSB.println("---------------------------");*/
  if(s.indexOf("OK") == -1)
    return false;
  return true;
}

String MA510::reader(){
  String response = "";
  while(Serial.available() > 0){
    response += (char)Serial.read();
  }
  return response;
}

String MA510::getUsefulPart(char* expected_answer, char* response, const char* value){
  String final = "";
  sprintf(expected_answer, value);
  String expected_answer_string = (String)expected_answer;
  String answer_string = (String)response;
  int position_of_string = answer_string.indexOf(expected_answer_string);

  if(position_of_string == -1){
    return "Error!"; 
  }

  int char_counter = position_of_string + expected_answer_string.length();
  while(answer_string.charAt(char_counter) != '\r' && char_counter < strlen(response)){
    if(answer_string.charAt(char_counter) != '\r'){
      final += (char)answer_string.charAt(char_counter);
    }
    char_counter++;
  }
  return final;
}

void MA510::sendData(String imei, String ccid, String remote_ip, String firmware_version, String temp, float temperature, float humidity){
  char text_buffer[300] = {0};
  String hex_char;
  String response = "";

  Serial.flush();
  SerialUSB.flush();

  SerialUSB.println("---------------------------------------------------------------------");
  SerialUSB.println("Sending is in progress.....");
  delay(3000);

  response = sendCommand("AT+MIPCALL=1", 2000);
  delay(2000);
  SerialUSB.print("sendData response: ");
  SerialUSB.println(response);
  
  SerialUSB.println("Opening a socket...");
  // response = sendCommand("AT+MIPOPEN=1,"+port+",\""+remote_ip+"\","+port+",1");
  response = sendCommand("AT+MIPOPEN=1,41234,\""+remote_ip+"\",41234,1", 2000);
  SerialUSB.println(response);
  delay(2000);

  SerialUSB.println("Sending telegram...");
  Serial.print("AT+MIPSEND=1,\"");
  SerialUSB.println("AT+MIPSEND=1,\"");

  SerialUSB.println();
  SerialUSB.print("Imei: ");
  SerialUSB.println(imei);
  SerialUSB.println(ccid);
  SerialUSB.println(firmware_version);
  SerialUSB.println(temp);
  SerialUSB.println(temperature);
  SerialUSB.println(humidity);
  
  char imei_char[100];
  char ccid_char[100];
  char firmware_char[100];
  char temp_char[100];
  
  strcpy(imei_char, imei.c_str());
  strcpy(ccid_char, ccid.c_str());
  strcpy(firmware_char, firmware_version.c_str());
  strcpy(temp_char, temp.c_str()); 
  
  sprintf(text_buffer, "{\"IMEI\": \"%s\", \"msgref\" : \"%s ICCID:%s IMEI:%s \", \"payload\" : \"|MT%s|T%06d|H%06d|\", \"gpsdata\" : \"10,20\"}", imei_char, firmware_char, ccid_char, imei_char, temp_char, (int)temperature, (int)humidity);
  //sprintf(text_buffer, "{\"IMEI\": \"%s\" , \"msgref\" : \"Bence_ref\", \"payload\" : \"|Bence_payload|\", \"gpsdata\" : \"49.4185,11.1180\"}", test);

  int c = 0;
  while (text_buffer[c] != '\0')
    c++;

  int len = c;
  for(int i=0; i<len; i++){
    String hex_char = String(text_buffer[i], HEX);
    do {} while(Serial.availableForWrite()<60);
    Serial.print(hex_char);
    SerialUSB.print(text_buffer[i]);
  }

  Serial.flush();
  SerialUSB.flush();
  
  Serial.println("\"");

  //TODO
  do {} while(Serial.availableForWrite()<60);
  Serial.flush();
  SerialUSB.flush();
  delay(2000);

  //Push data
  response = sendCommand("AT+MIPPUSH=1", 2000);
  SerialUSB.println("AT+MIPPUSH=1");
  SerialUSB.println(response);
  delay(3000);
  SerialUSB.println();

  //Close socket
  response = sendCommand("AT+MIPCLOSE=1", 2000);
  SerialUSB.println("AT+MIPCLOSE=1");
  SerialUSB.println(response);
  delay(500);
  SerialUSB.println();

  //Close the GSM Link
  response = sendCommand("AT+MIPCALL=0", 2000);
  SerialUSB.println("AT+MIPCALL=1");
  SerialUSB.println(response);
  delay(500);
  SerialUSB.println();
  SerialUSB.println("---------------------------------------------------------------------");
  SerialUSB.flush();
}
