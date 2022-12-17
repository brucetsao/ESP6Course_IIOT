#include <String.h>

#define RXD2 16
#define TXD2 17



char Oledchar[30] ;
//char* AP2 = "Brucetsao" ;
//char* PW2 = "12345678";

char* AP2 = "R1-1117" ;
char* PW2 = "3716037160";


#define maxfeekbacktime 5000

byte cmd ;

byte StrTemp[] = {0x01,0x04,0x00,0x01,0x00,0x01,0x60,0x0a}  ;


 
