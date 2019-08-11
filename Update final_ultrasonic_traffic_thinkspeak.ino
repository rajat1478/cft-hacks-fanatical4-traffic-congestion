
#include <SoftwareSerial.h>
#include <stdlib.h>

//shift register pins
int latchPin = 11;
int clockPin = 13;
int dataPin = 12;

//Ultrasonic sensor pins
const int  trigPin=10;
int echoPin1 = 9;
int echoPin2 = 8;
int echoPin3 = 7;
int echoPin4 = 6;
int echoPin5 = 5;
int echoPin6 = 4;
int echoPin7 = 3;
int echoPin8 = 2;
int sensor1State=0;
int sensor2State=0;
int sensor3State=0;
int sensor4State=0;
long duration, distance, UltraSensor1,UltraSensor2,UltraSensor3,UltraSensor4,UltraSensor5,UltraSensor6,UltraSensor7,UltraSensor8,road1,road2,road3,road4;
int count1,count2,count3,count4;
word case0 = 0b10010010;
byte case1=  0b00010000;
word case2 = 0b10010010;
byte case3 = 0b00100000;
word case4 = 0b10010000;
byte case5 = 0b11000000;
word case6 = 0b10010001;
byte case7 = 0b10100000;
word case8 = 0b10000110;
byte case9 = 0b01000000;
word case10 = 0b10001010;
byte case11 = 0b01000000;
word case12 = 0b00110010;
byte case13 = 0b01000000;
word case14 = 0b01010010;
byte case15 = 0b01000000;
word case16 = 0b01001001;
byte case17 = 0b00100000;



// replace with your channel's thingspeak API key
String apiKey = "API_key";    //Api key of my channel Traffic management system

// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(2, 3); // RX, TX

int i=1;


// this runs once
void setup() {
  //for setting shift register pin into output     
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

 //for Setting ultrasonic sensor pins into output and input 
pinMode(trigPin,OUTPUT);             //Trigger to all ultrasonic sensors
pinMode(echoPin1,INPUT);             //define pin into iinput to receive echos
pinMode(echoPin2,INPUT);      
pinMode(echoPin3,INPUT);
pinMode(echoPin4,INPUT);
pinMode(echoPin5,INPUT);
pinMode(echoPin6,INPUT);
pinMode(echoPin7,INPUT);
pinMode(echoPin8,INPUT);
  
  // enable debug serial
  Serial.begin(115200); 
  // enable software serial
  ser.begin(115200);
  
  // reset ESP8266
  ser.println("AT+RST");
}


// the loop 
void loop() {
  
  //for triggering all the sensors
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
      

  SonarSensor(trigPin, echoPin1);          //function to calculate distance of 1st sensor
  UltraSensor1 = distance; 
  Serial.print("Distance1");
  Serial.println(UltraSensor1);
  SonarSensor(trigPin, echoPin2);          //function to calculate distance of 2nd sensor
  UltraSensor2 = distance; 
  Serial.print("Distance2");
  Serial.println(UltraSensor2);
  road1=UltraSensor1+UltraSensor2;
  
  SonarSensor(trigPin, echoPin3);          //function to calculate distance of 3rd sensor
  UltraSensor3 = distance; 
  Serial.print("Distance3");
  Serial.println(UltraSensor3);
  SonarSensor(trigPin, echoPin4);          //function to calculate distance of 4th sensor
  UltraSensor4 = distance; 
  Serial.print("Distance4");
  Serial.println(UltraSensor4);
  road2=UltraSensor3+UltraSensor4;

  
  SonarSensor(trigPin, echoPin5);          //function to calculate distance of 5th sensor
  UltraSensor5 = distance; 
  Serial.print("Distance5");
  Serial.println(UltraSensor5);
  SonarSensor(trigPin, echoPin6);          //function to calculate distance of 6th sensor
  UltraSensor6 = distance;
  Serial.print("Distance6"); 
  Serial.println(UltraSensor6);
  road3=UltraSensor5+UltraSensor6;

  
  SonarSensor(trigPin, echoPin7);          //function to calculate distance of 7th sensor
  UltraSensor7 = distance; 
  Serial.print("Distance7");
  Serial.println(UltraSensor7);
  SonarSensor(trigPin, echoPin8);          //function to calculate distance of 8th sensor
  UltraSensor8 = distance; 
  Serial.print("Distance8");
  Serial.println(UltraSensor8);
  road4=UltraSensor7+UltraSensor8;

  //For conversion into String it is used to display on server
  String state1=String(road1);
  String state2=String(road2);
  String state3=String(road3);
  String state4=String(road4);


  //Conditions for checking density of perticular road and release the signals
  
  if(road1<road2&&road1<road3&&road1<road4){
    count1++;
     // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
    
    getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

   //for 4th green light
   case1 = 0b10010010;
   case0=  0b00010000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case1); 
   shiftOut(dataPin, clockPin, LSBFIRST, case0);
   digitalWrite(latchPin, HIGH);
   delay(15000);
   
    if(!(road1<road2&&road1<3&&road1<road4))
    {
      
    //for 4th yellow light 
   case2 = 0b10010010;
   case3 = 0b00100000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case3);
   shiftOut(dataPin, clockPin, LSBFIRST, case2);
   digitalWrite(latchPin, HIGH);
   delay(8000);
    }
    if(count1>5)
    {
      count1=0;
      routine();
    }
  
}
 else if (road2<road1&&road2<road3&&road2<road4){
  count2++;
   // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
    
    getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

    //For 3rd green light
   case4 = 0b10010000;
   case5 = 0b11000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case5);
   shiftOut(dataPin, clockPin, LSBFIRST, case4);
   digitalWrite(latchPin, HIGH);
   delay(10000);

  if(!(road2<road1&&road2<road3&&road2<road4)){
    
   //for 3rd yellow light
   case6 = 0b10010001;
   case7 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case7);
   shiftOut(dataPin, clockPin, LSBFIRST, case6);
   digitalWrite(latchPin, HIGH);
   delay(8000);
  }
   
   if(count2>5)
    {
      count2=0;
      routine();
    }
 
}

