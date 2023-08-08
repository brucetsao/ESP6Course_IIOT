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
StaticJsonDocument<2000> json_doc;

void initjson()
{

}
