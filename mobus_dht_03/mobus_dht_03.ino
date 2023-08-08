//-----------------------
#include "arduino_secrets.h"
#include "crc16.h"
#include <WiFi.h>
#include <WiFiMulti.h>


WiFiMulti wifiMulti;


//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
          // your network key Index number (needed only for WEP)

  IPAddress ip ;
  long rssi ;
  
int status = WL_IDLE_STATUS;
char iotserver[] = "iot.nuk.edu.tw";    // name address for Google (using DNS)
int iotport = 80 ;
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
String strGet="GET /env/tempadd.php";
String strHttp=" HTTP/1.1";
String strHost="Host: iot.nuk.edu.tw";  //OK
 String connectstr ;
String MacData ;
WiFiClient client;

//  control parameter 
boolean systemstatus = false ;
boolean Reading = false ;
boolean Readok = false ;
// int trycount = 0 ;
int wifierror = 0 ;
boolean btnflag = false ;
//---------------

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
  ttt.toUpperCase() ;
  return ttt ;
}





void printWiFiStatus() {
  // printStrTemp the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

String GetMacAddress() {
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







void initAll()
{
    Serial.begin(9600);
     Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("System Start") ;  
    wifiMulti.addAP("BRUCE", "12345678");
    wifiMulti.addAP("R1-1117", "3716037160");

    Serial.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

  
 
}



void ShowInternet()
{
    ShowMAC() ;
    ShowIP()  ;
}

void ShowAP(String apname)
{

}
void ClearShow()
{

}

void ShowMAC()
{


}
void ShowIP()
{


}




void ShowString(String ss)
{

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





void setup() 
{
        
  //Initialize serial and wait for port to open:
    initAll() ;
/*
  WiFi.disconnect(true);
  WiFi.setSleep(false);

  
    wifiMulti.addAP(AP2, PW2);
    wifiMulti.addAP(AP1, PW1);

    Serial.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Successful Connectting to Access Point:");
        Serial.println(WiFi.SSID());
        Serial.print("\n");
        
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
         ShowAP(WiFi.SSID()) ;
              
    }
    
  printWiFiStatus();
  ShowInternet() ;
  ShowString("init System,Pls Wait") ;
*/

    

} 
void requesttemperature()
{
    Serial.println("now send data to device") ;
    Serial2.write(StrTemp,8);
     Serial.println("end sending") ;      
}
void requestdata()
{
    Serial.println("now send request to device") ;
    Serial2.write(StrTemp,8);

     Serial.println("end sending") ;      
}
int GetDHTdata(byte *dd)
{
  int count = 0 ;
  long strtime= millis() ;
  while ((millis() -strtime) < 2000)
    {
    if (Serial2.available()>0)
      {
        Serial.println("Controler Respones") ;
          while (Serial2.available()>0)
          {
             Serial2.readBytes(&cmd,1) ;
             Serial.print(print2HEX((int)cmd)) ;
             count++ ;
              *(dd+count) =cmd ;
          }
          Serial.print("\n---------\n") ;
      }
      return count ;
    }
  
}
void loop()   
{
  requestdata();
    delay(200);
    receivedlen = GetDHTdata(&receiveddata[0]) ;
    if (receivedlen >2)
      {
          if (CompareCRC16(ModbusCRC16(receiveddata,receivedlen-2),receiveddata[receivedlen-2],receiveddata[receivedlen-1]))
            {
                temp = receiveddata[3]*256+receiveddata[4] ;
                humid = receiveddata[5]*256+receiveddata[6] ;
                Serial.print("From Device :(") ;
                Serial.print(temp) ;
                Serial.print(" .C / ") ;
                Serial.print(humid) ;
                Serial.print(")\n") ;
            }
      }
    /*
    if (Serial2.available()>0)
      {
        Serial.println("Controler Respones") ;
          while (Serial2.available()>0)
          {
             Serial2.readBytes(&cmd,1) ;
             Serial.print(print2HEX((int)cmd)) ;

          }
          Serial.print("\n---------\n") ;
      }
      */
    delay(5000) ;
} // END Loop
