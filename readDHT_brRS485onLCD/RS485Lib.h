//#include <HardwareSerial .h>
#include <String.h>
#define SERIAL_BAUD 9600

HardwareSerial RS485Serial(2);//声明串口2

#define RXD2 16
#define TXD2 17
#define maxfeekbacktime 5000
#define floatlen 1
long temp , humid ;
double TempValue,HumidValue ;
byte cmd ;
byte receiveddata[250] ;
int receivedlen = 0 ;
byte StrTemp[] = {0x01,0x04,0x00,0x01,0x00,0x02,0x20,0x0B}  ;   //連續讀取溫濕度控制命令
byte Str1[] = {0x01,0x04,0x00,0x01,0x00,0x01,0x60,0x0A}  ;      //讀取溫度控制命令
byte Str2[] = {0x01,0x04,0x00,0x02,0x00,0x01,0x90,0x0A}  ;      //讀取濕度控制命令
int phasestage=1 ;    //控制讀取溫度、濕度、列印階段
boolean flag1 = false ;    //讀取成功溫度值控制旗標
boolean flag2 = false ;   //讀取成功濕度度值控制旗標


void initRS485()    //啟動Modbus 溫溼度感測器
{
    RS485Serial.begin(SERIAL_BAUD,SERIAL_8N1,RXD2,TXD2);  
  //初始化串口2

}

void requesttemperature()
{
    Serial.println("now send data to device") ;
    RS485Serial.write(Str1,8);  //  送出讀取溫度 str1 字串陣列 長度為8
     Serial.println("end sending") ;      
}
void requesthumidity()
{
    Serial.println("now send data to device") ;
    RS485Serial.write(Str2,8);    //送出讀取濕度str2 字串陣列 長度為8
     Serial.println("end sending") ;      
}

void requestdata()
{
    Serial.println("now send request to device") ;
    RS485Serial.write(StrTemp,8);   //  送出讀取溫濕度 StrTemp 字串陣列 長度為8

     Serial.println("end sending") ;      
}

int GetDHTdata(byte *dd)
{
  int count = 0 ;   //讀取陣列計數器
  long strtime= millis() ;    //計數器開始時間
  while ((millis() -strtime) < 2000)    //兩秒之內
    {
    if (RS485Serial.available()>0)    //有資料嗎
      {
        Serial.println("Controler Respones") ;    //印出有資料嗎
          while (RS485Serial.available()>0)   //仍有資料
          {
             RS485Serial.readBytes(&cmd,1) ;    // 讀取一個byte到cnd unsigned char or byte
             Serial.print(print2HEX((int)cmd)) ;  //印出剛才讀入資料內容，以16進位方式印出'
              *(dd+count) =cmd ;    //剛才讀入資料內容放入陣列
              count++ ;   //陣列計數器 加一

          }
          Serial.print("\n---------\n") ;
      }
      return count ;
    }
  
}
                       
