#include <String.h> // String 使用必備函示庫
#include "initPins.h" // 系統共用函數
#include "LCDSensor.h" // LCD 2004 共用函數
#include "MQTTLib.h" // ＭＱＴＴ　Ｂｒｏｋｅｒ 共用函數

// 註解掉不必要的函式庫
// #include "JSONLIB.h" // arduino json 使用必備函示庫

//--------------------
// 顯示網際網路連接基本資訊
void ShowInternetonLCD(String s1, String s2, String s3) ;
// 顯示溫度與濕度在LCD上
void ShowSensoronLCD(double s1, double s2) ;
// 網路連線，連上熱點
boolean initWiFi() ;
// 系統初始化
void initAll() ;

//----------------
// 連接MQTT Broker
void connectMQTT() ;
// 處理收到的訊息
void mycallback(char* topic, byte* payload, unsigned int length) ;
//----------------
// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  // initialize digital pin LED_BUILTIN as an output.
  // 系統初始化
  initAll();
  // 網路連線，連上熱點
  initWiFi();
  // MQTT Broker 初始化連線
  initMQTT();

  // 檢查網路連接狀態
  if (CheckWiFi()) {
    TurnonWifiLed(); // 打開Wifi連接燈號
    Serial.println("Wifi is Connected");
  } else {
    TurnoffWifiLed(); // 關閉Wifi連接燈號
    Serial.println("Wifi is lost");
  }

  BeepOff(); // 關閉嗡鳴器

  // 顯示網際網路連接基本資訊
  ShowInternet();

  // 初始化LCD螢幕
  initLCD();

  // 顯示網際網路連接基本資訊
  ShowInternetonLCD(MacData, APname, IPData);

  Serial.println("System Ready");

  delay(initDelay) ;    //等待多少秒，進入loop()重複執行的程序
}

// the loop function runs over and over again forever
void loop()
{

  if (!mqttclient.connected())   //如果MQTT 斷線(沒有連線)
  {
    Serial.println("connectMQTT  again");  //印出 "connectMQTT  again"
    connectMQTT();   //重新與MQTT Server連線
  }

  mqttclient.loop();    //處理MQTT 通訊處理程序
  //給作業系統處理多工程序的機會
  //delay(300) ;
}

void initAll()     //系統初始化
{
  Serial.begin(9600) ;
  Serial.println("System Start");
  MacData = GetMacAddress() ; //取得網路卡編號
  Serial.print("MAC:(");
   Serial.print(MacData); 
   Serial.print(")\n");  
}
