#include<SoftwareSerial.h>
int bluetoothTx = 2;
int bluetoothRx = 3;
int motor1 = 11;
int motor2 = 10;
int motor3 = 5;
int motor4 = 7;
int enable1 = 9;
int enable2 = 6;
int TriggerPin = 12;
int EchoPin = 13;
char msg;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);//Arduino RX,Tx
int counter = 0;
int Speed = 205;

void setup() {
  //Motor-pins
  pinMode(motor1, OUTPUT);
  pinMode(motor2,OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  //H-bro pins
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  Serial.begin(9600);
  digitalWrite(EchoPin,LOW);
  bluetooth.begin(115200);//standard bluetooth mate
  //Ultra-sound pins
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
void loop() {
  //Bluetooth funktioner.
  bluetooth.listen();
  modtagFraBluetooth();
  //Sender signalet (retning) og hastighed til app'en. Kan findes i div-element "fraArduino".
  bluetooth.print(msg);
  bluetooth.println(Speed);
  //Sætter default-speeden
  analogWrite(enable1,Speed);
  analogWrite(enable2,Speed);
  delay(10);
}
//Følgende funktioner bestemmer outputten til de forskellige motor-forbindelser ved forskellige kommandoer.
//Ved stop, f.x. skal alle fire motorer have en LOW input.
void Stop() {
  digitalWrite(motor1, LOW); 
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
}  
void forward() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
}
void backwards() {
  digitalWrite(motor1,LOW);
  digitalWrite(motor2,HIGH);
  digitalWrite(motor3,LOW);
  digitalWrite(motor4,HIGH);
}
void turnleft() {
  Speed=Speed-40;
  digitalWrite(motor1,HIGH);
  digitalWrite(motor2,LOW);
  digitalWrite(motor3,LOW);
  digitalWrite(motor4,HIGH);
  delay(100);
  Speed=Speed+40;
  Serial.println(Speed);
}
void turnright() {
  Speed=Speed-40;
  digitalWrite(motor1,LOW);
  digitalWrite(motor2,HIGH);
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,LOW);
  delay(100);
  Speed=Speed+40;
  Serial.println(Speed);
}
//slowdown() og speedup() har en grænse for hvor høj og lav de kan sætte Speed-værdien, da analogwrite ikke kan bruge højere end 255,
//og motorerne ikke kører med en input på lavere end ~120.
void slowdown() {  
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
  Serial.println(msg);
  
  //Kode til ultralyds-sensoren. Aktiverer og de-aktiverer TriggerPin i korte intervaller, for at måle hvor hurtigt echoet kommer tilbage.
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  
  int val = pulseIn(EchoPin, HIGH);
  val=val/58; //Divider med 58 for at omregne til distance i stedet for tid.
  //Printer distancen til console.
  if (val >= 200) {
    Serial.print("Out of range.");
    Serial.println();
  } else {
    Serial.print("Distance: ");
    Serial.print(val);
    Serial.print(" cm ");
    Serial.println();
  }
  //Bestemmer hvilke bevægelses-funktioner der køres ud fra signalet modtaget fra remoten og om der er nok plads foran robotten til at køre frem.
  if(bluetooth.available()) { 
    msg=(char)bluetooth.read();
  } 
  if(val<25 && msg =='a' && 0<val) {
    turnright();
  } else if(msg =='s') {
    Stop();
  } else if(msg =='v') {
    turnleft();
  } else if(msg =='h') {
    turnright();
  } else if(msg =='b') {
    backwards();
  } else if (msg =='a'){
    forward();
  } else {
    Stop(); 
  }
  if(msg =='+'){
    speedup();
  } else if(msg =='-') {
    slowdown();
  } else {
    Stop();
  }
}
