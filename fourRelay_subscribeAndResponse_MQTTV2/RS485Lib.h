// 包含String函式庫
#include <String.h>

// 設定序列通訊速率為9600
#define SERIAL_BAUD 9600

// 宣告三個字串型態的變數
String DeviceAddress, RelayStatus, IOStatus;

// 宣告一個長整數型態的變數 DeviceBaud
long DeviceBaud;

// 宣告一個硬體序列通訊的物件RS485Serial，設定其使用第二個UART通訊
HardwareSerial RS485Serial(2);

// 定義第二個UART的RX腳位為16
#define RXD2 16

// 定義第二個UART的TX腳位為17
#define TXD2 17

// 定義最大反饋時間為5000毫秒
#define maxfeekbacktime 5000

// 包含必要的函數庫和定義常數
byte cmd ; // 命令字元
byte receiveddata[250] ; // 接收到的資料陣列
int receivedlen = 0 ; // 接收到的資料長度

// 不同的命令
byte cmd_address[] = {0x00,0x03,0x00,0x00,0x00,0x01,0x85,0xDB}  ;      //讀取裝置位址
byte cmd_baud[] = {0xFF,0x03,0x03,0xE8,0x00,0x01,0x11,0xA4 }  ;      //讀取裝置通訊速度
byte cmd_relay[] = {0xFF,0x01,0x00,0x00,0x00,0x08,0x28,0x12}  ;      //讀取裝置繼電器狀態
byte cmd_IO[] = {0xFF,0x02,0x00,0x00,0x00,0x08,0x6C,0x12 }  ;      //讀取裝置IO器狀態

int phasestage=1 ;    //控制讀取位址、速率、繼電器狀態、IO狀態階段
boolean flag1 = false ;    //第一階段讀取成功裝置位址控制旗標
boolean flag2 = false ;   //第二階段讀取成功裝置速率控制旗標
boolean flag3 = false ;    //第三階段讀取成功繼電器狀態控制旗標
boolean flag4 = false ;   //第四階段讀取成功IO狀態控制旗標

unsigned int readcrc;   //crc16計算值暫存變數
uint8_t Hibyte, Lobyte ;    //crc16計算值 高位元與低位元變數

//這段程式是一個用來控制Modbus溫溼度感測器的程式
/*
 其中，initRS485()用來初始化串口，
 requestaddress()、
 requestbaud()和
 requestrelay()
 則是分別用來向溫溼度感測器發送讀取裝置位址、
 通訊速率和繼電器狀態的命令。
 每個函數都會將命令發送出去，
 並印出相應的訊息，
 以便在調試時確定程式是否正常運行。
 */
void initRS485()    //啟動Modbus 溫溼度感測器
{
    RS485Serial.begin(SERIAL_BAUD,SERIAL_8N1,RXD2,TXD2);  
    /* 
     初始化串口2，
     設定通訊速率為SERIAL_BAUD(9600)
     數據位為8、
     無校驗位、
     停止位為1，
     RX腳位為RXD2，
     TX腳位為TXD2 
     */
}

void requestaddress()   //讀取裝置位址
{
    Serial.println("now send Device Address data to device") ;
    RS485Serial.write(cmd_address,8);  //  送出讀取溫度 str1 字串陣列 長度為8
    //  送出讀取裝置位址的命令，cmd_address是一個byte陣列，長度為8
     Serial.println("end sending") ;      
}
void requestbaud()    //讀取裝置通訊速率
{
    Serial.println("now send Communication Baud data to device") ;
    RS485Serial.write(cmd_baud,8);    
    //  送出讀取裝置通訊速率的命令，cmd_baud是一個byte陣列，長度為8
     Serial.println("end sending") ;      
}

void requestrelay()     //讀取繼電器狀態
{
    Serial.println("now send Relay request to device") ;
    RS485Serial.write(cmd_relay,8);   
    //  送出讀取繼電器狀態的命令，cmd_relay是一個byte陣列，長度為8
     Serial.println("end sending") ;      
}

void requestIO()    //讀取光偶合狀態
{
    Serial.println("now send IO request to device") ;
    RS485Serial.write(cmd_IO,8);   //  送出讀取光偶合狀態 cmd_IO 字串陣列 長度為8
     Serial.println("end sending") ;      
}

int GetRS485data(byte *dd)    
{
  //這是一個函式，接收 RS485 通訊模組傳回的資料，並將其存儲在指定的陣列中。
  int count = 0 ;  // 計數器，用於追蹤已讀取的資料數量
  long strtime= millis() ; // 計數器開始時間
  while ((millis() -strtime) < 2000) // 在兩秒內讀取資料
    {
    if (RS485Serial.available()>0)    // 檢查通訊模組是否有資料傳回
      {
        Serial.println("Controler Respones") ;    //印出有資料嗎
          while (RS485Serial.available()>0)   // 只要還有資料，就持續讀取
          {
             RS485Serial.readBytes(&cmd,1) ;    /// 從 RS485 模組讀取一個 byte 資料，儲存到變數 cmd 中
             Serial.print(count) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(":") ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print("(") ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(print2HEX((int)cmd)) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print("/") ;  //印出剛才讀入資料內容，以16進位方式印出'
              *(dd+count) =cmd ;     // 將讀取到的資料存儲到指定的陣列中
             Serial.print(print2HEX((int)*(dd+count))) ;  //印出剛才讀入資料內容，以16進位方式印出'
             Serial.print(")\n") ;  //印出剛才讀入資料內容，以16進位方式印出'
              count++ ;   // 增加計數器

          }
          Serial.print("\n---------\n") ;
      }
      return count ;  // 返回讀取到的資料量
    }
  
}
String RS485data2String(byte *dd, int ln)   
{// 將 byte 陣列轉換成字串型態，每個 byte 會以 16 進位方式表示
    String tmp ;
    for(int i=0 ; i<ln; i++)
      {
        tmp= tmp +print2HEX((int)*(dd+i)) ;
      } 
    return tmp;
}

long TranBaud(int bb)
{
  // 取得通訊速率的值
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
{//// 將一個 byte 轉換成 8 個 bit 的字串，其中每個 bit 以 0 或 1 表示
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
                      
