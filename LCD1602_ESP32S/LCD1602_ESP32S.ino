//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
// This program is used for Arduino V1.6.5 
#include <Wire.h>     //I2C 基本函式 (必要)
#include <LiquidCrystal_I2C.h>    //LCD 1602/2004 的函式 

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// 產生lcd 螢幕物件(I2C使用位址，幾行(縱向)，幾列(橫向))
void setup()
{
  lcd.init();                      // initialize the lcd 
    //cd 螢幕物件 初始化  lcd.init()
  // Print a message to the LCD.
  lcd.backlight();    //開啟背光
  lcd.clear() ;   //清除螢幕
  lcd.setCursor(0,0);   //設定游標位置(行數、列數)
  lcd.print("Hello, world!"); //在游標位置印出字 :"Hello, world!"
   lcd.setCursor(0,1);    //設定游標位置(行數、列數)
  lcd.print("Power By BruceTsao!");   //在游標位置印出字 "Power By BruceTsao!"

}


void loop()
{
    lcd.setCursor(0,0);
  lcd.print("Hello, world!");
   lcd.setCursor(0,1);
  lcd.print("Power By BruceTsao!");
   lcd.setCursor(0,2);
  lcd.print("2Power By BruceTsao!");
   lcd.setCursor(0,3);
  lcd.print("3Power By BruceTsao!");
  delay(1000);
}
