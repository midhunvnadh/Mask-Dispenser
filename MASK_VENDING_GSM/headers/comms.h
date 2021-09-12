void ShowSerialData(SoftwareSerial &SIM900)
{
  while(SIM900.available()!=0){
    Serial.print(char (SIM900.read()));
  }
}

void initComms(SoftwareSerial &SIM900){
  
  Serial.println("Check Communication...");
  SIM900.println("AT");
  delay(2000);
  ShowSerialData(SIM900);

  Serial.println("Configure bearer profile 1...");    
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  ShowSerialData(SIM900);
    
  Serial.println("APN of the provider...");  
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
  delay(2000);
  ShowSerialData(SIM900);
}


void sendToServer(String number, SoftwareSerial &SIM900){
  String num = String(number);
  Serial.println("Trying to send " + num + " to server");
  Serial.println("Open GPRS context");
  SIM900.println("AT+SAPBR=1,1");
  delay(2000);
  ShowSerialData(SIM900);

  Serial.println("Query the GPRS context...");
  SIM900.println("AT+SAPBR=2,1");
  delay(2000);
  ShowSerialData(SIM900);
  
  Serial.println("Init HTTP...");
  SIM900.println("AT+HTTPINIT");
  delay(2000);
  ShowSerialData(SIM900);
  
  
  Serial.println("Set parameters for HTTP session...");
  SIM900.println("AT+HTTPPARA=\"CID\",1");
  delay(2000);
  ShowSerialData(SIM900);

  Serial.println("Sending Request...");
  SIM900.println("AT+HTTPPARA=\"URL\",\"direct.techify.club/api/sendData?number=" + num + "\"");
  delay(2000);
  ShowSerialData(SIM900);
  
  Serial.println("Start GET session...");
  SIM900.println("AT+HTTPACTION=0");
  delay(2000);
  ShowSerialData(SIM900);

  Serial.println("Read data from HTTPS server...");
  SIM900.println("AT+HTTPREAD");
  ShowSerialData(SIM900);

  Serial.println("Terminate HTTP service...");
  SIM900.println("AT+HTTPTERM");
  ShowSerialData(SIM900);

  Serial.println("Close GPRS context...");
  SIM900.println("AT+SAPBR=0,1");
  ShowSerialData(SIM900);
}