else if(road3<road1&&road3<road2&&road3<road4){
  count3++;
   // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
    
    getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

    //for 2nd green light 
    case8 = 0b10000110;
    case9 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case9);
   shiftOut(dataPin, clockPin, LSBFIRST, case8);
   digitalWrite(latchPin, HIGH);
   delay(10000);
  
  if(!(road3<road1&&road3<road2&&road3<<road4)){
    
   //for 2nd yellow light
    case10 = 0b10001010;
    case11 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case11);
   shiftOut(dataPin, clockPin, LSBFIRST, case12);
   digitalWrite(latchPin, HIGH);
   delay(5000);

}
  if(count3>5)
    {
      count3=0;
      routine();
    }
}

else if (road4<road1&&road4<road2&&road4<road3){
  count4++;
   // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
    
    getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

    //for 1st greeen light 
    case12 = 0b00110010;
    case13 = 0b01000000; 
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case13);
   shiftOut(dataPin, clockPin, LSBFIRST, case12);
   digitalWrite(latchPin, HIGH);
   delay(1000);

  if(!(road4<road1&&road4<road2&&road4<road3))
  {
    //for 1st yellow light 
   case14 = 0b01010010;
   case15 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case15);
   shiftOut(dataPin, clockPin, LSBFIRST, case14);
   digitalWrite(latchPin, HIGH);
   delay(1000);
}
  if(count4>5)
    {
      count4=0;
      routine();
    }
}
else {
  
   // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
    
  getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
  getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  
   case14 = 0b01001001;
   case15 = 0b00100000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case15);
   shiftOut(dataPin, clockPin, LSBFIRST, case14);
   digitalWrite(latchPin, HIGH);
   delay(1000);
  
}


  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  /*if(i==0)
  {
    Serial.println("1");
  getStr +="&field1=";
  getStr += String(state1);
  i++;
  }
  else if(i==1)
  {Serial.println("2");
  getStr +="&field2=";
  getStr += String(state2);
  i++;
  }
  else if(i==2)
  {
    Serial.println("3");
  getStr +="&field3=";
  getStr += String(state3);
  i++;
  }
  else if(i==3)
  {
    Serial.println("4");
  getStr +="&field4=";
  getStr += String(state4);
  i=0;
  }
  
  */
  getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}

void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration*0.034/2;
delay(100);
} 

void routine()
{
  //for 4th green light
   case1 = 0b10010010;
   case0=  0b00010000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case1); 
   shiftOut(dataPin, clockPin, LSBFIRST, case0);
   digitalWrite(latchPin, HIGH);
   delay(5000);
   
    //for 4th yellow light 
   case2 = 0b10010010;
   case3 = 0b00100000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case3);
   shiftOut(dataPin, clockPin, LSBFIRST, case2);
   digitalWrite(latchPin, HIGH);
   delay(3000);
  
  //For 3rd green light
   case4 = 0b10010000;
   case5 = 0b11000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case5);
   shiftOut(dataPin, clockPin, LSBFIRST, case4);
   digitalWrite(latchPin, HIGH);
   delay(5000);

   //for 3rd yellow light
   case6 = 0b10010001;
   case7 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case7);
   shiftOut(dataPin, clockPin, LSBFIRST, case6);
   digitalWrite(latchPin, HIGH);
   delay(3000);
  
  //for 2nd green light 
    case8 = 0b10000110;
    case9 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case9);
   shiftOut(dataPin, clockPin, LSBFIRST, case8);
   digitalWrite(latchPin, HIGH);
   delay(5000);

   //for 2nd yellow light
    case10 = 0b10001010;
    case11 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case11);
   shiftOut(dataPin, clockPin, LSBFIRST, case12);
   digitalWrite(latchPin, HIGH);
   delay(3000);
  
  //for 1st greeen light 
    case12 = 0b00110010;
    case13 = 0b01000000; 
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case13);
   shiftOut(dataPin, clockPin, LSBFIRST, case12);
   digitalWrite(latchPin, HIGH);
   delay(5000);

   //for 1st yellow light 
   case14 = 0b01010010;
   case15 = 0b01000000;
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, case15);
   shiftOut(dataPin, clockPin, LSBFIRST, case14);
   digitalWrite(latchPin, HIGH);
   delay(3000);
  //end
}
