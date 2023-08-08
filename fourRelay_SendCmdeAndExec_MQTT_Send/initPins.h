#define _Debug 1    //輸出偵錯訊息
#define _debug 1    //輸出偵錯訊息
#define initDelay   6000    //初始化延遲時間
#define loopdelay 60000   //loop 延遲時間

//--------------------
#include <String.h>
#define Ledon HIGH    //打開LED 燈的電位設定
#define Ledoff LOW    //關閉LED 燈的電位設定
#define WifiLed 2   // 連線WIFI 成功之指示燈
#define AccessLED 15    // 系統運作之指示燈
#define BeepPin 4   // 控制器之嗡鳴器
/*
  這段程式碼是定義了一些常數，下面是每個常數的註解：

  _Debug 和 _debug：用於控制是否輸出偵錯訊息，因為兩者都設置為 1，所以輸出偵錯訊息。
  initDelay：初始化延遲時間，設置為 6000 毫秒。
  loopdelay：loop() 函數的延遲時間，設置為 60000 毫秒。
  WifiLed：WiFi 指示燈的引腳號，這裡設置為 2。
  AccessLED：設備狀態指示燈的引腳號，這裡設置為 15。
  BeepPin：蜂鳴器引腳號，這裡設置為 4。
*/

#include <WiFi.h>   //使用網路函式庫
#include <WiFiClient.h>   //使用網路用戶端函式庫
#include <WiFiMulti.h>    //多熱點網路函式庫

WiFiMulti wifiMulti;    //產生多熱點連線物件

 WiFiClient client;   //產生連線物件

String IpAddress2String(const IPAddress& ipAddress) ; // 將 IP 位址轉換為字串
void debugoutln(String ss) ;  // 輸出偵錯訊息
void debugout(String ss)  ; // 輸出偵錯訊息



