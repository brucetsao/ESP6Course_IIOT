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
char iotserver[] = "163.22.24.51";    // name address for Google (using DNS)
int iotport = 9999 ;
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
String strGet="GET /dhtdata/dhtadd.php";
String strHttp=" HTTP/1.1";
String strHost="Host: 163.22.24.51:9999";  //OK
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
  MacData = GetMacAddress() ;
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
    phasestage=1 ;
    flag1 = false ;
    flag2 = false ;
    

} 
void requesttemperature()
{
    Serial.println("now send data to device") ;
    Serial2.write(Str1,8);
     Serial.println("end sending") ;      
}
void requesthumidity()
{
    Serial.println("now send data to device") ;
    Serial2.write(Str2,8);
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
              *(dd+count) =cmd ;
              count++ ;

          }
          Serial.print("\n---------\n") ;
      }
      return count ;
    }
  
}
void loop()   
{
  if ((phasestage==1) && flag1 && flag1)
  {
      Serial.print("From Device :(") ;
      Serial.print((float)temp/10) ;
      Serial.print(" .C / ") ;
      Serial.print((float)humid/10) ;
      Serial.print(")\n") ;
      // Senddata to nas here
      flag1 = false ;
      flag2 = false ;
      SendtoNAS() ;
     delay(30000) ; 
     return ;     
  }
  if (phasestage == 1)
    {
        requesttemperature() ;
    }
  if (phasestage == 2)
    {
        requesthumidity() ;
    }

    delay(200);
    receivedlen = GetDHTdata(receiveddata) ;
    if (receivedlen >2)
      {
              Serial.print("Data Len:") ;
              Serial.print(receivedlen) ;
              Serial.print("\n") ;
              Serial.print("CRC:") ;
              Serial.print(ModbusCRC16(receiveddata,receivedlen-2)) ;
              Serial.print("\n") ;
              for (int i = 0 ; i <receivedlen ; i++)
                {
                  Serial.print(receiveddata[i],HEX) ;
                  Serial.print("/") ;
                }
                  Serial.print("...\n") ;
              Serial.print("CRC Byte:") ;
              Serial.print(receiveddata[receivedlen-1],HEX) ;
              Serial.print("/") ;
              Serial.print(receiveddata[receivedlen-2],HEX) ;
              Serial.print("\n") ;
          if (CompareCRC16(ModbusCRC16(receiveddata,receivedlen-2),receiveddata[receivedlen-1],receiveddata[receivedlen-2]))
            {
                if (phasestage == 1)
                {
                    temp = receiveddata[3]*256+receiveddata[4] ;
                    flag1 = true ;
                    phasestage=2 ;
                    return ;
                }
                if (phasestage == 2)
                {
                    humid = receiveddata[3]*256+receiveddata[4] ;
                    flag2 = true ;
                    phasestage=1 ;
                    return ;
                }

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


void SendtoNAS()
{
    //http://163.22.24.51:9999/dhtdata/dhtadd.php?MAC=AABBCCDDEEFF&T=34&H=34

   //AccessOn() ; 
  connectstr = "?MAC=" + MacData + "&T=" + String((double)temp/10) + "&H=" + String((double)humid/10);
  Serial.println(connectstr) ;
  if (client.connect(iotserver, iotport)) {
    Serial.println("Make a HTTP request ... ");
    //### Send to Server
    String strHttpGet = strGet + connectstr + strHttp;
    Serial.println(strHttpGet);

    client.println(strHttpGet);
    client.println(strHost);
    client.println();
  }

  if (client.connected())
  {
    client.stop();  // DISCONNECT FROM THE SERVER
  }

  //AccessOff() ;

}
