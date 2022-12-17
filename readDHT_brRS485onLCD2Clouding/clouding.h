//http://nuk.arduino.org.tw:8888/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
char iotserver[] = "nuk.arduino.org.tw";    // name address for Google (using DNS)
  // NCNU Clouding Server DNS name
int iotport = 8888 ;
// nuk.arduino.org.tw Clouding Server port : 8888
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):


String strGet="GET /dhtdata/dhDatatadd.php";
//  DB Agent 程式
String strHttp=" HTTP/1.1";   // Web  Socketing Header 
String strHost="Host: nuk.arduino.org.tw";  // Web  Socketing Header 
 String connectstr ;    //組成 Ｒｅｓｔｆｕｌ　Ｃｏｍｍｕｎｉｃａｔｉｏｎ　Ｓｔｒｉｎｇ變數
//http://nuk.arduino.org.tw:8888/dhtdata/dhDatatadd.php?MAC=349454353988&T=24.73&H=49.18
// host is  ==>nuk.arduino.org.tw:8888
//  app program is ==> /dhtdata/dhDatatadd.php
//  App parameters ==> ?MAC=349454353988&T=24.73&H=49.18
//http://nuk.arduino.org.tw:8888/dhtdata/dhDatatadd.php?MAC=349454353988&T=24.73&H=49.18

void SendtoClouding()     //傳送感測資料到雲端
{
//http://nuk.arduino.org.tw:8888/dhtdata/dhDatatadd.php?MAC=349454353988&T=24.73&H=49.18

// host is  ==>nuk.arduino.org.tw:8888
//  app program is ==> /dhtdata/dhDatatadd.php
//  App parameters ==> ?MAC=AABBCCDDEEFF&T=34&H=34
          connectstr = "?MAC="+MacData+"&T="+String(TempValue)+"&H="+String(HumidValue);
          //組成GET Format 的Resetful  的 Parameters 字串
         Serial.println(connectstr) ;
         if (client.connect(iotserver, iotport)) //  client.connect(iotserver, iotport)＝＝＞連線到雲端主機
         {
                 Serial.println("Make a HTTP request ... ");
                 //### Send to Server
                 String strHttpGet = strGet + connectstr + strHttp;
                 Serial.println(strHttpGet);    //　傳送通訊ｈｅａｄｅｒ
     
                 client.println(strHttpGet);    //　傳送通訊ｈｅａｄｅｒ
                 client.println(strHost);       //　結尾
                 client.println();     //　通訊結束
               } 
    
if (client.connected()) 
{ 
  client.stop();  // DISCONNECT FROM THE SERVER
}

    
}
 