IPAddress ip ;    //網路卡取得IP位址之原始型態之儲存變數
String IPData ;   //網路卡取得IP位址之儲存變數
String APname ;   //網路熱點之儲存變數
String MacData ;   //網路卡取得網路卡編號之儲存變數
long rssi ;   //網路連線之訊號強度'之儲存變數
int status = WL_IDLE_STATUS;  //取得網路狀態之變數
char uartstr[2000] ;
#define SerialReaddelay 800  
boolean initWiFi()   //網路連線，連上熱點
{
  //加入連線熱點資料
  wifiMulti.addAP("NCNUIOT", "12345678");  //加入一組熱點
  wifiMulti.addAP("NCNUIOT2", "12345678");  //加入一組熱點
  wifiMulti.addAP("NUKIOT", "iot12345");  //加入一組熱點

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  //通訊埠印出 "Connecting to "
  wifiMulti.run();  //多網路熱點設定連線
 while (WiFi.status() != WL_CONNECTED)     //還沒連線成功
  {
    // wifiMulti.run() 啟動多熱點連線物件，進行已經紀錄的熱點進行連線，
    // 一個一個連線，連到成功為主，或者是全部連不上
    // WL_CONNECTED 連接熱點成功
    Serial.print(".");   //通訊埠印出
    delay(500) ;  //停500 ms
     wifiMulti.run();   //多網路熱點設定連線
  }
    Serial.println("WiFi connected");   //通訊埠印出 WiFi connected
    Serial.print("AP Name: ");   //通訊埠印出 AP Name:
    APname = WiFi.SSID();
    Serial.println(APname);   //通訊埠印出 WiFi.SSID()==>從熱點名稱
    Serial.print("IP address: ");   //通訊埠印出 IP address:
    ip = WiFi.localIP();
    IPData = IpAddress2String(ip) ;
    Serial.println(IPData);   //通訊埠印出 WiFi.localIP()==>從熱點取得IP位址
    //通訊埠印出連接熱點取得的IP位址
     
  
  debugoutln("WiFi connected"); //印出 "WiFi connected"，在終端機中可以看到
  debugout("Access Point: "); //印出 "Access Point: "，在終端機中可以看到
  debugoutln(APname); //印出 APname 變數內容，並換行
  debugout("MAC address: "); //印出 "MAC address: "，在終端機中可以看到
  debugoutln(MacData); //印出 MacData 變數內容，並換行
  debugout("IP address: "); //印出 "IP address: "，在終端機中可以看到
  debugoutln(IPData); //印出 IPData 變數內容，並換行
  /*
    這些語句是用於在終端機中顯示一些網路相關的信息，
    例如已連接的Wi-Fi網路名稱、MAC地址和IP地址。
    debugout()和debugoutln()是用於輸出信息的自定義函數，
    這裡的程式碼假定這些函數已經在代碼中定義好了
  */

 return true ; 
}
boolean CheckWiFi() //檢查Wi-Fi連線狀態的函數
{
  //這些語句是用於檢查Wi-Fi連線狀態和顯示網路連線資訊的函數。CheckWiFi()函數檢查Wi-Fi連線狀態是否已連接，如果已連接，則返回true，否則返回false。
  if (WiFi.status() != WL_CONNECTED) //如果Wi-Fi未連接
  {
    return false ; //回傳false表示未連線
  }
  else //如果Wi-Fi已連接
  {
    return true ; //回傳true表示已連線
  }
}
void ShowInternet() //顯示網路連線資訊的函數
{
  //ShowInternet()函數則印出MAC地址、Wi-Fi名稱和IP地址的值。這些信息通常用於調試和診斷網路連線問題
  Serial.print("MAC:") ; //印出 "MAC:"
  Serial.print(MacData) ; //印出MacData變數的內容
  Serial.print("\n") ; //換行
  Serial.print("SSID:") ; //印出 "SSID:"
  Serial.print(APname) ; //印出APname變數的內容
  Serial.print("\n") ; //換行
  Serial.print("IP:") ; //印出 "IP:"
  Serial.print(IPData) ; //印出IPData變數的內容
  Serial.print("\n") ; //換行
}
//--------------------
/*
  這段程式碼中定義了三個函式：
  debugoutln、debugout

  debugoutln和debugout函式的功能，
  都是根據_Debug標誌來決定是否輸出字串。
  如果_Debug為true，
  則分別使用Serial.println和Serial.print輸出參數ss。
*/
void debugoutln(String ss)
{
  /*
   debugoutln(String ss)：印出一行字串，
   如果 _Debug 這個變數的值為真（即非零），
   則使用 Serial.println() 函式印出，
   否則不做任何事情。
   
   其中， _Debug 是一個全域變數，
   用來控制是否要印出除錯訊息。
   如果程式中有許多除錯訊息要印出，
   但在正式運作時不需要這些訊息，
   可以把 _Debug 設為零，
   這樣就不會有任何印出動作。
   而在除錯時，
   只需要把 _Debug 設為非零值，
   就可以印出所有的除錯訊息
   */
  if (_Debug)
    Serial.println(ss) ;
}
void debugout(String ss)
{
    /*
  debugout(String ss)：印出一個字串，
  如果 _Debug 這個變數的值為真，
  則使用 Serial.print() 函式印出，
  否則不做任何事情
  
   其中， _Debug 是一個全域變數，
   用來控制是否要印出除錯訊息。
   如果程式中有許多除錯訊息要印出，
   但在正式運作時不需要這些訊息，
   可以把 _Debug 設為零，
   這樣就不會有任何印出動作。
   而在除錯時，
   只需要把 _Debug 設為非零值，
   就可以印出所有的除錯訊息
   */
  if (_Debug)
    Serial.print(ss) ;
}


long POW(long num, int expo)
{
  /*
    POW函式是用來計算一個數的n次方的函式。
    函式有兩個參數，
    第一個參數為num，表示底數，
    第二個參數為expo，表示指數。
    如果指數為正整數，
    則使用迴圈來進行計算，否則返回1。
  */
  long tmp = 1 ;
  if (expo > 0)
  {
    for (int i = 0 ; i < expo ; i++)
      tmp = tmp * num ;
    return tmp ;
  }
  else
  {
    return tmp ;
  }
}

String SPACE(int sp)
{
  /*
    SPACE函式是用來生成一個包含指定數量空格的字串。
    函式只有一個參數，即空格的數量。
    使用for循環生成指定數量的空格，
    然後將它們連接起來形成一個字串，
    最後返回這個字串。
  */
  String tmp = "" ;
  for (int i = 0 ; i < sp; i++)
  {
    tmp.concat(' ')  ;
  }
  return tmp ;
}


