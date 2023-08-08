#include <ArduinoJson.h>  //Json 使用元件
/*
  REQUEST=
  {
    DEVICE:"E89F6DE869F8",
    COMMAND:STATUS
  }

  DOJOB =
{ 
  "DEVICE":"E89F6DE869F8", 
  "SENSOR":"RELAY" ,(RELAY=繼電器，INPUT=IO輸入)
  "SET":1 ,         (n=第幾組)
  "OPERATION":"HIGH"    (RELAY=繼電器, HIGHn=第幾組)
}




  REQUESTSTATUS=
  {
    DEVICE:"E89F6DE869F8",
    RELAY:{0,0,1,1},
    IO:{0,0,0,0}
  }

  EX.
*/
DeserializationError json_error; // 建立一個DeserializationError物件，用來儲存JSON解析過程中的錯誤
StaticJsonDocument<2000> json_doc; // 建立一個容量為2000位元組的靜態JSON文件(json_doc)
StaticJsonDocument<2000> json_received; // 建立一個容量為2000位元組的靜態JSON文件(json_received)
char json_data[2000]; // 建立一個長度為2000的字元陣列，用來儲存JSON資料

/*
  其中，靜態JSON文件是指在編譯時期就已經固定容量，
  因此可以有效節省動態記憶體使用。
  而DeserializationError物件則是
  用來儲存JSON解析過程中的錯誤訊息，
  可以在開發和除錯時提供相關資訊。
  至於char陣列json_data則是用來儲存JSON資料，
  這個陣列的長度是1000位元組。

*/

/*
  boolean jsoncharstringcompare(const char* s1,String s2)
  {
  char cc;
  int cnt=0;
  int cmpcnt=0 ;
  boolean ret=true;
  cc = *(cnt)s1 ;
  while (cc != '\0')
  {
      if (cnt > s2.length)
      {
          if (cmpcnt >0)
            {
              ret=true;
            }
            else
            {
              ret=false;
            }
            break ;
      }
      if (cc == (char)s2.substring(cnt,cnt))
        {
            cmpcnt++;
            cnt++;
            continue
        }
        else
        {
          ret = false;
          break;
        }
    return ret;
  }
  }
*/
void initjson()
{

}

void setjsondata(String vmac, String vip, String vaddress, long vbaud, String vrelay, String vIO)
{
  json_doc.clear() ;    //重設空值
  // 建立一個JSON物件(json_doc)，用來儲存JSON資料
  // 將vmac、vaddress、vbaud、vrelay、vIO存放到json_doc物件中，對應的Key值為"MAC"、"DeviceAddress"、"DeviceBaud"、"Relay"、"IO"
  json_doc["MAC"] = vmac ;
  json_doc["DeviceAddress"] = vaddress ;
  json_doc["DeviceBaud"] = vbaud ;
  json_doc["Relay"] = vrelay ;
  json_doc["IO"] = vIO ;

  // 使用ArduinoJSON函式庫的serializeJson()方法，將json_doc中的json物件序列化成字串，存放在名為json_data的String物件中
  serializeJson(json_doc, json_data);

  // 在Serial埠輸出"Json Data:("字串，以便區分不同的輸出訊息
  Serial.print("Json Data:(\n");

  // 使用ArduinoJSON函式庫的serializeJsonPretty()方法，將json_doc中的json物件序列化成可讀性較高的格式，並輸出至Serial埠
  serializeJsonPretty(json_doc, Serial);

  // 在Serial埠輸出換行字元，以便區分不同的輸出訊息
  Serial.print("\n)\n");
}

void TranChar2Json(char *p)  //轉換讀入的json document的字串，轉入JSON 元件之json_doc 變數
{
  // 建立一個String物件並將char指標p轉換成字串傳入
  String input = String(p) ;

  // 使用ArduinoJSON函式庫的deserializeJson()方法，將json字串轉換成json物件，存放在名為json_doc的物件中
  deserializeJson(json_doc, input );

  // 使用ArduinoJSON函式庫的serializeJson()方法，將json_doc中的json物件序列化成字串，並輸出至Serial埠
  serializeJson(json_doc, Serial);
  // 在Serial埠輸出換行字元，以便區分不同的輸出訊息
  Serial.println("");
}

void Printjson()    //解譯得到json 文件，並列印解譯後每一個元件的內容
{
  /*

    Printjson() : 解譯得到json 文件，並列印解譯後每一個元件的內容
    參數:
    無參數
    回傳值:
    無回傳值
    動作:
    從 json_doc 解譯出 "DEVICE"、"SENSOR"、"SET"、"OPERATION" 的值
    以 Serial.print() 列印出每個元件的內容
    說明:
    本函式會從全域變數 json_doc 解析出指定元件的值，並列印出來。
    列印的內容包括 "Device Name"、"Sensor Name"、"Which set"、"OPERATION"，
    分別對應 json_doc 中的 "DEVICE"、"SENSOR"、"SET"、"OPERATION"。
    列印完成後，不會有任何回傳值。
  */

  // 從 json_doc 解譯出 "DEVICE" 的值
  const char* devname = json_doc["DEVICE"] ;
  // 從 json_doc 解譯出 "SENSOR" 的值
  const char* sensorname = json_doc["SENSOR"] ;
  // 從 json_doc 解譯出 "SET" 的值
  int setnum = json_doc["SET"] ;
  // 從 json_doc 解譯出 "OPERATION" 的值
  const char* op = json_doc["OPERATION"] ;

  // 列印 "Device Name:" 和 "DEVICE" 的值
  Serial.print("Device Name:") ;
  Serial.print(devname) ;
  Serial.print("\n") ;

  // 列印 "Sensor Name:" 和 "SENSOR" 的值
  Serial.print("Sensor Name:") ;
  Serial.print(sensorname) ;
  Serial.print("\n") ;

  // 列印 "Which set:" 和 "SET" 的值
  Serial.print("Which set:") ;
  Serial.print(setnum) ;
  Serial.print("\n") ;

  // 列印 "OPERATION:" 和 "OPERATION" 的值
  Serial.print("OPERATION:") ;
  Serial.print(op) ;
  Serial.print("\n") ;

}
