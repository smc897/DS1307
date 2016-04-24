#include "Arduino.h"
#include "DS1307.h"
 //-----------------------functions for DS1307 module----------------------

  //this is the initialization for the Module
  void DS1307::initDS1307(){
    digitalWrite(SDA,1);
    digitalWrite(SCL,1);
    TWBR=0x80;
    int inittmp=getReg(0);
    inittmp&=~0x80;
    setReg(0,inittmp);
    }

    //this sets the seconds register
    void DS1307::setSeconds(int s){

      if(s>59)error(1);
      int sectens=s/10;
      int secones=s-10*sectens;
      int sectmp=(sectens<<4)+secones;
      setReg(0,sectmp);
      }

     //this gets the seconds register
     int DS1307::getSeconds(){
      int sectmp=getReg(0);
      int sectens=sectmp>>4;
      int secones=sectmp&0x0F;
      int seconds=10*sectens+secones;
      return seconds;
      }

      //this sets the minute register
       void DS1307::setMinutes(int m){

      if(m>59)error(1);
      int mintens=m/10;
      int minones=m-10*mintens;
      int mintmp=(mintens<<4)+minones;
      setReg(1,mintmp);
      }

     //this gets the minute register
     int DS1307::getMinutes(){
      int minutetmp=getReg(1);
      int mintens=minutetmp>>4;
      int minones=minutetmp&0x0F;
      int minutes=10*mintens+minones;
      return minutes;
      }

        //this function sets or clears military time mode
  void DS1307::setMilitary(boolean mil){
    //military=mil;
    int tmpmil;
    if(mil){
      //clear military bit
      tmpmil=getReg(2);
      tmpmil&=~0x40;
      setReg(2,tmpmil);
      }
      else{
        //set military bit
       // Serial.println("in 12 hour mode");
        tmpmil=getReg(2);
        tmpmil|=0x40;
        setReg(2,tmpmil);
        }
        
    }

    //this sets the hours
    void DS1307::setHours(int hr){ //fix hours setting
        int hrTens=hr/10;
        int hrOnes=hr-10*hrTens;
        int tmphr=hrOnes+(hrTens<<4);

        char tmp1=getReg(2);
        if((tmp1&0x40)==0){ //military
     //   Serial.println("in military mode");
          if(hr<0 || hr>23){
            Serial.println("hour out of range.");
            while(1){}
            }
          }
          else {
              
                  //set military time bit
            tmphr|=0x40;
            
            if(hr<0 || hr>12){
              Serial.println("hour out of range.");
              while(1){}
              }
              
              if((tmp1&0x20)>0) {
              //    Serial.println("setting AMPM to PM, tmp1 is: ");
              //    Serial.println(tmp1,HEX);
                  tmphr|=0x20;
              }
              else {
                 //      Serial.println("setting AMPM to AM, tmp1 is: ");
                 // Serial.println(tmp1,HEX);
                  tmphr&=~0x20;
              }
            }
   
    //      Serial.print("tmphr: ");
    //      Serial.println(tmphr,HEX);
          setReg(2,tmphr);
      }

      //this sets the day
      void DS1307::setDay(int day){
        if(day<1 || day>7){
          Serial.println("day out of range.");
          while(1){}
          }
        setReg(3,day);
        }

       //this function sets the day of month
       void DS1307::setDate(int date){
        int dateTens=date/10;
        int dateOnes=date-10*dateTens;
        int daytmp=(dateTens<<4)+dateOnes;
        setReg(4,daytmp);
        }

        //set the month
         void DS1307::setMonth(int month){
            int monthTens=month/10;
        int monthOnes=month-10*monthTens;
        int monthtmp=(monthTens<<4)+monthOnes;
        setReg(5,monthtmp);
          }

          //set the year
         void DS1307::setYear(int yr){
            int yearTens=yr/10;
        int yearOnes=yr-10*yearTens;
        int yrtmp=(yearTens<<4)+yearOnes;
        setReg(6,yrtmp);
          }

         //get the year
         int DS1307::getYear(){
          int yrtmp=getReg(6);
          int yearTens=yrtmp>>4;
          int yearOnes=yrtmp&0x0F;
          return 10*yearTens+yearOnes;
          }

          //get the month
          int DS1307::getMonth(){
          int monthtmp=getReg(5);
          int monthTens=(monthtmp&0x10)>>4;
          int monthOnes=(monthtmp&0x0F);
          return 10*monthTens+monthOnes;
          }

          //get the date
            int DS1307::getDate(){
          int datetmp=getReg(4);
          int dateTens=(datetmp&0x30)>>4;
          int dateOnes=(datetmp&0x0F);
          return 10*dateTens+dateOnes;
          }

          //get day
          int DS1307::getDay(){
            int daytmp=getReg(3);
            return daytmp;
            }

            //get hours
            int DS1307::getHours(){
              char tmphr=getReg(2);
              
        //      Serial.print("in get hours, tmp is: ");
        //      Serial.println(tmp,HEX);
              int hrTens,hrOnes;
              if((tmphr&0x40)==0x40){
                //12 hour
             
                hrTens=(tmphr&0x10)>>4;
                hrOnes=(tmphr&0x0F);
                return hrTens*10+hrOnes;
                }
                else{
                  //24 hour
                //  military=true;
                  tmphr&=~0x40;
                      hrTens=(tmphr&0x30)>>4;
                hrOnes=(tmphr&0x0F);
                return hrTens*10+hrOnes;
                  }
              }

              //this gets the ampm status
              String DS1307::getAMPM(){
                char tmpampm=getReg(2);
                if((tmpampm&0x20)>0)
                return "PM";
                else return "AM";
                }

                //this sets the AMPM status
                void DS1307::setAMPM(boolean ampm){
                  int tmpampm=getReg(2);
                  if(ampm) tmpampm|=0x20;
                  else tmpampm&=~0x20;
          
                  setReg(2,tmpampm);
                  }
        

      //----------------------------------functions for I2C----------------------

      //this function sets the regaddr to val
      void DS1307::setReg(int regaddr,int val){
          TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){
  //Serial.println("waiting for TWINT to be set...");
  }