// This function converts a long integer to a string with leading zeros (if necessary)
// The function takes in three arguments:
// - num: the long integer to convert
// - len: the length of the resulting string (including leading zeros)
// - base: the base to use for the conversion (e.g., base 16 for hexadecimal)
String strzero(long num, int len, int base)
{
  String retstring = String(""); // Initialize an empty string
  int ln = 1 ;  // Initialize the length counter to 1
  int i = 0 ; // Initialize the iteration variable to 0
  char tmp[10] ;  // Declare a character array to store the converted digits
  long tmpnum = num ; // Initialize a temporary variable to the value of num
  int tmpchr = 0 ;  // Initialize a temporary variable to store the converted digit
  char hexcode[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'} ; // Character array for hexadecimal digits
  // Character array for hexadecimal digits
  while (ln <= len) // Loop until the length of the string reaches the desired length
  {
    tmpchr = (int)(tmpnum % base) ; // Compute the remainder of tmpnum when divided by base
    tmp[ln - 1] = hexcode[tmpchr] ; // Store the corresponding character in tmp array
    ln++ ;
    tmpnum = (long)(tmpnum / base) ; // Divide tmpnum by base and update its value
  }
  for (i = len - 1; i >= 0 ; i --) // Iterate through the tmp array in reverse order
  {
    retstring.concat(tmp[i]); // Append each character to the output string
  }
  return retstring; // Return the resulting string
}
// This function converts a string representing a number in the specified base to an unsigned long integer
// The function takes in two arguments:
// - hexstr: the string to convert
// - base: the base of the number system used in hexstr (e.g., base 16 for hexadecimal)
unsigned long unstrzero(String hexstr, int base)
{
  /*
   上述函式的作用是
   將一個十六進位的字串轉換為對應的 unsigned long 整數。
   程式碼的主要部分是 for 迴圈，
   它會迭代輸入字串中的每個字符，
   將其轉換為大寫字母，
   並根據其在十六進位數字字串中的索引值更新 tmpnum 變數的值
   。最後，該函式會回傳 unsigned long 整數的結果
   */
String chkstring; // 儲存檢查字串
int len = hexstr.length(); // 計算輸入字串的長度

unsigned int i = 0;
unsigned int tmp = 0;
unsigned int tmp1 = 0;
unsigned long tmpnum = 0; // 宣告變數

String hexcode = String("0123456789ABCDEF"); // 包含十六進位數字的字串

for (i = 0; i < len; i++) // 迭代輸入字串
{
hexstr.toUpperCase(); // 將輸入字串中的每個字符轉換為大寫字母
tmp = hexstr.charAt(i); // 取得位於 i 處的字符的 ASCII 碼
tmp1 = hexcode.indexOf(tmp); // 在 hexcode 字串中查找字符的索引值
tmpnum = tmpnum + tmp1 * POW(base, (len - i - 1)); // 根據當前字符的值更新 tmpnum 變數的值
}
return tmpnum; // 回傳 unsigned long 整數的結果
}

String  print2HEX(int number)
{
  // 這個函式接受一個整數作為參數
  // 將該整數轉換為16進制表示法的字串
  // 如果該整數小於16，則在字串前面加上一個0
  // 最後返回16進制表示法的字串
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number, HEX);
  }
  else
  {
    ttt = String(number, HEX);
  }
  ttt.toUpperCase() ;   //轉成大寫
  return ttt ;
}


String GetMacAddress()    //取得網路卡編號
{
  // the MAC address of your WiFi shield
  String Tmp = "" ;
  byte mac[6];
  
  // print your MAC address:
  WiFi.macAddress(mac);
  for (int i=0; i<6; i++)
    {
        Tmp.concat(print2HEX(mac[i])) ; //連接每一個MAC Address的byte
    }
    Tmp.toUpperCase() ;   //轉成大寫英文字
  return Tmp ;  //回傳網路卡編號
}

void ShowMAC()  //於串列埠印出網路卡號碼
{
  
  Serial.print("MAC Address:(");  //印出 "MAC Address:("
  Serial.print(MacData) ;   //印出 MacData 變數內容
  Serial.print(")\n");    //印出 ")\n"


}
String IpAddress2String(const IPAddress& ipAddress)
{
  //回傳ipAddress[0-3]的內容，以16進位回傳
  // The function returns a string containing the IP address in the format "x.x.x.x", where x is a number between 0 and 255 representing each segment of the IP address.
  
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}




