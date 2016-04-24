#include<EEPROM.h>
void setup()
{
  Serial.begin(9600);
  int start=EEPROM.read(0);
  Serial.println(start,DEC);
  for(int i=1;i<=start;i++)
  {
    Serial.print(i,DEC);
    Serial.print("=");
    Serial.print((char)EEPROM.read(i));
    Serial.println();
    
  }
}
void loop(){}
