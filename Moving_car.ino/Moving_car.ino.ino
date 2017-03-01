#include<SoftwareSerial.h>
int bluetoothTx = 2;
int bluetoothRx = 3;
int motor2 = 10;
int motor1 = 11;
int motor4 = 7;
int motor3 = 5;
int enable1 = 9;
int enable2 = 6;
int TriggerPin = 12;
int EchoPin = 13;
char msg;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);//Arduino RX,Tx

int counter = 0;
int Speed = 155;
void setup() {
pinMode(motor1,OUTPUT);
pinMode(motor2,OUTPUT);
pinMode(motor3,OUTPUT);
pinMode(motor4,OUTPUT);
pinMode(enable1,OUTPUT);
pinMode(enable2,OUTPUT);
  Serial.begin(9600);
  pinMode(EchoPin,OUTPUT);
  digitalWrite(EchoPin,LOW);
  bluetooth.begin(115200);//standard bluetooth mate

pinMode(TriggerPin, OUTPUT);
pinMode(EchoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bluetooth.listen();
  modtagFraBluetooth();
  bluetooth.print(msg);
  bluetooth.println(Speed);
  
  analogWrite(enable1,Speed);
  analogWrite(enable2,Speed);
  delay(10);
}
  void Stop(){
    digitalWrite(motor1,LOW); 
    digitalWrite(motor2,LOW);
    digitalWrite(motor3,LOW);
    digitalWrite(motor4,LOW);
  }
  
void forward(){
  digitalWrite(motor1,HIGH);
  digitalWrite(motor2,LOW);
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,LOW);
}

void backwards(){
  digitalWrite(motor1,LOW);
  digitalWrite(motor2,HIGH);
  digitalWrite(motor3,LOW);
  digitalWrite(motor4,HIGH);
}

void turnleft(){
  Speed=Speed-40;
  digitalWrite(motor1,HIGH);
  digitalWrite(motor2,LOW);
  digitalWrite(motor3,LOW);
  digitalWrite(motor4,HIGH);
  delay(100);
  Speed=Speed+40;
  Serial.println(Speed);
}

void turnright(){
  Speed=Speed-40;
  digitalWrite(motor1,LOW);
  digitalWrite(motor2,HIGH);
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,LOW);
  delay(100);
  Speed=Speed+40;
  Serial.println(Speed);
}
void slowdown(){  
  if(120<Speed){
    Speed=Speed-10;
  }
  Serial.println(Speed);
  delay(300);
}
void speedup(){
  if(Speed<=245){
    Speed=Speed+10;
  }
  delay(300);
 Serial.println(Speed);
}
void modtagFraBluetooth(){
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  Serial.println(msg);
  int val = pulseIn(EchoPin, HIGH);
  val=val/58;
  if (val >= 200) {
    Serial.print("Out of range.");
    Serial.println();
  }
  else {
    Serial.print("Distance: ");
    Serial.print(val);
    Serial.print(" cm ");
    Serial.println();
  }

  delay(10);
  if(bluetooth.available()){ 
    msg=(char)bluetooth.read();
    } 
    if(val<25 && msg =='a' && 0<val){
    turnright();
         
    }else if(msg =='s'){
      Stop();
    }     
      else if(msg =='v'){
        turnleft();
    }
      else if(msg =='h'){
        turnright();
      }
      else if(msg =='b'){
        backwards();
      }
      else if (msg =='a'){
        forward();
      }
      if(msg =='+'){
        speedup();
      }else if(msg =='-'){
        slowdown();
      } 
  
      
     //returner det modtagne
  }