//將字元陣列轉換為字串
String chrtoString(char *p)
{
  String tmp ; //宣告一個名為tmp的字串
  char c ; //宣告一個名為c的字元
  int count = 0 ; //宣告一個名為count的整數變數，初始值為0
  while (count < 100) //當count小於100時進入迴圈
  {
    c = *p ; //將指標p指向的值指派給c
    if (c != 0x00) //當c不為空值(0x00)時
    {
      tmp.concat(String(c)) ; //將c轉換為字串型別並連接到tmp字串中
    }
    else //當c為空值(0x00)時
    {
      return tmp ; //回傳tmp字串
    }
    count++ ; //count值增加1
    p++; //將指標p指向下一個位置

  }
}


void CopyString2Char(String ss, char *p)  //將字串複製到字元陣列
{
  //將字串複製到字元陣列
  if (ss.length() <= 0) //當ss字串長度小於等於0時
  {
    p = 0x00 ; //將指標p指向的位置設為空值(0x00)
    return ; //直接回傳
  }
  ss.toCharArray(p, ss.length() + 1) ; //將ss字串轉換為字元陣列，並存儲到指標p指向的位置
  //(p+ss.length()+1) = 0x00 ;
}
boolean CharCompare(char *p, char *q)
{
  boolean flag = false ; //宣告一個名為flag的布林變數，初始值為false
  int count = 0 ; //宣告一個名為count的整數變數，初始值為0
  int nomatch = 0 ; //宣告一個名為nomatch的整數變數，初始值為0
  
  while (flag < 100) //當flag小於100時進入迴圈
  {
     if (*(p+count) == 0x00 or *(q+count) == 0x00) //當指標p或指標q指向的值為空值(0x00)時
        break ; //跳出迴圈
      if (*(p+count) != *(q+count) )  
        {
          nomatch ++ ; //nomatch值增加1
        }
        count++ ; //count值增加1
  }   //end of  while (flag < 100) 
  if (nomatch > 0)
  {
    return false ;
  }
  else
  {
    return true ;
  }


}
// This function converts a double number to a String with specified number of decimal places
// dd: the double number to be converted
// decn: the number of decimal places to be displayed
String Double2Str(double dd, int decn)
{
  // Extract integer part of the double number
  int a1 = (int)dd ;
  int a3 ;

  // If decimal places are specified
  if (decn > 0)
  {
    // Extract decimal part of the double number
    double a2 = dd - a1 ;
    // Multiply decimal part with 10 to the power of specified number of decimal places
    a3 = (int)(a2 * (10 ^ decn));
  }

  // If decimal places are specified, return the String with decimal places
  if (decn > 0)
  {
    return String(a1) + "." + String(a3) ;
  }
  // If decimal places are not specified, return the String with only the integer part
  else
  {
    return String(a1) ;
  }

}

//--------------GPIO Function

void TurnonWifiLed()    //打開Wifi連接燈號
{
  digitalWrite(WifiLed, Ledon) ;
}

void TurnoffWifiLed()   //關閉Wifi連接燈號
{
  digitalWrite(WifiLed, Ledoff) ;
}

void AccessOn()   //打開動作燈號
{
  digitalWrite(AccessLED, Ledon) ;
}

void AccessOff()    //關閉動作燈號
{
  digitalWrite(AccessLED, Ledoff) ;
}
void BeepOn()   //打開嗡鳴器
{
  digitalWrite(BeepPin, Ledon) ;
}
void BeepOff()  //關閉嗡鳴器
{
  digitalWrite(BeepPin, Ledoff) ;
}
/*
  上面的程式碼是定義了幾個控制 LED 和蜂鳴器的函數，
  這些函數都是透過 Arduino 的 digitalWrite() 函數來控制 LED 或蜂鳴器輸出的狀態，
  其中 Ledon 和 Ledoff 是定義高低電位狀的常數，

  
  GPIO 是 General Purpose Input/Output 的簡稱，
  是微控制器的一種外部接口。
  程式中的 digitalWrite 可以控制 GPIO 的狀態，
  其中 WifiLed、AccessLED、BeepPin 都是 GPIO 的編號，
  Ledon 和 Ledoff 則是表示 GPIO 的狀態，
  通常是高電位或低電位的編號，
  具體的編號會依據使用的裝置而有所不同。
  因此，以上程式碼可以用來控制 Arduino 的 GPIO 狀態，
  從而控制裝置的狀態。
*/
