#include "Arduino.h"
#include "stddef.h"
#include "MA510.h"

//Turns on the MA510 modem.
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

//Checks if the modem is on by sending an AT command. In the checkResponse method it checks if the answer was OK.
bool MA510::isOn(){
  //SerialUSB.println("isOn was called.");
  String res = sendAT();
  return MA510::checkResponse(res);
}

//I use this method to send different AT commands to the modem.
String MA510::sendCommand(String command, unsigned int timeout){
  //SerialUSB.println("sendCommand was called.");
  String response = "";

  //This line send the command itself to the modem's UART serial port.
  Serial.println(command);
  unsigned int previous = millis();

  delay(3000);

  //This loop reads the resoponse to the AT command.
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

//This is the method to send the basic AT command. Could have used the sendCommand with AT as parameter.
String MA510::sendAT(){
  //SerialUSB.println("Send at was called");
  String s = "";
  Serial.println("AT");
  
  delay(1000);
  
  while(Serial.available() > 0){
    //SerialUSB.println("sendAT loop");
    s += (char)Serial.read();
  }
  
  return s;
}

//This method gets the IP address of the cloud server. I added recursion because sometimes the hardware cannot get the IP on the first try. 
//The users can add how many times they want the method to try again. The default is 5.
String MA510::getIP(char* domain, unsigned int timeout, int recursion_counter, String default_value){
  SerialUSB.println("GetIP was called");

  char response[150];
  char expected_answer[50];
  char command_buffer[50];
  unsigned int previous_time;
  int counter = 0;
  String command_response = "";
  char value[50];

  //Fill the response array.
  memset(response, '\0', 150);

  //To empty the input buffer
  emptyBuffer();
  //Send this AT command to open a wireless GSM connection.
  command_response = sendCommand("AT+MIPCALL=1", 1000);

  //Add the string command into the buffer.
  sprintf(command_buffer, "AT+MIPDNS=\"%s\"", domain);
  sprintf(value, "+MIPDNS: \"%s\",", domain);

  //Send the AT command to get the IP
  Serial.println(command_buffer);
  delay(1000);

  //Load the response message from the cloud
  previous_time = millis();
  while(Serial.available() > 0 && (millis() - previous_time) < timeout){    
    response[counter] = Serial.read();
    counter++;
  }

  //Just printing out the response to check the functionalities.
  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("This is the response: ");
  SerialUSB.println(response);
  SerialUSB.println("----------------------------------------------------------------");
  emptyBuffer();

  //Close the GSM link.
  command_response = sendCommand("AT+MIPCALL=0", 1000);
  delay(1000);
  SerialUSB.println("Modem disconnected.");

  //Just to follow the number of runs.
  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("Recursion Counter: ");
  SerialUSB.println(recursion_counter);
  SerialUSB.println("----------------------------------------------------------------");

  //This method returns the IP from the response message.
  String final = getUsefulPart(expected_answer, response, value);
  SerialUSB.println("----------------------------------------------------------------");
  SerialUSB.print("Final ip: ");
  SerialUSB.println(final);
  SerialUSB.println("----------------------------------------------------------------");
  
  //If there is an error, so the IP is not present call the method again. If the method ran more times then the recursion_counter set the default value.
  //Users can set a default IP address.
  if(final == "Error!"){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getIP(domain, timeout, recursion_counter, default_value);
  }

  //Return the IP address as a string.
  return final;
}

//This method returns the modem's IMEI number.
String MA510::getIMEI(int recursion_counter, String default_value){
  SerialUSB.println("GetIMEI was called");
  Serial.println("AT+CGSN");
  delay(500);

  //I store the response from the modem. Imei number is in the message but it has to be extracted.
  String imei = reader();
  int pos = imei.indexOf("AT+CGSN");

  SerialUSB.print("imei recursion_counter: ");
  SerialUSB.println(recursion_counter);

  //Check if the return message contains the IMEI or there was an error.
  if(pos == -1){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getIMEI(recursion_counter, default_value);
  }

  //Extract the IMEI number from the message.
  String final = imei.substring(pos+10, pos+25); 
  return final;
}

//Similar to the getIMEI. This method returns the modem's CCID (SIM).
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

//This method reads and returns the value of the voltage inside the MA510 modem.
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

  //This is the AT command to get the voltage.
  Serial.println("AT+CBC");

  previous_time = millis();
  while(Serial.available() > 0 && (millis() - previous_time) < timeout){
    response[counter] = Serial.read();
    counter++;
  }

  SerialUSB.print("vcc recursion_counter: ");
  SerialUSB.println(recursion_counter);

  //Extract the voltage data from the returned message.
  vcc = getUsefulPart(expected_answer, response, "+CBC: ");
  if(vcc == "Error!"){
    if(recursion_counter == 0){
      return default_value;
    }
    recursion_counter--;
    return getModemVoltage(timeout, recursion_counter, default_value);
  }
  //Convert value to float.
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
  while(Serial.available() > 0 &&(millis() - previous_time) < timeout){
    response[counter] = Serial.read();
    counter++;
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

//Checks if the AT command returns an OK.
bool MA510::checkResponse(String s){
  if(s.indexOf("OK") == -1)
    return false;
  return true;
}

//Reads the response from AT commands.
String MA510::reader(){
  String response = "";
  while(Serial.available() > 0){
    response += (char)Serial.read();
  }
  return response;
}

//This method extracts the IP or the Voltage from the message.
String MA510::getUsefulPart(char* expected_answer, char* response, const char* value){
  String final = "";

  //Load into string buffer
  sprintf(expected_answer, value);
  String expected_answer_string = (String)expected_answer;
  String answer_string = (String)response;
  int position_of_string = answer_string.indexOf(expected_answer_string);

  if(position_of_string == -1){
    return "Error!"; 
  }

  //Extracts the IP or the Voltage based on the position in the response message.
  int char_counter = position_of_string + expected_answer_string.length();
  while(answer_string.charAt(char_counter) != '\r' && char_counter < strlen(response)){
    final += (char)answer_string.charAt(char_counter);
    char_counter++;
  }
  return final;
}

void MA510::sendData(String imei, String ccid, String remote_ip, String firmware_version, String temp, float data_array[]){
  char text_buffer[300] = {0};
  char display_buffer[20] = {0};
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
  //SerialUSB.println(temperature);
  //SerialUSB.println(humidity);
  
  char imei_char[100];
  char ccid_char[100];
  char firmware_char[100];
  char temp_char[100];
  
  strcpy(imei_char, imei.c_str());
  strcpy(ccid_char, ccid.c_str());
  strcpy(firmware_char, firmware_version.c_str());
  strcpy(temp_char, temp.c_str()); 
  
  sprintf(text_buffer, "{\"IMEI\": \"%s\", \"msgref\" : \"%s ICCID:%s IMEI:%s \", \"payload\" : \"", imei_char, firmware_char, ccid_char, imei_char);
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

  //Compose the telegram

  sprintf(display_buffer, "U%06d", (int)(data_array[0]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "M%06d", (int)(temp.toFloat()*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "A%06d", (int)(data_array[1]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "B%06d", (int)(data_array[2]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "C%06d", (int)(data_array[3]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "D%06d", (int)(data_array[4]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "E%06d", (int)(data_array[5]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "F%06d", (int)(data_array[6]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "G%06d", (int)(data_array[7]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sprintf(display_buffer, "H%06d", (int)(data_array[8]*100));
  sendHexString(String(display_buffer) + "|");
  Serial.flush();

  sendHexString("\", \"gpsdata:\" : \"");
  String gps = "TESTING";
  sendHexString(gps + "\"}");
  
  Serial.println("\"");

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

void MA510::sendHexString(String data){
  String hexChar;
  for(int i = 0; i < data.length(); i++){
    hexChar = String(data[i], HEX);
    do {} while(Serial.available() < 60);
    Serial.print(hexChar);
    SerialUSB.print(data[i]);
  }
}
