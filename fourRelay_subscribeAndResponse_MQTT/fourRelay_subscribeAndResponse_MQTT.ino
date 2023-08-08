#include <String.h>   //String 使用必備函示庫
#include "initPins.h"     //系統共用函數
#include "crc16.h"   //arduino json 使用必備函示庫
#include "RS485Lib.h"   //arduino json 使用必備函示庫
#include "JSONLIB.h"   //arduino json 使用必備函示庫
#include "LCDSensor.h"    // LCD 2004 共用函數
#include "MQTTLib.h"    // ＭＱＴＴ　Ｂｒｏｋｅｒ 共用函數








//--------------------
void ShowInternetonLCD(String s1,String s2,String s3) ;  //顯示網際網路連接基本資訊
void ShowSensoronLCD(double s1, double s2) ;   //顯示溫度與濕度在LCD上
boolean initWiFi()  ; //網路連線，連上熱點
 void initAll() ;    //系統初始化
//----------------
 void connectMQTT() ;
 void mycallback(char* topic, byte* payload, unsigned int length)  ;
//----------------

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化
    initRS485()  ;  //啟動Modbus  
    initWiFi() ;  //網路連線，連上熱點
  
    initMQTT()  ;//MQTT Broker 初始化連線
    
      //網路連線，連上熱點
   if (CheckWiFi())
    {
      TurnonWifiLed() ;  //打開Wifi連接燈號
      Serial.println("Wifi is Connected.....");
    }
    else
    {
      TurnoffWifiLed() ;  //打開Wifi連接燈號
      Serial.println("Wifi is lost");
    }    
    BeepOff() ;   //關閉嗡鳴器
   ShowInternet()  ; //顯示網際網路連接基本資訊
      
    initLCD()  ;  //初始化LCD螢幕
    ShowInternetonLCD(MacData,APname,IPData)  ; //顯示網際網路連接基本資訊



  Serial.println("System  Ready");
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
 // delay(10000) ;
}

void initAll()     //系統初始化
{
    Serial.begin(9600) ;
    Serial.println("System Start");
     MacData = GetMacAddress() ; //取得網路卡編號
}
