#include "LiquidCrystal-I2C/LiquidCrystal_I2C.cpp"

#define LCD_DIMS (int[])       {16,2}
LiquidCrystal_I2C lcd(0x27, LCD_DIMS[0], LCD_DIMS[1]);


void lcdStartAnimation(){
  char Show[] = {"|/-\0"};
  lcd.setCursor(4, 0);
  lcd.print("STARTING");
  for(int i = 0 ; i < LCD_DIMS[0]; i++){
      lcd.setCursor(i, 1);
      lcd.print("*");
      delay(70);
  }
}

void initLCD(){
  lcd.backlight();
  lcd.begin();
  lcdStartAnimation();
}

void NetworkConnecting(){
  lcd.setCursor(0, 0);
  lcd.print(" Connecting.... ");
}

void Enterphonenumber(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Phone.....");
}

void lcdDisplay(String phone, String status_message){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   " + phone + "   ");
  lcd.setCursor(0, 1);
  lcd.print(status_message);
}
