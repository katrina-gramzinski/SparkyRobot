/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

  The circuit:
    - +V connection of the PING))) attached to +5V
    - GND connection of the PING))) attached to ground
    - SIG connection of the PING))) attached to digital pin 7

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Ping
*/

// this constant won't change. It's the pin number of the sensor's output:
const int pingLeft = 2;
const int pingMiddle = 7;
const int pingRight = 13;
int incomingByte = 0;
const int enablePinLeft = 6;
const int aPinLeft = 5;
const int bPinLeft = 4;
const int pwmLeft = 3;
const int wheel_speed = 60;

   
const int enablePinRight = 8;
const int aPinRight = 9;
const int bPinRight = 10;
const int pwmRight = 11;
int output[3];
int buf = 0 ;  
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(aPinLeft, OUTPUT);
  pinMode(bPinLeft, OUTPUT);
  pinMode(pwmLeft, OUTPUT);
  pinMode(enablePinLeft, OUTPUT);

  pinMode(aPinRight, OUTPUT);
  pinMode(bPinRight, OUTPUT);
  pinMode(pwmRight, OUTPUT);
  pinMode(enablePinRight, OUTPUT);
   
}

void loop() {
  incomingByte = Serial.read();
  if ( buf !=  incomingByte && incomingByte !=-1 ) {
    buf = incomingByte;
//  Serial.println(incomingByte);
    if (incomingByte==1)
      left(wheel_speed);
    else if (incomingByte==2)
      right(wheel_speed);
    else if (incomingByte==3)
      forward(wheel_speed);
    else if (incomingByte==4)
      backward(wheel_speed);
    else if (incomingByte==5)
      roam();
    else 
      halt();
   } 
}

void roam(){
  while (incomingByte!=0) {
    checkPingSensors();
//    Serial.print(output[0]);
//    Serial.print(" ");
//    Serial.println(output[1]);
//    Serial.println(" ");
//    Serial.println(output[2]);
    if (output[0] < 30 || output[1] < 30 || output[2] < 30) {
      if (output[0] < 10)
        right(wheel_speed-20);
      else if (output[2] < 10)
        left(wheel_speed-20);
      else if (output[1] > 30) {
        forward(wheel_speed-20);
      }
      else if (output[0] > output[2]) {
        left(wheel_speed-20);
        //delay(1000);
      }
      else {
        right(wheel_speed-20);
        //delay(1000);
      }
    }
    else
      forward(wheel_speed-20);
    delay(1);
    incomingByte = Serial.read();
    if ( buf !=  incomingByte && incomingByte !=-1 ) {
      buf = incomingByte;
      if (incomingByte == 0)
        halt();
        break;
    }
  }
  return;
}

void checkPingSensors(){ 
  long durationMiddle, durationLeft, durationRight, inchesMiddle, inchesRight, inchesLeft, cmMiddle, cmLeft, cmRight;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingLeft, OUTPUT);
  digitalWrite(pingLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(pingLeft, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingLeft, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingLeft, INPUT);
  durationLeft = pulseIn(pingLeft, HIGH);

  // convert the time into a distance
  cmLeft = microsecondsToCentimeters(durationLeft);

  //Serial.print(cmLeft);
  //Serial.print(" cm");
  //Serial.print("\t");
  output[0] = cmLeft;

  pinMode(pingMiddle, OUTPUT);
  digitalWrite(pingMiddle, LOW);
  delayMicroseconds(2);
  digitalWrite(pingMiddle, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingMiddle, LOW);

  pinMode(pingMiddle, INPUT);
  durationMiddle = pulseIn(pingMiddle, HIGH);

  cmMiddle = microsecondsToCentimeters(durationMiddle);
  
  //Serial.print(cmMiddle);
  //Serial.print(" cm");
  //Serial.print("\t");
  output[1] = cmMiddle;

  pinMode(pingRight, OUTPUT);
  digitalWrite(pingRight, LOW);
  delayMicroseconds(2);
  digitalWrite(pingRight, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingRight, LOW);

  pinMode(pingRight, INPUT);
  durationRight = pulseIn(pingRight, HIGH);

  cmRight = microsecondsToCentimeters(durationRight);
  
  //Serial.print(cmRight);
  //Serial.println(" cm");
  output[2] = cmRight;

  delay(100);
}



long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void left(int pwmSpeed){
  digitalWrite(aPinRight, HIGH);
  digitalWrite(bPinRight, LOW);
  digitalWrite(enablePinRight, HIGH);
  analogWrite(pwmRight, pwmSpeed);

  digitalWrite(aPinLeft, HIGH);
  digitalWrite(bPinLeft, LOW);
  digitalWrite(enablePinLeft, HIGH);
  analogWrite(pwmLeft, pwmSpeed);
}

void halt(){
  digitalWrite(aPinRight, LOW);
  digitalWrite(bPinRight, LOW);
  digitalWrite(enablePinRight, LOW);
  

  digitalWrite(aPinLeft, LOW);
  digitalWrite(bPinLeft, LOW);
  digitalWrite(enablePinLeft, LOW);
  
}

void right(int pwmSpeed){
  digitalWrite(aPinRight, LOW);
  digitalWrite(bPinRight, HIGH);
  digitalWrite(enablePinRight, HIGH);
  analogWrite(pwmRight, pwmSpeed);

  digitalWrite(aPinLeft, LOW);
  digitalWrite(bPinLeft, HIGH);
  digitalWrite(enablePinLeft, HIGH);
  analogWrite(pwmLeft, pwmSpeed);
}

void backward(int pwmSpeed){
  digitalWrite(aPinRight, LOW);
  digitalWrite(bPinRight, HIGH);
  digitalWrite(enablePinRight, HIGH);
  analogWrite(pwmRight, pwmSpeed-10);

  digitalWrite(aPinLeft, HIGH);
  digitalWrite(bPinLeft, LOW);
  digitalWrite(enablePinLeft, HIGH);
  analogWrite(pwmLeft, pwmSpeed);
}

void forward(int pwmSpeed){
  // put your main code here, to run repeatedly:
  digitalWrite(aPinRight, HIGH);
  digitalWrite(bPinRight, LOW);
  digitalWrite(enablePinRight, HIGH);
  analogWrite(pwmRight, pwmSpeed-10);

  digitalWrite(aPinLeft, LOW);
  digitalWrite(bPinLeft, HIGH);
  digitalWrite(enablePinLeft, HIGH);
  analogWrite(pwmLeft, pwmSpeed);
  
  delayMicroseconds(10);
  }
  int serialReadInt() {
  int ret = 0;
  for (int i = 1; i >= 0; i--) {
    while (!Serial.available()) {
    }
    ret = Serial.read();
    Serial.println(ret);
   // ret |= b << (i * 8);
  }
  return ret;
}
