#include <PubSubClient.h>   //MQTT 函式庫
#include <ArduinoJson.h>    //Json 使用元件

WiFiClient WifiClient;    //　ｗｅｂ　ｓｏｃｋｅｔ　元件

PubSubClient mqttclient(WifiClient) ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件
//名稱為mqttclient，使用 WifiClient 的網路連線端

#define mytopic "/ncnu/TeamTeacher"    //這是老師用的，同學請要改
#define mytopicA "/ncnu/TeamA"    //這是老師用的，同學請要改
#define mytopicB "/ncnu/TeamB"    //這是老師用的，同學請要改
#define mytopicC "/ncnu/TeamC"    //這是老師用的，同學請要改


#define MQTTServer "broker.emqx.io" 
#define MQTTPort 1883
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密


char buffer[400];

String SubTopic =String("/ncnu/ncnu310/") ;
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


void fillTopic(String mm)
{

    
      mm.toCharArray(&fullTopic[0],mm.length()+1) ;
    fullTopic[mm.length()+1] = '\n' ;

}


void Genjsondata(String myid,String myname, float temp, float humid )
{

    StaticJsonDocument<400> doc;    //產生一個json物件，取名doc，有400字元大小

  // 動態產生一個400長度的json物件，DynamicJsonDocument doc(400);

  // JSON input string.

  // Add values in the document
  // 下列格式化json
  doc["id"] = myid;
  doc["name"] = myname ;
 doc["temperature"] = String(temp) ;
 doc["humidity"] = String(humid);

 //----------------------------

serializeJson(doc, buffer); 
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
  //FullTopic = SubTopic+MacData ;
  Serial.print("String Topic:[") ;
  Serial.print(mytopic) ;
  Serial.print("]\n") ;
  // fillTopic(FullTopic) ;

  Serial.print("char Topic:[") ;
  Serial.print(fullTopic) ;
  Serial.print("]\n") ;   
  mqttclient.subscribe(mytopic); //訂閱我們的主旨
  Serial.println("\n MQTT connected!");


  // client.unsubscribe("/hello");
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
