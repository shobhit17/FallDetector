    #include<SoftwareSerial.h>
    #include <EEPROM.h>
    SoftwareSerial GPRS(9, 10);  
    char buffer[256]; // buffer array for data recieve over serial port  
    int count = 0; // counter for buffer array
    const int groundpin = 18;             // analog input pin 4 -- ground
    const int powerpin = 19;              // analog input pin 5 -- voltage
    const int xpin = A3;                  // x-axis of the accelerometer
    const int ypin = A2;                  // y-axis
    const int zpin = A1;   
    void setup()  
    {  
        GPRS.begin(9600); // the GPRS baud rate   
        Serial.begin(9600); // the Serial port of Arduino baud rate.
        pinMode(groundpin, OUTPUT);
        pinMode(powerpin, OUTPUT);
        digitalWrite(groundpin, LOW); 
        digitalWrite(powerpin, HIGH);
        pinMode(3, OUTPUT);
        pinMode(2, OUTPUT);
        digitalWrite(11, LOW);   // Setting the baud rate of Serial Monitor (Arduino)
        delay(100);   
    }
   int loopNo=0,flag2=0;
   float xg2,yg2,zg2;
    
   int StringContains(String s, String search) {
      int max = s.length() - search.length();
      int lgsearch = search.length();

      for (int i = 0; i <= max; i++) {
        if (s.substring(i, i + lgsearch) == search) return i;
      }

     return -1;
    }
    
      
   void writeno(String no){
     int start=EEPROM.read(0);
     for(int i=0;i<no.length();i++)
      {
        char a=no.charAt(i);
        EEPROM.write(start+i+1,a);
      }    
    
     EEPROM.write(0,start+no.length()); 
  }
  
  void printROM(){
      for(int j=1;j<=EEPROM.read(0);j++)
          Serial.println((char)(EEPROM.read(j)));
    }
    
    
  void deleteno(String no)
    {
      int start=EEPROM.read(0);
      String romData="";  
      for(int y=1;y<=start;y++)
       {
         romData+=((String)((char)EEPROM.read(y)));       
       }
       Serial.print("ROM Data was ");
       Serial.print(romData);
       Serial.println();
       int noIndex = StringContains(romData,no);
       ++noIndex;
       Serial.println("noIndex= ");
       Serial.println(noIndex,DEC);
       for(int v=noIndex+14;v<=start+1;v++)
       {
         EEPROM.write((v-14),EEPROM.read(v-1));
       }
       EEPROM.write(0,start-13);
       Serial.print("ROM read(0)=");
       Serial.print(EEPROM.read(0));
       Serial.println();
    }
  
  void SendMessage(String number)
    {
      Serial.print("Inside Send Message");
      GPRS.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      GPRS.println("AT+CMGS=\""+number+"\"\r"); // Replace x with mobile number
      delay(1000);
      GPRS.println("emergency 23.17 80.02");// The SMS text you want to send
      delay(100);
      GPRS.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
    }  
    
   void loop()  
    {  
     
      if (GPRS.available()) // if date is comming from softwareserial port ==> data is comming from gprs shield  
        {  
            while (GPRS.available()) // reading data into char array   
            {  
                buffer[count++] = GPRS.read(); // writing data into array  
                if (count == 256) break;  
            }  
            String msg(buffer);
            
            Serial.println(msg);
            int addIndex=StringContains(msg,"Add");
            int removeIndex=StringContains(msg,"Remove");
            int emergencyIndex=StringContains(msg,"EMERGENCY");
            if(addIndex!=-1)
             {
               char no[15];
               for(int i=addIndex+4;i<addIndex+17;i++)
                no[i-(addIndex+4)]=buffer[i];
               String number(no);
               number=number.substring(0,13);
               Serial.println(number); 
               writeno(number);
               printROM();
             }
            
            else if(removeIndex!=-1)
               {
                  char no[15];
                   for(int j=removeIndex+7;j<removeIndex+20;j++)
                    {
                      no[j-(removeIndex+7)]=buffer[j];
                    }
                   String number(no);
                   number=number.substring(0,13);
                   Serial.print("Deleted Number is ");
                   Serial.print(number);
                   Serial.println(); 
                   deleteno(number);
                   printROM();               
               }
           else if(emergencyIndex!=-1)
           {
               flag2++;
               if(flag2==1){ 
                   Serial.println("Inside Emergency");
                    int start=EEPROM.read(0);
                    int cnt=(start/13);
                    Serial.println("Cnt =");
                    Serial.println(cnt,DEC);
                    for(int i=0;i<cnt;i++)
                    {
                      String num="";
                      for(int j=1;j<=13;j++)
                      {
                         num+=((String)((char)EEPROM.read((i*13)+j)));           
                      }
                      Serial.println("Message Send to "+num);  
                      SendMessage(num);
                    }
               }
           }
            clearBufferArray(); // call clearBufferArray function to clear the storaged data from the array  
            count = 0; // set counter of while loop to zero  
        }
      float xg = analogRead(xpin);

      float yg = analogRead(ypin);

      float zg = analogRead(zpin);
  
      
      int flag=0;
          if(loopNo>0)
          {
    
              float diffx=xg2-xg;
              float diffy=yg2-yg;
              float diffz=zg2-zg;
              if(diffx<0)
               diffx=diffx*-1;
              if(diffy<0)
               diffy=diffy*-1;
              if(diffz<0)
               diffz=diffz*-1;
              //Serial.print("DiffSum=");
              float diffSum=diffx+diffy+diffz;
            //  Serial.println(diffSum);
              //Serial.println();
    
              if(diffSum>250||diffSum<-250)
              {
                digitalWrite(3, HIGH);
                digitalWrite(2, LOW);
                delay(6000);
                flag2++;
               if(flag2==1){ 
                   Serial.println("Inside Emergency");
                    int start=EEPROM.read(0);
                    int cnt=(start/13);
                    Serial.println("Cnt =");
                    Serial.println(cnt,DEC);
                    for(int i=0;i<cnt;i++)
                    {
                      String num="";
                      for(int j=1;j<=13;j++)
                      {
                         num+=((String)((char)EEPROM.read((i*13)+j)));           
                      }
                      Serial.println("Message Send to "+num);  
                      SendMessage(num);
                    }
               }
                
              }
         }
         xg2=xg;
         yg2=yg;
         zg2=zg;  
         delay(5);//Delay Before next reading
         loopNo=1;  
    
    }  
    
    
    void clearBufferArray() // function to clear buffer array  
        {  
            for (int i = 0; i < count; i++)  
            {  
                buffer[i] = NULL;  
            } // clear all index of array with command NULL  
        }  
