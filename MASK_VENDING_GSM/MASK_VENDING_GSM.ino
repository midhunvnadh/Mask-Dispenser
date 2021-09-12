#include <SoftwareSerial.h>
#include "headers/comms.h"
#include "headers/dialpad.h"
#include "headers/lcd.h"

SoftwareSerial SIM900(2, 3);

void setup() {
  SIM900.begin(9600);
  Serial.begin(9600);
  initLCD();

  NetworkConnecting();
  initComms(SIM900);
  Enterphonenumber();
}

String number = "";
String status_message = "";

void loop() {
  char customKey = keypad.getKey();
  if (customKey){
    status_message= "";
    if(customKey == 'D'){
      int lastIndex = number.length();
      number.remove(lastIndex-1, 2);
    }
    else if(customKey == 'A'){
      if(number.length() == 10){
        lcdDisplay(number, " Sending data... ");
        sendToServer(number, SIM900);
        status_message = "Sent!";
      }
      else{
        status_message = "Invalid Phone!";
      }
    }
    else{
      if(number.length() < 10)
        number += customKey;
    }
    lcdDisplay(number, status_message);
    if(status_message == "Sent!"){
      number = "";
      status_message = "";
      delay(3000);
      Enterphonenumber();
    }
  }
}
