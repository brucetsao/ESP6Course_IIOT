#include <PubSubClient.h>   //MQTT 函式庫
#include <ArduinoJson.h>    //Json 使用元件

WiFiClient WifiClient;    //　ｗｅｂ　ｓｏｃｋｅｔ　元件

PubSubClient mqttclient(WifiClient) ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件
//名稱為mqttclient，使用 WifiClient 的網路連線端

#define mytopic "/ncnu/controller"    //這是老師用的，同學請要改


#define MQTTServer "broker.emqx.io" 
#define MQTTPort 1883
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密


char buffer[400];

String SubTopic =String("/ncnu/relay/") ;
String FullTopic ;
char fullTopic[35] ;

char clintid[20];
void mycallback(char* topic, byte* payload, unsigned int length)  ;
 void connectMQTT() ;
 
void fillCID(String mm)
{
    // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;


}

void initMQTT()   //MQTT Broker 初始化連線
{
 mqttclient.setServer(MQTTServer, MQTTPort);
  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //mq.tongxinmao.com:18832
  mqttclient.setCallback(mycallback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    
     fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  
  mqttclient.setServer(MQTTServer, MQTTPort);   // 設定MQTT Server  URL and Port
  mqttclient.setCallback(mycallback); //設定MQTT 回叫系統使用的函式:mycallback
  connectMQTT();    //連到MQTT Server
  
  
}

 void connectMQTT()
 {

  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  //印出MQTT Client基本訊息
  while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword))  //沒有連線
  {
      Serial.print("-");    //印出"-"
      delay(1000);
    }
    Serial.print("\n");
  Serial.print("String Topic:[") ;
  Serial.print(mytopic) ;
  Serial.print("]\n") ;
  mqttclient.subscribe(mytopic); //訂閱我們的主旨
  Serial.println("\n MQTT connected!");
}

boolean ReadModbusDeviceStatus()
{   
    boolean ststusret= false;
    int loopcnt=1;
    phasestage=1 ;    //控制讀取位址、速率、繼電器狀態、IO狀態階段
    flag1 = false ;    //讀取成功裝置位址控制旗標
    flag2 = false ;   //讀取成功裝置速率控制旗標
    flag3 = false ;    //讀取成功繼電器狀態控制旗標
    flag4 = false ;   //讀取成功IO狀態控制旗標
    boolean loopin=true;
    while (loopin)    //loop for read device all information completely
    {
       if (phasestage == 5 ) //讀取所有資料完成
            {
                Serial.println("Pass stage 5") ;
                //setjsondata(String vmac, String vaddress, long vbaud, String vrelay, String vIO)
                setjsondata(MacData,IPData, DeviceAddress, DeviceBaud, RelayStatus, IOStatus) ; //combine all sensor data into json document
                ststusret=true ;
                loopin=false ;
                break;
                
            }
        if (phasestage == 1 && !flag1 ) //讀取溫度階段，且未讀取成功溫度值
        {
            requestaddress() ;    //讀取裝置位址
        }
      if (phasestage == 2  && !flag2) //讀取濕度階段，且未讀取成功濕度值
        {
            requestbaud() ;   //讀取裝置通訊速率
        }
      if (phasestage == 3 && !flag3 ) //讀取溫度階段，且未讀取成功溫度值
        {
            requestrelay() ;    //讀取繼電器狀態
        }
      if (phasestage == 4  && !flag4) //讀取濕度階段，且未讀取成功濕度值
        {
            requestIO() ;   //讀取光偶合狀態
        }
          delay(200);
        receivedlen = GetRS485data(&receiveddata[0]) ;
        if (receivedlen >2)
          {
            loopcnt++ ;
            readcrc = ModbusCRC16(&receiveddata[0],receivedlen-2) ; //read  crc16 of readed data
            Hibyte = receiveddata[receivedlen-1]  ;
            Lobyte = receiveddata[receivedlen-2]  ;
                  Serial.print("Return Data Len:") ;
                  Serial.print(receivedlen) ;
                  Serial.print("\n") ;
                  Serial.print("CRC:") ;
                  Serial.print(readcrc) ;
                  Serial.print("\n") ;
                  Serial.print("Return Data is:(");
                  Serial.print(RS485data2String(&receiveddata[0],receivedlen)) ;
                  Serial.print(")\n");
              if (!CompareCRC16(readcrc,Hibyte,Lobyte) )
              {
                // error happen and resend request and re-read again
                 continue;
              }
    //------------------------------------------------------
            if (phasestage == 1 && !flag1 ) //讀取裝置位址，且未讀取成功裝置位址
            {
                Serial.println("Pass stage 1") ;
                DeviceAddress = print2HEX((int)receiveddata[4]) ; //讀取裝置位址
                phasestage = 2;
                flag1 = true ;    //成功讀取裝置位址
                 continue;
            }
          if (phasestage == 2  && !flag2) //讀取裝置速率，且未讀取成功裝置速率
            {
                Serial.println("Pass stage 2") ;
                DeviceBaud = TranBaud((int)receiveddata[4]) ; //讀取裝置位址
                phasestage = 3;
                flag2 = true ;    //成功讀取裝置位址
                 continue;
            }
          if (phasestage == 3 && !flag3 ) //讀取繼電器狀態，且未讀取成功繼電器狀態
            {
                Serial.println("Pass stage 3") ;
                RelayStatus = byte2bitstring((int)receiveddata[3]) ; //讀取繼電器狀態
                phasestage = 4 ;
                flag3 = true ;    //成功讀取繼電器狀態
                 continue;
            }
          if (phasestage == 4  && !flag4) //讀取IO光耦合，且未讀取成功IO光耦合值
            {
                Serial.println("Pass stage 4") ;
                IOStatus = byte2bitstring((int)receiveddata[3]) ; //讀取IO光耦合值
                phasestage = 5 ;
                flag4 = true ;    //成功讀取繼電器狀態
                 continue;
            }
  
          } 

                  
         loopcnt++ ;
         if (loopcnt >20)
         {
            ststusret=false;
            loopin=false ;
            Serial.println("Exceed max loop and break out") ;
            break;
         } 
    }  //end of while for read modbus
    return ststusret;
}

void mycallback(char* topic, byte* payload, unsigned int length) 
{
  //mycallback(char* topic, byte* payload, unsigned int length)  參數格式固定，勿更改

    String payloadString;  // 將接收的 payload 轉成字串
    // 顯示訂閱內容
    Serial.print("Incoming:(") ;
    for (int i = 0; i < length; i++) 
    {
      payloadString = payloadString + (char)payload[i];
      //buffer[i]= (char)payload[i] ;
      Serial.print(payload[i],HEX) ;
    } 
    Serial.print(")\n") ;  
  
  payloadString = payloadString + '\0';  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");
  
  //---------------------
  Serial.print("Content [");
  Serial.print(payloadString);
  Serial.print("] \n");
  deserializeJson(json_received,payloadString);
  const char* mc = json_received["DEVICE"] ;
  const char* rq = json_received["COMMAND"] ;
  const char* st = String("STATUS").c_str() ;
 // Serial.println("======================");
///  Serial.println(json_received) ;
  Serial.println(mc) ;
   Serial.println("------------------------"); 
 Serial.println(rq) ;
  Serial.println("------------------------"); 
  Serial.println("======================");
  //boolean jsoncharstringcompare(const char* s1,String s2)
  if (strcmp(rq,st) )
    {
        Serial.println("decoding sensor data"); 
        ReadModbusDeviceStatus() ;
   }
   else
    {
        Serial.println("Error Command"); 
   }
   
}
