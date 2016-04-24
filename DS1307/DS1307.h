#ifndef DS1307_h
#define DS1307_h

#include "Arduino.h"

#define START 0x08
#define REP_START 0x10
#define T_SLA_ACK 0x18
#define MT_SLA_ACK 0x40
class DS1307{

private:
int tmp;
//boolean military,ampm;

//function declarations
public:
void initDS1307();
void setSeconds(int);
void setMinutes(int);
void setMilitary(boolean);
void setAMPM(boolean);
void setHours(int);
void setDay(int);
void setDate(int);
void setMonth(int);
void setYear(int);

int getYear();
int getMonth();
int getDate();
int getHours();
int getMinutes();
int getSeconds();
String getAMPM();
int getDay();

//internal for I2C
private:
void error(int);
int getReg(int);
void setReg(int,int);

};

extern DS1307 DS1307module;
#endif


