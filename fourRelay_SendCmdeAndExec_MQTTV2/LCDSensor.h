  // 引用Wire.h和LiquidCrystal_I2C.h程式庫
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  //這段程式碼主要是使用 LiquidCrystal_I2C 函式庫來控制 I2C 介面的 LCD 顯示屏，程式碼內容如下
  // 使用LiquidCrystal_I2C程式庫初始化LCD螢幕
  LiquidCrystal_I2C lcd(0x27, 20, 4); // 設置LCD位址為0x27，顯示16個字符和2行
  //這個程式需要引入 Wire.h 和 LiquidCrystal_I2C.h 兩個庫。然後，定義一個名為 lcd 的 LiquidCrystal_I2C 類型物件，並且指定 LCD 顯示屏的 I2C 位址、行數和列數
  
  //LiquidCrystal_I2C lcd(0x3F,20,4); // 設置LCD位址為0x3F，顯示16個字符和2行（注释掉的代码）
  
  // 清除LCD螢幕
  void ClearShow()
  {
    // ClearShow() 函式會把 LCD 的游標移動到左上角，然後使用 lcd.clear() 函式清除螢幕內容，最後再把游標移動到左上角。
  
    lcd.setCursor(0, 0); //設置LCD螢幕座標
    lcd.clear() ; //清除LCD螢幕
    lcd.setCursor(0, 0); //設置LCD螢幕座標
  }
  // 初始化LCD螢幕
  void initLCD()
  {
    debugoutln("Init LCD Screen") ; //调试信息输出
    lcd.init(); //初始化LCD螢幕
    lcd.backlight(); //打开LCD背光
    lcd.setCursor(0, 0); //設置LCD螢幕座標
  }
  /*
     接下來是一些顯示字串的函式，例如 ShowLCD1()、ShowLCD2()、ShowLCD3()、ShowLCD4() 和 ShowString()。這些函式都是用來在 LCD 顯示屏上顯示不同的字串。以 ShowLCD1() 函式為例，程式碼內容如下
  
  */
  // 在第一行顯示字串cc
  void ShowLCD1(String cc)
  {
    lcd.setCursor(0, 0); //設置LCD螢幕座標
    lcd.print("                    "); // 清除第一行的内容
    lcd.setCursor(0, 0); //設置LCD螢幕座標
    lcd.print(cc); //在第一行顯示字串cc
  }
  
  // 在第一行左邊顯示字串cc
  void ShowLCD1L(String cc)
  {
    lcd.setCursor(0, 0); //設置LCD螢幕座標
    // lcd.print("                    ");
    lcd.setCursor(0, 0); //設置LCD螢幕座標
    lcd.print(cc); //在第一行左邊顯示字串cc
  }
  
  // 在第一行右邊顯示字串cc
  void ShowLCD1M(String cc)
  {
    // lcd.setCursor(0,0);
    // lcd.print(" ");
    lcd.setCursor(13, 0); //設置LCD螢幕座標
    lcd.print(cc); //在第一行右邊顯示字串cc
  }
  
  // 在第二行顯示字串cc
  void ShowLCD2(String cc)
  {
    lcd.setCursor(0, 1); //設置LCD螢幕座標
    lcd.print("                    ");
    lcd.setCursor(0, 1); //設置LCD螢幕座標
    lcd.print(cc); //在目前位置顯示字串cc
  }
  
  void ShowLCD3(String cc)
  {
    //ShowLCD3()函數會將字串參數cc顯示在LCD螢幕的第3行上。
    lcd.setCursor(0, 2); //設置LCD螢幕座標
    lcd.print("                    ");
    lcd.setCursor(0, 2); //設置LCD螢幕座標
    lcd.print(cc);  //在目前位置顯示字串cc
  
  }
  void ShowLCD4(String cc)
  {
    lcd.setCursor(0, 3); //設置LCD螢幕座標
    lcd.print("                    ");
    lcd.setCursor(0, 3); //設置LCD螢幕座標
    lcd.print(cc);  //在目前位置顯示字串cc
  
  }
  
  
  void ShowString(String ss)
  {
    lcd.setCursor(0, 3); //設置LCD螢幕座標
    lcd.print("                    ");
    lcd.setCursor(0, 3); //設置LCD螢幕座標
    lcd.print(ss.substring(0, 19));
    //delay(1000);
  }
  
  void ShowAPonLCD(String ss)  //顯示熱點
  {
    ShowLCD1M(ss) ; //顯示熱點
  
  }
  void ShowMAConLCD(String ss)  //顯示網路卡編號
  {
    //這個函數顯示所連接的網路卡編號，參數 ss 為網路卡編號。
    ShowLCD1L(ss) ; //顯示網路卡編號
  
  }
  void ShowIPonLCD(String ss)  //顯示連接熱點後取得網路編號(IP Address)
  {
    //這個函數顯示連接熱點後所取得的網路位址 (IP Address)，參數 ss 為網路編號。
    ShowLCD2("IP:" + ss) ; //顯示取得網路編號(IP Address)
  
  }
  void ShowInternetonLCD(String s1, String s2, String s3) //顯示網際網路連接基本資訊
  {
    //這個函數顯示網際網路的連接基本資訊，包括網路卡編號、連接的熱點名稱以及所取得的網路編號，分別由 s1、s2 和 s3 三個參數傳遞。
    ShowMAConLCD(s1)  ;  //顯示熱點
    ShowAPonLCD(s2) ;    //顯示連接熱點名稱
    ShowIPonLCD(s3) ;  //顯示連接熱點後取得網路編號(IP Address)}
  
  }
  
  void ShowSensoronLCD(double s1, double s2)    //顯示溫度與濕度在LCD上
  {
    //這個函數顯示溫度和濕度數值，參數 s1 為溫度值，s2 為濕度值。顯示的格式為 "T:溫度值.C H:濕度值.%"
    //    ShowLCD3("T:"+Double2Str(TempValue,1)+".C "+"T:"+Double2Str(HumidValue,1)+".%" ) ;
    ShowLCD3("T:" + Double2Str(s1, 1) + ".C " + "H:" + Double2Str(s2, 1) + ".%" ) ;
  }
