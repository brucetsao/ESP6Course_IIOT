#include <ArduinoJson.h>  //Json 使用元件
/*
 REQUEST=
 {
    DEVICE:"E89F6DE869F8",
    COMMAND:STATUS
 }
 
 DOJOB =
 {
    DEVICE:"E89F6DE869F8",
    TARGET:RELAY/IO
    SET:1~N(0=ALL),
    COMMAND:ON/OFF
  }



 
 REQUESTSTATUS=
 {
    DEVICE:"E89F6DE869F8",
    RELAY:{0,0,1,1},
    IO:{0,0,0,0}
  }
    
 EX.
 */


DeserializationError json_error;
StaticJsonDocument<2000> json_doc ;    //處理
StaticJsonDocument<1000> json_received;

char json_data[1000];

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

void setjsondata(String vmac,String vip, String vaddress, long vbaud, String vrelay, String vIO) //combine all sensor data into json document
{
  //combine all sensor data into json document
  json_doc["MAC"] = vmac ;
  json_doc["DeviceAddress"] = vaddress ;
  json_doc["DeviceBaud"] = vbaud ;
  json_doc["Relay"] = vrelay ;
  json_doc["IO"] = vIO ;
   serializeJson(json_doc, json_data); 
   Serial.print("Json Data:(\n");
  // Serial.print(json_data);
   serializeJsonPretty(json_doc, Serial);
   Serial.print("\n)\n");
   
}
void TranChar2Json(char *p)
{
    String input = String(p) ;
    deserializeJson(json_doc,input );
   // json_doc.prettyPrintTo(Serial) ;
   // Serial.println("");
}
