#include <DS1307.h>

DS1307 ds1;
void setup(){
  Serial.begin(9600);
  ds1.initDS1307();
  ds1.setSeconds(10);
  ds1.setMinutes(10);
  ds1.setMilitary(false); //set military time
  ds1.setAMPM(true); //1 is PM
  ds1.setHours(10);
  ds1.setDay(3);
  ds1.setDate(20);
  ds1.setMonth(11);
  ds1.setYear(16);
  }

void loop(){
  Serial.print(ds1.getYear());
  Serial.print("/");
  Serial.print(ds1.getMonth());
  Serial.print("/");
  Serial.print(ds1.getDate());
  Serial.print(" ");
  Serial.print(ds1.getHours());
  Serial.print(":");
  Serial.print(ds1.getMinutes());
  Serial.print(":");
  Serial.print(ds1.getSeconds());
  Serial.println(ds1.getAMPM());
  Serial.print("day: ");
  Serial.println(ds1.getDay());
  delay(5000);
  }
