#include <String.h>   //String 使用必備函示庫
#include "initPins.h"     //系統共用函數
#include "RS485Lib.h"   //arduino json 使用必備函示庫
#include "JSONLIB.h"   //arduino json 使用必備函示庫
#include "LCDSensor.h"    // LCD 2004 共用函數
//--------------------
void ShowInternetonLCD(String s1, String s2, String s3) ; //顯示網際網路連接基本資訊，這個函式接受三個字串參數，用於在LCD上顯示網際網路連線的基本資訊。
void ShowSensoronLCD(double s1, double s2) ;   //顯示溫度與濕度在LCD上，這個函式接受兩個雙精度浮點數參數，用於在LCD上顯示溫度和濕度。
boolean initWiFi()  ; //網路連線，連上熱點，這個函式返回布林值，用於初始化WiFi連接。當連接成功時返回 true，否則返回 false。
void initAll() ;    //這個函式用於初始化整個系統。
//----------------
void connectMQTT() ; //這個函式用於連接MQTT代理伺服器。
void mycallback(char* topic, byte* payload, unsigned int length)  ;
//這個函式用於處理MQTT訂閱時收到的資料。它有三個參數，分別是主題、資料內容和資料長度。

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  // initialize digital pin LED_BUILTIN as an output.
  // 將LED_BUILTIN腳位設為輸出模式
  initAll() ;   //系統初始化
  // 執行系統初始化
  initRS485()  ;  //啟動Modbus
  // 啟動Modbus通訊協定
  initWiFi() ;  //網路連線，連上熱點
  // 連接WiFi熱點

  // 網路連線，連上熱點
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
  // 關閉嗡鳴器
  ShowInternet()  ; //顯示網際網路連接基本資訊
  // 顯示網際網路連接基本資訊

  initLCD()  ;  //初始化LCD螢幕
  // 初始化LCD螢幕
  ShowInternetonLCD(MacData, APname, IPData)  ; //顯示網際網路連接基本資訊
  // 顯示網際網路連接基本資訊

  Serial.println("System  Ready");
  // 顯示System Ready的文字
  delay(initDelay) ;    //等待多少秒，進入loop()重複執行的程序
  // 延遲一段時間進入loop()重複執行的程序
}
// the loop function runs over and over again forever
void loop()
{
  /*

    本程式碼區塊在判斷序列埠是否有可用的資料，如果有則執行以下動作：
    印出訊息 "Command coming from Serial Port"，表示有從序列埠收到指令
    讀取從序列埠收到的資料到字串 uartstr
    以 Serial.println() 列印出字串 uartstr
    以 Serial.print() 列印出分隔線，並在分隔線旁邊印出收到的字元數 ll
    呼叫 TranChar2Json(uartstr)，將讀取到的字串轉換為 JSON 格式後存入全域變數 json_doc 中
    呼叫 Printjson()，解譯得到的 JSON 文件，並列印出每一個元件的內容
  */
  if (Serial.available() > 0) { // 檢查序列埠是否有可用的資料
    Serial.println("Command coming from Serial Port"); // 印出訊息表示有從序列埠收到指令
    int ll = SerialRead(uartstr); // 讀取從序列埠收到的資料到字串 uartstr
    Serial.println(uartstr); // 列印字串 uartstr
    Serial.print("----------(");
    Serial.print(ll); // 列印讀取到的字元數 ll
    Serial.print(")----------\n");
    TranChar2Json(uartstr); // 將讀取到的字串轉換為 JSON 格式後存入全域變數 json_doc 中
    Printjson(); // 解譯得到的 JSON 文件，並列印出每一個元件的內容
  }

}

void initAll()     //系統初始化
{
  Serial.begin(38400) ;
  Serial.println("System Start");
  MacData = GetMacAddress() ; //取得網路卡編號
  Serial.print("MAC:(");
  Serial.print(MacData);
  Serial.print(")\n");
}

int SerialRead(char *p)
{
  int cnt = 0 ;
  while (Serial.available() > 0) //如果 Commands from Serial Port
  {
    *(p + cnt) = Serial.read();
    cnt++ ;
    delayMicroseconds(SerialReaddelay) ;
  }
  *(p + cnt) = '\n' ;
  return cnt ;
}