if((TWSR&0xF8)!=START && (TWSR&0xF8)!=REP_START)
error(TWSR&0xF8);
//else Serial.println("ack correct");


TWDR=0xD0;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
if((TWSR&0xF8)!=T_SLA_ACK)
error(TWSR&0xF8);

TWDR=regaddr;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
//Serial.println(TWDR);

TWDR=val;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
//Serial.println(TWDR);


//send stop
TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

//this function gets regaddr
int DS1307::getReg(int regaddr){

//set the address
     TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){
  //Serial.println("waiting for TWINT to be set...");
  }
if((TWSR&0xF8)!=START && (TWSR&0xF8)!=REP_START)
error(TWSR&0xF8);
//else Serial.println("ack correct");


TWDR=0xD0;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
if((TWSR&0xF8)!=T_SLA_ACK)
error(TWSR&0xF8);

TWDR=regaddr;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
//Serial.println(TWDR);

//send stop
TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

//now, read from the address
 //read from ds1307
TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){
  //Serial.println("waiting for TWINT to be set...");
  }
if((TWSR&0xF8)!=START && (TWSR&0xF8)!=REP_START)
error(TWSR&0xF8);
//else Serial.println("ack correct");


TWDR=0xD1;
TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}
if((TWSR&0xF8)!=MT_SLA_ACK)
error(TWSR&0xF8);


TWCR=(1<<TWINT)|(1<<TWEN);
while(!(TWCR&(1<<TWINT))){}

int i2ctmp=TWDR;

//send stop
TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

return i2ctmp;
  }

  //this is an error function
  void DS1307::error(int code){
   Serial.print("error, code is:");
   Serial.println(code,HEX);
    while(1){}
}