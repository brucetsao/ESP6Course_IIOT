//#include <HardwareSerial .h>
#include <String.h>
#define SERIAL_BAUD 9600
String DeviceAddress, RelayStatus,IOStatus;
long DeviceBaud;
HardwareSerial RS485Serial(2);//声明串口2

#define RXD2 16
#define TXD2 17
#define maxfeekbacktime 5000

byte cmd ;
byte receiveddata[250] ;
int receivedlen = 0 ;

byte cmd_address[] = {0x00,0x03,0x00,0x00,0x00,0x01,0x85,0xDB}  ;      //讀取裝置位址
byte cmd_baud[] = {0xFF,0x03,0x03,0xE8,0x00,0x01,0x11,0xA4 }  ;      //讀取裝置通訊速度
byte cmd_relay[] = {0xFF,0x01,0x00,0x00,0x00,0x08,0x28,0x12}  ;      //讀取裝置繼電器狀態
byte cmd_IO[] = {0xFF,0x02,0x00,0x00,0x00,0x08,0x6C,0x12 }  ;      //讀取裝置IO器狀態

int phasestage=1 ;    //控制讀取位址、速率、繼電器狀態、IO狀態階段
boolean flag1 = false ;    //讀取成功裝置位址控制旗標
boolean flag2 = false ;   //讀取成功裝置速率控制旗標
boolean flag3 = false ;    //讀取成功繼電器狀態控制旗標
boolean flag4 = false ;   //讀取成功IO狀態控制旗標

unsigned int readcrc;
uint8_t Hibyte, Lobyte ;

void initRS485()    //啟動Modbus 溫溼度感測器
{
    RS485Serial.begin(SERIAL_BAUD,SERIAL_8N1,RXD2,TXD2);  
  //初始化串口2

}

void requestaddress()   //讀取裝置位址
{
    Serial.println("now send Device Address data to device") ;
    RS485Serial.write(cmd_address,8);  //  送出讀取溫度 str1 字串陣列 長度為8
     Serial.println("end sending") ;      
}
void requestbaud()    //讀取裝置通訊速率
{
    Serial.println("now send Communication Baud data to device") ;
    RS485Serial.write(cmd_baud,8);    //送出讀取濕度str2 字串陣列 長度為8
     Serial.println("end sending") ;      
}

void requestrelay()     //讀取繼電器狀態
{
    Serial.println("now send Relay request to device") ;
    RS485Serial.write(cmd_relay,8);   //  送出讀取溫濕度 StrTemp 字串陣列 長度為8

     Serial.println("end sending") ;      
}

void requestIO()    //讀取光偶合狀態
{
    Serial.println("now send IO request to device") ;
    RS485Serial.write(cmd_IO,8);   //  送出讀取溫濕度 StrTemp 字串陣列 長度為8

     Serial.println("end sending") ;      
}

int GetRS485data(byte *dd)
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
             Serial.print(count) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(":") ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print("(") ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(print2HEX((int)cmd)) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print("/") ;  //印出剛才讀入資料內容，以16進位方式印出'
              *(dd+count) =cmd ;    //剛才讀入資料內容放入陣列
             Serial.print(print2HEX((int)*(dd+count))) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(")\n") ;  //印出剛才讀入資料內容，以16進位方式印出'
              count++ ;   //陣列計數器 加一

          }
          Serial.print("\n---------\n") ;
      }
      return count ;
    }
  
}
String RS485data2String(byte *dd, int ln)
{
    String tmp ;
    for(int i=0 ; i<ln; i++)
      {
        tmp= tmp +print2HEX((int)*(dd+i)) ;
      } 
    return tmp;
}

long TranBaud(int bb)
{
    switch (bb)
    {
      case 2:
        return 4800;
        break;
      case 3:
        return 9600;
        break;
      case 4:
        return 19200;
        break;
      default:
        return 9600;
        break;
    }
}

String byte2bitstring(int dd)
{
  String tmp;
  int vv=dd;
  int k=0;
  for(int i=0; i <8;i++)    // loop 8 times fot modal values for bit
    {
        k=(dd % 2) ;
        dd = (int)(dd/2) ;
        if (k == 1)
          {
              tmp = "1"+tmp ;
          }
          else
          {
              tmp = "0"+tmp ;
          }
          
    }
  
  return tmp ;
}
                      
