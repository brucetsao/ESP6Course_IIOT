#include <PubSubClient.h> // 匯入MQTT函式庫
#include <ArduinoJson.h> // 匯入Json使用元件

WiFiClient WifiClient; // 建立WiFi客戶端元件

PubSubClient mqttclient(WifiClient) ; // 建立一個 MQTT物件，名稱為mqttclient，使用 WifiClient 的網路連線端

#define mytopic "/ncnu/controller" // 設定MQTT主題

#define MQTTServer "broker.emqx.io" // 設定MQTT Broker的IP
#define MQTTPort 1883 // 設定MQTT Broker的Port
char* MQTTUser = ""; // 設定MQTT使用者帳號，這裡不需要
char* MQTTPassword = ""; // 設定MQTT使用者密碼，這裡不需要

char buffer[400]; // 設定緩衝區大小

String SubTopic = String("/ncnu/relay/"); // 設定訂閱主題字串的前半部份
String FullTopic ; // 完整主題字串的字串變數
char fullTopic[35] ; // 完整主題字串的字元陣列

char clintid[20]; // 設定MQTT客戶端ID

void mycallback(char* topic, byte* payload, unsigned int length) ; // 設定MQTT接收訊息的回調函式
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
  // 這個函式用來初始化 MQTT Broker 連線設定
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



}
