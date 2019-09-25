//gsm connected to seril3
//init gsm communication



#include <dht.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include "constants.h"

dht DHT;
Servo servo;
NewPing range2(pinUSonic::echo1, pinUSonic::trig1,MAX);
NewPing range1(pinUSonic::echo2, pinUSonic::trig2,MAX);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcdPrint("   SMART BIN    ","BOOTING UP . . .");
  Serial.begin(9600);
  servo.attach(pinServo);
  pinMode(46,INPUT);
  pinMode(47,INPUT);
  pinMode(48,INPUT);
  pinMode(49,INPUT);

}

void loop() {
  DHT.read11(pinDht);
  if(DHT.temperature>0 && DHT.humidity>0){
    temp = DHT.temperature;
    humid = DHT.humidity;
  }
  bin1 = map(range1.ping_cm(),10,45,100,0);
  delay(100);
  bin2 = map(range2.ping_cm(),10,45,100,0);
  sprintf(strTemp1,"  %3d%s   %3d%s   ",bin1,"%", bin2,"%");
  sprintf(strTemp2,"   %2dc    %2d%s   ",temp, humid,"%");
  lcdPrint(strTemp1, strTemp2);
  Serial.println(String(temp) + "\t" + String(humid));
  while(!digitalRead(ir1) || !digitalRead(ir2)){
    bio = 0;
    for(int i=0; i<=BIOSAMPLES; i++){
      if(isBio()){
        bio--;
      }else{
        bio++;
      }
      delay(250);
    }
    if(bio<=-BIOSAMPLES||bio>=BIOSAMPLES){
      if(bio<=-BIOSAMPLES){
        lcdPrint("Garbage detected", "NonBiodegradable");
        Serial.println("Non Biodegradable");
        servo.write(plate::nonbio-35);
        while(!digitalRead(ir1)||!digitalRead(ir2));
        delay(2000);
        servo.write(plate::nonbio);
        delay(1000);
        servo.write(plate::flat);
      }else if(bio>=BIOSAMPLES){
        lcdPrint("Garbage detected", "Biodegradable");
        Serial.println("Biodegradable");
        servo.write(plate::bio+35);
        while(!digitalRead(ir1)||!digitalRead(ir2));
        delay(2000);
        servo.write(plate::bio);
        delay(1000);
        servo.write(plate::flat);
      }
      bio = 0;
    } 
  }
  
  delay(250);

}

void lcdPrint(char * str1, char * str2) {
  lcd.setCursor(0,0);
  lcd.print(str1);
  lcd.setCursor(0,1);
  lcd.print(str2);
}


bool isBio(){
  if(digitalRead(46)||digitalRead(47)||!digitalRead(48)||!digitalRead(49)) return true;
  else return false;
}
