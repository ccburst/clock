#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include <virtuabotixRTC.h>
//button0
#define lock 2
//button1
#define add_min 3
//button2
#define add_hour 4
virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);
uint8_t set_time[2]={0,0};
int tempmin;
//mode(HIGH for display time , LOW for set alarm)
int state=LOW;
int once=LOW;
int a;

void setup() {
//set time
  pinMode(add_hour,INPUT);
  pinMode(A0,INPUT);
  pinMode(lock,INPUT);
  pinMode(9,OUTPUT);
  pinMode(10,INPUT);
  pinMode(add_min,INPUT);
  lcd.begin(16,2);
  lcd.backlight();
  delay(1000);
  lcd.noBacklight();
  delay(1000);
  lcd.backlight();
  delay(1000);
}
void loop() {
  if(digitalRead(lock)==0){
    displayTime();
  }
  else{ 
    delay(500);
    setAlarm();
  }
} 
 
void setAlarm(){
//set up alarm
while(digitalRead(lock)==0){
  if(digitalRead(add_hour)==1){
    set_time[0]++;
  }
  if(digitalRead(add_min)==1){
   set_time[1]++;
  }
    //carry in or no
  if(set_time[0]>=24){
   set_time[0]=set_time[0]-24;
  }
  if(set_time[1]>=60){
    set_time[0]++;
    set_time[1]=set_time[1]-60;
  }
//output time
  lcd.setCursor(0,1);
  lcd.print(set_time[0]);
  lcd.print(":");
  lcd.print(set_time[1]);
  delay(250);
  }
  delay(250);
  lcd.clear();
}
 
void displayTime(){
//ouput time
  myRTC.updateTime();
  lcd.setCursor(0,0);
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);
  delay(500);
  lcd.clear();
  //check the time whether reach the alarm time or not.
  if(set_time[0]==myRTC.hours && set_time[1]==myRTC.minutes && state==LOW && once==LOW && analogRead(A0)<900){
    state=HIGH;
  }
  if(digitalRead(10)==HIGH&&state==HIGH){
    once=HIGH;
    state=LOW;
    tempmin=myRTC.minutes;
  }
  digitalWrite(9,state);
  if(tempmin!=myRTC.minutes){
    if(state==HIGH){
      state=LOW;
    }
    once=LOW;
  }  
}
