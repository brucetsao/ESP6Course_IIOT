#include <String.h>   //String 使用必備函示庫
#include "initPins.h"
#include "RS485Lib.h"
#include "crc16.h"


//--------------------

 void initAll() ;    //系統初始化
//----------------

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化
  Serial.println("System  Ready");
 //-----------------
     phasestage=1 ;
    flag1 = false ;
    flag2 = false ;
}

// the loop function runs over and over again forever
void loop() 
{
  if (phasestage == 1 && !flag1 ) //讀取溫度階段，且未讀取成功溫度值
    {
        requesttemperature() ;    //要求讀取溫度階段
    }
  if (phasestage == 2  && !flag2) //讀取濕度階段，且未讀取成功濕度值
    {
        requesthumidity() ;   //要求讀取濕度階段
    }
  if (phasestage == 3 && flag1 && flag2)    //已完成讀取溫度與濕度
    {
        Serial.print("Temperature:(");    
        Serial.print(TempValue);
        Serial.print(")\n");    //印出溫度
        Serial.print("Humidity:(");
        Serial.print(HumidValue);
        Serial.print(")\n");     //印出濕度
        phasestage=1 ;
        flag1 = false ;
        flag2 = false ;
        delay(loopdelay) ;
        return ;
    }

    delay(200);
    receivedlen = GetDHTdata(receiveddata) ;
    if (receivedlen >2)
      {
              Serial.print("Data Len:") ;
              Serial.print(receivedlen) ;
              Serial.print("\n") ;
              Serial.print("CRC:") ;
              Serial.print(ModbusCRC16(receiveddata,receivedlen-2)) ;
              Serial.print("\n") ;
              for (int i = 0 ; i <receivedlen ; i++)
                {
                  Serial.print(receiveddata[i],HEX) ;
                  Serial.print("/") ;
                }
                  Serial.print("...\n") ;
              Serial.print("CRC Byte:") ;
              Serial.print(receiveddata[receivedlen-1],HEX) ;
              Serial.print("/") ;
              Serial.print(receiveddata[receivedlen-2],HEX) ;
              Serial.print("\n") ;
          if (CompareCRC16(ModbusCRC16(receiveddata,receivedlen-2),receiveddata[receivedlen-1],receiveddata[receivedlen-2]))
            {
                if (phasestage == 1)
                {
                    temp = receiveddata[3]*256+receiveddata[4] ;
                    TempValue = (double)temp / (10^ floatlen) ;
                    flag1 = true ;
                    phasestage=2 ;
                    return ;
                }
                if (phasestage == 2)
                {
                    humid = receiveddata[3]*256+receiveddata[4] ;
                     HumidValue = (double)humid / (10^ floatlen) ;
                    flag2 = true ;
                    phasestage=3 ;
                    return ;
                }

            }
      }
    /*
    if (Serial2.available()>0)
      {
        Serial.println("Controler Respones") ;
          while (Serial2.available()>0)
          {
             Serial2.readBytes(&cmd,1) ;
             Serial.print(print2HEX((int)cmd)) ;

          }
          Serial.print("\n---------\n") ;
      }
      */

  delay(loopdelay) ;
}

void initAll()     //系統初始化
{
    Serial.begin(9600) ;
    Serial.println("System Start");
    
   MacData = GetMacAddress() ; //取得網路卡編號

      initWiFi() ;  //網路連線，連上熱點
   ShowInternet() ;    //秀出網路卡編號
    
     initRS485() ;   //啟動HTU21D 溫溼度感測器
 
}
