#include <String.h>

#define RXD2 16
#define TXD2 17



char Oledchar[30] ;
//char* AP2 = "Brucetsao" ;
//char* PW2 = "12345678";

char* AP2 = "R1-1117" ;
char* PW2 = "3716037160";


#define maxfeekbacktime 5000
long temp , humid ;
byte cmd ;
byte receiveddata[250] ;
int receivedlen = 0 ;
byte StrTemp[] = {0x01,0x04,0x00,0x01,0x00,0x02,0x20,0x0B}  ;
byte Str1[] = {0x01,0x04,0x00,0x01,0x00,0x01,0x60,0x0A}  ;
byte Str2[] = {0x01,0x04,0x00,0x02,0x00,0x01,0x90,0x0A}  ;
int phasestage=1 ;
boolean flag1 = false ;
boolean flag2 = false ;

 
