#include<EEPROM.h>
void setup()
{
  Serial.begin(9600);
  //int start=EEPROM.read(0);
  //Serial.println(start,DEC);
  EEPROM.write(0,0);
  for(int i=1;i<=50;i++)
  {
    EEPROM.write(i,'-');
    
  }
}
void loop(){}
