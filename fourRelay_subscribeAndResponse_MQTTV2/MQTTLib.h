#include <PubSubClient.h> // 匯入MQTT函式庫
#include <ArduinoJson.h> // 匯入Json使用元件

WiFiClient WifiClient; // 建立WiFi客戶端元件

PubSubClient mqttclient(WifiClient) ; // 建立一個 MQTT物件，名稱為mqttclient，使用 WifiClient 的網路連線端

#define mytopic "/ncnu/controller" // 設定MQTT主題
#define sendtopic "/ncnu/controller/status"    //這是老師用的，同學請要改


#define MQTTServer "broker.emqx.io" // 設定MQTT Broker的IP
#define MQTTPort 1883 // 設定MQTT Broker的Port
char* MQTTUser = ""; // 設定MQTT使用者帳號，這裡不需要
char* MQTTPassword = ""; // 設定MQTT使用者密碼，這裡不需要


char buffer[400];// 設定緩衝區大小

String SubTopic = String("/ncnu/relay/"); // 設定訂閱主題字串的前半部份
String FullTopic ; // 完整主題字串的字串變數
char fullTopic[35] ; // 完整主題字串的字元陣列

char clintid[20]; // 設定MQTT客戶端ID
void mycallback(char* topic, byte* payload, unsigned int length)  ;// 設定MQTT接收訊息的回調函式
void connectMQTT() ; // 設定MQTT連線函式

 
void fillCID(String mm)
{
   /*
   // 這個函式將從傳入的字串(mm)中產生一個隨機的 client id，
// 先將第一個字元設為 't'，第二個字元設為 'w'，
// 接著將 mm 複製到 clintid 陣列中，並在最後面補上換行符號 '\n'
    */
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
    boolean ststusret= false;   //是否讀取成功
    int loopcnt=1;    //讀取次數計數器
    phasestage=1 ;    //控制讀取位址、速率、繼電器狀態、IO狀態階段
    flag1 = false ;    //讀取成功裝置位址控制旗標
    flag2 = false ;   //讀取成功裝置速率控制旗標
    flag3 = false ;    //讀取成功繼電器狀態控制旗標
    flag4 = false ;   //讀取成功IO狀態控制旗標
    boolean loopin=true;    //產生並設定讀取資料迴圈控制旗標
    while (loopin)    //loop for read device all information completely
    {
       if (phasestage == 5 ) //到第五階段代表讀取所有資料完成
            {
                Serial.println("Pass stage 5") ;    //印出第五階段
                //setjsondata(String vmac, String vaddress, long vbaud, String vrelay, String vIO)
                setjsondata(MacData,IPData, DeviceAddress, DeviceBaud, RelayStatus, IOStatus) ; //combine all sensor data into json document
                 mqttclient.publish(sendtopic, json_data);    //傳送buffer變數到MQTT Broker，指定mytopic傳送
                ststusret=true ;    //資料完全讀取成功
                loopin=false ;  //跳出讀取資料迴圈
                break;
                
            }
        if (phasestage == 1 && !flag1 ) //第一階段讀取溫度階段，且未讀取成功溫度值
        {
            requestaddress() ;    //讀取裝置位址
        }
      if (phasestage == 2  && !flag2) //第二階段讀取濕度階段，且未讀取成功濕度值
        {
            requestbaud() ;   //讀取裝置通訊速率
        }
      if (phasestage == 3 && !flag3 ) //第三階段讀取溫度階段，且未讀取成功溫度值
        {
            requestrelay() ;    //讀取繼電器狀態
        }
      if (phasestage == 4  && !flag4) //第四階段讀取濕度階段，且未讀取成功濕度值
        {
            requestIO() ;   //讀取光偶合狀態
        }
          delay(200);   //延遲0.2秒
        receivedlen = GetRS485data(&receiveddata[0]) ;  //資料長度
        if (receivedlen >2)   //如果資料長度大於二
          {
            loopcnt++ ;     //讀取次數計數器加一
            readcrc = ModbusCRC16(&receiveddata[0],receivedlen-2) ; //read  crc16 of readed data
            //計算資料之crc16資料
            Hibyte = receiveddata[receivedlen-1]  ; //計算資料之crc16資料之高位元
            Lobyte = receiveddata[receivedlen-2]  ; //計算資料之crc16資料之低位元
                  Serial.print("Return Data Len:") ;    //印出訊息
                  Serial.print(receivedlen) ;   //印出資料長度
                  Serial.print("\n") ;    //印出訊息
                  Serial.print("CRC:") ;    //印出訊息
                  Serial.print(readcrc) ;   //印出CRC16資料
                  Serial.print("\n") ;    //印出訊息
                  Serial.print("Return Data is:(");   //印出訊息
                  Serial.print(RS485data2String(&receiveddata[0],receivedlen)) ;
                  Serial.print(")\n");    //印出訊息
              if (!CompareCRC16(readcrc,Hibyte,Lobyte) )  //使用crc16判斷讀取資料是否不正確
              {
                // error happen and resend request and re-read again
                 continue;    //失敗
              }
    //------------------------------------------------------
            if (phasestage == 1 && !flag1 ) //第一階段讀取裝置位址，且未讀取成功裝置位址
            {
                Serial.println("Pass stage 1") ;
                DeviceAddress = print2HEX((int)receiveddata[4]) ; //讀取裝置位址
                phasestage = 2;   //下次進入第二階段
                flag1 = true ;    //成功讀取裝置位址
                 continue;
            }
          if (phasestage == 2  && !flag2) //第二階段讀取裝置速率，且未讀取成功裝置速率
            {
                Serial.println("Pass stage 2") ;
                DeviceBaud = TranBaud((int)receiveddata[4]) ; //讀取裝置位址
                phasestage = 3; //下次進入第三階段
                flag2 = true ;    //成功讀取裝置位址
                 continue;
            }
          if (phasestage == 3 && !flag3 ) //第三階段讀取繼電器狀態，且未讀取成功繼電器狀態
            {
                Serial.println("Pass stage 3") ;
                RelayStatus = byte2bitstring((int)receiveddata[3]) ; //讀取繼電器狀態
                phasestage = 4 ;    //下次進入第四階段
                flag3 = true ;    //成功讀取繼電器狀態
                 continue;
            }
          if (phasestage == 4  && !flag4) //第四階段讀取IO光耦合，且未讀取成功IO光耦合值
            {
                Serial.println("Pass stage 4") ;
                IOStatus = byte2bitstring((int)receiveddata[3]) ; //讀取IO光耦合值
                phasestage = 5 ;    //下次進入第五階段
                flag4 = true ;    //成功讀取繼電器狀態
                 continue;
            }
  
          } 

                  
         loopcnt++ ;    //讀取次數計數器加一
         if (loopcnt >20)   //讀取次數計數器超過20
         {
            ststusret=false;    //讀取失敗，讀取成功之旗標設定false
            loopin=false ;    //不再進入迴圈
            Serial.println("Exceed max loop and break out") ;
            break;
         } 
    }  //end of while for read modbus
    return ststusret;   //回傳讀取成功之旗標
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
  const char* st = "STATUS" ;
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
