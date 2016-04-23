#include "Arduino.h"
#include "DS1307.h"
 //-----------------------functions for DS1307 module----------------------

  //this is the initialization for the Module
  void DS1307::initDS1307(){
    digitalWrite(SDA,1);
    digitalWrite(SCL,1);
    TWBR=0x80;
    tmp=getReg(0);
    tmp&=~0x80;
    setReg(0,tmp);
    }

    //this sets the seconds register
    void DS1307::setSeconds(int s){

      if(s>59)error(1);
      int sectens=s/10;
      int secones=s-10*sectens;
      tmp=(sectens<<4)+secones;
      setReg(0,tmp);
      }

     //this gets the seconds register
     int DS1307::getSeconds(){
      tmp=getReg(0);
      int sectens=tmp>>4;
      int secones=tmp&0x0F;
      int seconds=10*sectens+secones;
      return seconds;
      }

      //this sets the minute register
       void DS1307::setMinutes(int m){

      if(m>59)error(1);
      int mintens=m/10;
      int minones=m-10*mintens;
      tmp=(mintens<<4)+minones;
      setReg(1,tmp);
      }

     //this gets the minute register
     int DS1307::getMinutes(){
      tmp=getReg(1);
      int mintens=tmp>>4;
      int minones=tmp&0x0F;
      int minutes=10*mintens+minones;
      return minutes;
      }

        //this function sets or clears military time mode
  void DS1307::setMilitary(boolean mil){
    military=mil;
    if(mil){
      //clear military bit
      tmp=getReg(2);
      tmp&=~0x40;
      setReg(2,tmp);
      }
      else{
        //set military bit
        tmp=getReg(2);
        tmp|=0x40;
        setReg(2,tmp);
        }
    }

    //this sets the hours, knowing ampm and military time bits
    void DS1307::setHours(int hr){ //fix hours setting
       
        int hrTens=hr/10;
        int hrOnes=hr-10*hrTens;
        tmp=hrOnes+(hrTens<<4);

        if(military){
          if(hr<0 || hr>23){
            Serial.println("hour out of range.");
            while(1){}
            }
          tmp&=~0x40; //clear military time bit
          }
          else {
            if(hr<0 || hr>23){
              Serial.println("hour out of range.");
              while(1){}
              }
            tmp+=(ampm<<6);}
          setReg(2,tmp);
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
        tmp=(dateTens<<4)+dateOnes;
        setReg(4,tmp);
        }

        //set the month
         void DS1307::setMonth(int month){
            int monthTens=month/10;
        int monthOnes=month-10*monthTens;
        tmp=(monthTens<<4)+monthOnes;
        setReg(5,tmp);
          }

          //set the year
         void DS1307::setYear(int yr){
            int yearTens=yr/10;
        int yearOnes=yr-10*yearTens;
        tmp=(yearTens<<4)+yearOnes;
        setReg(6,tmp);
          }

         //get the year
         int DS1307::getYear(){
          tmp=getReg(6);
          int yearTens=tmp>>4;
          int yearOnes=tmp&0x0F;
          return 10*yearTens+yearOnes;
          }

          //get the month
          int DS1307::getMonth(){
          tmp=getReg(5);
          int monthTens=(tmp&0x10)>>4;
          int monthOnes=(tmp&0x0F);
          return 10*monthTens+monthOnes;
          }

          //get the date
            int DS1307::getDate(){
          tmp=getReg(4);
          int dateTens=(tmp&0x30)>>4;
          int dateOnes=(tmp&0x0F);
          return 10*dateTens+dateOnes;
          }

          //get day
          int DS1307::getDay(){
            tmp=getReg(3);
            return tmp;
            }

            //get hours
            int DS1307::getHours(){
              tmp=getReg(2);
              int hrTens,hrOnes;
              if(tmp&0x40==0x40){
                //12 hour
                military=false;
                ampm=(tmp&0x20)>>6;

                tmp&=~0x60; //clear tmp ampm and 12/24 hr stuff
                hrTens=(tmp&0x10)>>4;
                hrOnes=(tmp&0x0F);
                return hrTens*10+hrOnes;
                }
                else{
                  //24 hour
                  military=true;
                  tmp&=~0x40;
                      hrTens=(tmp&0x30)>>4;
                hrOnes=(tmp&0x0F);
                return hrTens*10+hrOnes;
                  }
              }

              //this gets the ampm status
              String DS1307::getAMPM(){
                tmp=getReg(2);
                if(tmp&0x20==0x20)
                return "PM";
                else return "AM";
                }

                //this sets the AMPM status
                void DS1307::setAMPM(boolean ampm){
                  tmp=getReg(2);
                  if(ampm) tmp|=0x20;
                  else tmp&=~0x20;
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

tmp=TWDR;

//send stop
TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

return tmp;
  }

  //this is an error function
  void DS1307::error(int code){
   Serial.print("error, code is:");
   Serial.println(code,HEX);
    while(1){}
}