#include <dht.h>
#include <Servo.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#define MAX 200
#define BIOSAMPLES 3

dht DHT;
Servo servo;
NewPing range(36,37,MAX);
NewPing range1(38,39,MAX);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int range_cm, range1_cm, bio = 0;
int temp, humid;
bool cap, ind;
char str[32];

void setup() {
  Serial.begin(9600);
//  dht.begin();
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  servo.attach(2);

}

void loop() {
  DHT.read11(41);
  if(DHT.temperature>0 && DHT.humidity>0){
    temp = DHT.temperature;
    humid = DHT.humidity;
  }
  range_cm = 200-range.ping_cm();
  range1_cm = 200-range1.ping_cm();
  cap = !digitalRead(6);
  ind = digitalRead(5);
  sprintf(str,"%d %d %d %d",range_cm, range1_cm, cap, ind);
//  Serial.println(String(temp) + "\t" + String(humid));
  Serial.println(str);
  lcdPrint();
  servo.write(90);
  while(!digitalRead(40)){
    bio = 0;
    for(int i=0; i<=BIOSAMPLES; i++){
      cap = !digitalRead(6);
      ind = digitalRead(5);
      if(cap||ind){
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
        servo.write(150);
        while(!digitalRead(40)) {}
        delay(2000);
        servo.write(90);
      }else if(bio>=BIOSAMPLES){
        lcdPrint("Garbage detected", "Biodegradable");
        Serial.println("Biodegradable");
        servo.write(30);
        while(!digitalRead(40)) {}
        delay(2000);
        servo.write(90);
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

void lcdPrint(){
  lcd.setCursor(0,0);
  sprintf(str,"%3d%s %3d%s        ",range_cm, "%", range1_cm, "%");
  lcd.print(str);
  lcd.setCursor(0,1);
  sprintf(str,"%3dc %3d%s        ", temp, humid,"%");
  lcd.print(str);
}
