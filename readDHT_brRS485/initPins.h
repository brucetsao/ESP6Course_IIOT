#define _Debug 1    //輸出偵錯訊息
#define _debug 1    //輸出偵錯訊息
#define initDelay   6000    //初始化延遲時間
#define loopdelay 5000   //loop 延遲時間


#define RelayPin 4
#define terminator '\n'
int ccmd = -1 ;
String cmdstr ;

#include <WiFi.h>   //使用網路函式庫
#include <WiFiClient.h>   //使用網路用戶端函式庫
#include <WiFiMulti.h>    //多熱點網路函式庫

WiFiMulti wifiMulti;    //產生多熱點連線物件



String IpAddress2String(const IPAddress& ipAddress) ;




  IPAddress ip ;    //網路卡取得IP位址之原始型態之儲存變數
  String IPData ;   //網路卡取得IP位址之儲存變數
  String APname ;   //網路熱點之儲存變數
  String MacData ;   //網路卡取得網路卡編號之儲存變數 
  long rssi ;   //網路連線之訊號強度'之儲存變數
  int status = WL_IDLE_STATUS;  //取得網路狀態之變數
  
void initWiFi()   //網路連線，連上熱點
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
  

 
}
void ShowInternet()   //秀出網路連線資訊
{
  Serial.print("MAC:") ;
  Serial.print(MacData) ;
  Serial.print("\n") ;
  Serial.print("SSID:") ;
  Serial.print(APname) ;
  Serial.print("\n") ;
  Serial.print("IP:") ;
  Serial.print(IPData) ;
  Serial.print("\n") ;    
}
//--------------------



//--------------------
long POW(long num, int expo)
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
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
    String tmp = "" ;
    for (int i = 0 ; i < sp; i++)
      {
          tmp.concat(' ')  ;
      }
    return tmp ;
}


String strzero(long num, int len, int base)
{
  String retstring = String("");
  int ln = 1 ;
    int i = 0 ; 
    char tmp[10] ;
    long tmpnum = num ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base) ;
        tmp[ln-1] = hexcode[tmpchr] ;
        ln++ ;
         tmpnum = (long)(tmpnum/base) ;
 
        
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
    
  return retstring;
}

unsigned long unstrzero(String hexstr, int base)
{
  String chkstring  ;
  int len = hexstr.length() ;
  
    unsigned int i = 0 ;
    unsigned int tmp = 0 ;
    unsigned int tmp1 = 0 ;
    unsigned long tmpnum = 0 ;
    String hexcode = String("0123456789ABCDEF") ;
    for (i = 0 ; i < (len ) ; i++)
    {
  //     chkstring= hexstr.substring(i,i) ; 
      hexstr.toUpperCase() ;
           tmp = hexstr.charAt(i) ;   // give i th char and return this char
           tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(base,(len -i -1) )  ;
 
        
    }
  return tmpnum;
}





String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
  }
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
        Tmp.concat(print2HEX(mac[i])) ;
    }
    Tmp.toUpperCase() ;
  return Tmp ;
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
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}




String chrtoString(char *p)
{
    String tmp ;
    char c ;
    int count = 0 ;
    while (count <100)
    {
        c= *p ;
        if (c != 0x00)
          {
            tmp.concat(String(c)) ;
          }
          else
          {
              return tmp ;
          }
       count++ ;
       p++;
       
    }
}


void CopyString2Char(String ss, char *p)
{
         //  sprintf(p,"%s",ss) ;

  if (ss.length() <=0)
      {
           *p =  0x00 ;
          return ;
      }
    ss.toCharArray(p, ss.length()+1) ;
   // *(p+ss.length()+1) = 0x00 ;
}

boolean CharCompare(char *p, char *q)
  {
      boolean flag = false ;
      int count = 0 ;
      int nomatch = 0 ;
      while (flag <100)
      {
          if (*(p+count) == 0x00 or *(q+count) == 0x00)
            break ;
          if (*(p+count) != *(q+count) )
              {
                 nomatch ++ ; 
              }
             count++ ; 
      }
     if (nomatch >0)
      {
        return false ;
      }
      else
      {
        return true ;
      }
      
        
  }


String Double2Str(double dd,int decn)
{
    int a1 = (int)dd ;
    int a3 ;
    if (decn >0)
    {    
        double a2 = dd - a1 ;
        a3 = (int)(a2 * (10^decn));
    }
    if (decn >0)
    {
        return String(a1)+"."+ String(a3) ;
    }
    else
    {
      return String(a1) ;
    }
  
}
