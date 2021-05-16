#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <HCSR04.h>

/* Start Settings */
#define BUZZER_PIN             5        // Pin for Buzzer
#define US_PIN (int[])         {8,9}    // Pin for Ultrasonic Sensor
#define US_PIN2 (int[])        {10,11}  // Pin for Ultrasonic Sensor
#define UV_MOTOR_PIN           3        // Pin for UV Chamber Motor
#define UV_CHAMBER_LED         4        // Pin for UV Chamber Motor
#define UV_CHAMBER_DURATION    15000    // Duration to keep chamber holding
#define UV_CHAMBER_T_DURATION  30000    // Duration for treatment
#define BUZZER_BREAK_TMP       37       // Start High Temperature functions in this Temperature (C)
#define ULS_BREAK_DIST         10       // Start UltraSonic functions in this distance (cms)
#define BUZZ_DURATION          3000     // Duration in seconds * 1000
#define LCD_DIMS (int[])       {16,2}   // Cols, Rows
/* End Settings */


Adafruit_MLX90614 t_sen = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(63, LCD_DIMS[0], LCD_DIMS[1]);
HCSR04 hc(US_PIN[0], US_PIN[1]), hc1(US_PIN2[0], US_PIN2[1]);
Servo UV_Chamber_Motor;

double tmp_oc, tmp_of, uls_d_snz, uls_d_uv;

void setup(){
  // Init LCD
  initLCD(lcd);
  
  // Init Temp Sensor
  t_sen.begin();

  // Init Serial
  Serial.begin(115200);
  
  // Init Pins
  pinMode(BUZZER_PIN,OUTPUT);
 
  // Init Servo
  UV_Chamber_Motor.attach(UV_MOTOR_PIN);
  
  // Init Pin
  pinMode(UV_CHAMBER_LED, OUTPUT);
}

void loop(){
  tmp_oc = t_sen.readObjectTempC();
  tmp_of = t_sen.readObjectTempF();
  uls_d_snz = hc.dist();
  uls_d_uv = hc1.dist();
      
  serialPrint(tmp_oc, uls_d_snz, uls_d_uv);
  
  showIdle(lcd);
  startBuzzIfNeeded(tmp_oc, uls_d_snz, lcd);
  openUVChamberIfNeeded(uls_d_uv, UV_Chamber_Motor);
  

  if(tmp_oc > BUZZER_BREAK_TMP || uls_d_snz < ULS_BREAK_DIST){
    printLCDBeep(lcd, (tmp_oc > BUZZER_BREAK_TMP ? "  HIGH TEMP !  " : "   DISPENSING    "), BUZZ_DURATION);
    stopBuzz();
    displayTemp(tmp_oc, tmp_of, lcd);
    printLCDBeep(lcd, "Thank You !", 2000);
  }
}

void initLCD(LiquidCrystal_I2C lcde){
  lcde.backlight();
  lcde.begin();
  lcdStartAnimation(lcde);
}

void openUVChamberIfNeeded(double dist, Servo servo){
  if(dist < ULS_BREAK_DIST){
    toggleUVChamber(servo, 1, lcd);
    printLCDBeep(lcd, "Thank You !", 3000);
    loop();
  }
  else{
  
  }
}

void showIdle(LiquidCrystal_I2C lcde){
  lcde.setCursor(6, 0);
  lcd.print("SHOW");
  lcde.setCursor(3, 1);
  lcd.print("YOUR  HAND");
  
  showIdleAnimation(lcde);
}

void lcdStartAnimation(LiquidCrystal_I2C lcde){
  char Show[] = {"|/-\0"};
  lcde.setCursor(4, 0);
  lcde.print("STARTING");
  for(int i = 0 ; i < LCD_DIMS[0]; i++){
      lcde.setCursor(i, 1);
      lcde.print("*");
      delay(70);
  }
  lcd.clear();
}

void startBuzzIfNeeded(double tmp, double dist, LiquidCrystal_I2C lcde){
  if(tmp > BUZZER_BREAK_TMP || dist < ULS_BREAK_DIST){
    tone(BUZZER_PIN, 1000);
    if(tmp > BUZZER_BREAK_TMP){
      tone(BUZZER_PIN, 600);
    }
  }
  else{
    noTone(BUZZER_PIN);
  }
}

void printLCDBeep(LiquidCrystal_I2C lcde, String reason,double beepDelay){
  lcde.clear();
  lcde.setCursor(0, 0);
  lcde.print(reason);
  lcde.setCursor(0, 1);
  for(int i = 0; i < LCD_DIMS[0]; i++){
    delay(beepDelay/LCD_DIMS[0]);
    lcde.print("-");
  }
  lcde.clear();
}

void serialPrint(double tmp, double dist, double dist2){
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" cm \t Distance2: ");
  Serial.print(dist2);
  Serial.print(" cm \t Temp: ");
  Serial.print(tmp);
  Serial.println(" °C");
}

void displayTemp(double tmp_c, double tmp_f, LiquidCrystal_I2C lcde){
  lcde.setCursor(0, 0);
  lcde.print("YOUR TEMPERATURE");
  lcde.setCursor(0, 1);
  
  lcde.print(tmp_c);
  lcde.print((char)0xDF);
  lcde.print("C  ");
  
  lcde.print(tmp_f);
  lcde.print((char)0xDF);
  lcde.print("F");
  
  stopBuzz();
  delay(5000);
  lcde.clear();
}
void stopBuzz(){
  noTone(BUZZER_PIN);
}
void showIdleAnimation(LiquidCrystal_I2C lcde){
  lcde.setCursor(0, 0);
  lcd.print("|");
  lcde.setCursor(0, 1);
  lcd.print("|");
  lcde.setCursor(LCD_DIMS[0]-1, 0);
  lcd.print("|");
  lcde.setCursor(LCD_DIMS[0]-1, 1);
  lcd.print("|");
}

void toggleUVChamber(Servo servo, int state, LiquidCrystal_I2C lcde){
  for (int pos = 0; pos <= 90; pos += 1) {
    // in steps of 1 degree
    servo.write(pos);
    delay(30);
    if(pos == 90){
      printLCDBeep(lcde, "DROP YOUR OBJECT", UV_CHAMBER_DURATION);
    }
  }
  servo.write(0);
  delay(1000);
  digitalWrite(UV_CHAMBER_LED, HIGH);
  printLCDBeep(lcde, "UV TREATING...", UV_CHAMBER_T_DURATION);
  digitalWrite(UV_CHAMBER_LED, LOW);
  for (int pos = 90; pos >= 0; pos -= 1) {
    // in steps of 1 degree
    servo.write(pos);
    delay(30);
    if(pos == 90){
      printLCDBeep(lcde, "OBJECT TREATED !", UV_CHAMBER_DURATION);
    }
  }
}
