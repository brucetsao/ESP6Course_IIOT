#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void ClearShow()    //清除LCD 螢幕
{
    lcd.setCursor(0,0);
    lcd.clear() ;
    lcd.setCursor(0,0);
}
 void initLCD()   //初始化LCD螢幕
 {
    debugoutln("Init LCD Screen") ;
   lcd.init();  //初始化LCD螢幕
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
 }
void ShowLCD1(String cc)
{
  lcd.setCursor(0,0);
  lcd.print("                    ");  
  lcd.setCursor(0,0);
  lcd.print(cc);  
  
}
void ShowLCD1L(String cc)
{
//  lcd.setCursor(0,0);
//  lcd.print("                    ");  
  lcd.setCursor(0,0);
  lcd.print(cc);  
  
}
void ShowLCD1M(String cc)
{
 // lcd.setCursor(0,0);
//  lcd.print("                    ");  
  lcd.setCursor(14,0);
  lcd.print(cc);  
  
}
void ShowLCD2(String cc)
{
  lcd.setCursor(0,1);
  lcd.print("                    ");  
  lcd.setCursor(0,1);
  lcd.print(cc);  
  
}
void ShowLCD3(String cc)
{
  lcd.setCursor(0,2);
  lcd.print("                    ");  
  lcd.setCursor(0,2);
  lcd.print(cc);  
  
}
void ShowLCD4(String cc)
{
  lcd.setCursor(0,3);
  lcd.print("                    ");  
  lcd.setCursor(0,3);
  lcd.print(cc);  
  
}


void ShowString(String ss)
{
  lcd.setCursor(0,3);
  lcd.print("                    ");  
  lcd.setCursor(0,3);
  lcd.print(ss.substring(0,19)); 
  //delay(1000);
}

void ShowAPonLCD(String ss)  //顯示熱點
{
  ShowLCD1M(ss) ; //顯示熱點

}
void ShowMAConLCD(String ss)  //顯示網路卡編號
{
  ShowLCD1L(ss) ; //顯示網路卡編號

}
void ShowIPonLCD(String ss)  //顯示連接熱點後取得網路編號(IP Address)}
{
    ShowLCD2("IP:"+ss) ;

}
void ShowInternetonLCD(String s1,String s2,String s3)   //顯示網際網路連接基本資訊
{

    ShowMAConLCD(s1)  ;  //顯示熱點
    ShowAPonLCD(s2) ;    //顯示連接熱點名稱
    ShowIPonLCD(s3) ;  //顯示連接熱點後取得網路編號(IP Address)}

}

void ShowSensoronLCD(double s1, double s2)    //顯示溫度與濕度在LCD上
{
//    ShowLCD3("T:"+Double2Str(TempValue,1)+".C "+"T:"+Double2Str(HumidValue,1)+".%" ) ;
    ShowLCD3("T:"+Double2Str(s1,1)+".C "+"H:"+Double2Str(s2,1)+".%" ) ;
}
