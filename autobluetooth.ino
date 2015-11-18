/*
 * Bluetooth remote control of a "car"
 *  control is done in tank-like fashion: two motors, left/right
 *
 * Idea and lots of code taken/inspired by:
 *  https://sites.google.com/site/bluetoothrccar/
 *  goes with Android App:
 *  https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=fr
 *
 * connexions:
 #  ARDUINO   < ========== > Bluetooth
 *  yellow:  0 rx  (arduino) tx  vert  (bluetooth)
 *  white :  1 tx  (arduino) rx  noir  (bluetooth)
 *  vert  :    5v  (arduino) vcc rouge (bluetooth)
 *  blanc :    Gnd (arduino) vcc orange(bluetooth)
 *
 * License: MIT
 */

#include <AFMotor.h>
const int pinfrontLights = 7;    //Pin that activates the Front lights.
const int pinbackLights  = 4;    //Pin that activates the Back lights.
char command = 'S';      // variable to receive the command from the serial port
char prevCommand = 'A';
int velocity = 0;
int turnRatio = 4;
int turnSpeed = 75;
unsigned long timer0 = 2000;  //Stores the time (in millis since execution started)
unsigned long timer1 = 0;  //Stores the time when the last command was received from the phone

// test
//AF_DCMotor motd(4,MOTOR12_64KHZ);
AF_DCMotor motd(4);
AF_DCMotor motg(3);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hello Humans! I've just started!");
}

void loop() {
  if(Serial.available() > 0){
    command = Serial.read();
    //Change pin mode only if new command is different from previous.
    Serial.println(command);
    switch(command){
    case 'F':
      goForward(velocity);
      break;
    case 'B':
      goBack(velocity);
      break;
    case 'L':
      goLeft();
      break;
    case 'R':
      goRight();
      break;
    case 'S':
      stop();
      break;
    case 'I':  //FR
      goRight(velocity);
      break;
    case 'J':  //BR
      goBackRight(velocity);
      break;
    case 'G':  //FL
      goLeft(velocity);
      break;
    case 'H':  //BL
      goBackLeft(velocity);
      break;
    case 'W':  //Font ON
      digitalWrite(pinfrontLights, HIGH);
      break;
    case 'w':  //Font OFF
      digitalWrite(pinfrontLights, LOW);
      break;
    case 'U':  //Back ON
      digitalWrite(pinbackLights, HIGH);
      break;
    case 'u':  //Back OFF
      digitalWrite(pinbackLights, LOW);
      break;
    case 'D':  //Everything OFF
      digitalWrite(pinfrontLights, LOW);
      digitalWrite(pinbackLights, LOW);
      stop();
      break;
    default:  //Get velocity
      if(command=='q'){
        velocity = 255;  //Full velocity
        //yellowCar.SetSpeebd_4W(velocity);
        setSpeed(velocity);
      }
      else{
        //Chars '0' - '9' have an integer equivalence of 48 - 57, accordingly.
        if((command >= 48) && (command <= 57)){
          //Subtracting 48 changes the range from 48-57 to 0-9.
          //Multiplying by 25 changes the range from 0-9 to 0-225.
          velocity = (command - 48)*25;
          //yellowCar.SetSpeed_4W(velocity);
          setSpeed(velocity);
        }
      }
    }
  }
}

void goBack(int velocity){
  motd.run(BACKWARD);
  motg.run(BACKWARD);
  motd.setSpeed(velocity);
  motg.setSpeed(velocity);
}

void goForward(int velocity){
  analogWrite(led1, 128);
  motd.run(FORWARD);
  motg.run(FORWARD);
  motd.setSpeed(velocity);
  motg.setSpeed(velocity);
}

void stop(){
  ledOff();
  motd.setSpeed(0);
  motg.setSpeed(0);
}

void goLeft(){
  goLeft(turnSpeed);
}

//yellowCar.ForwardRight_4W(velocity);
void goLeft(int velocity){
  goLeft(velocity, FORWARD);
}

void goBackLeft(int velocity){
  goLeft(velocity, BACKWARD);
}

void goLeft(int velocity, int direction){
  ledOn();
  motg.run(RELEASE);
  motd.run(direction);
  motd.setSpeed(velocity/turnRatio);
}


//yellowCar.Right_4W();
void goRight(){
  goRight(turnSpeed);
}

//yellowCar.ForwardRight_4W(velocity);
void goRight(int velocity){
  goRight(velocity, FORWARD);
}

void goBackRight(int velocity){
  goRight(velocity, BACKWARD);
}

void goRight(int velocity, int direction){
  ledOn();
  motd.run(RELEASE);
  motg.run(direction);
  motg.setSpeed(velocity/turnRatio);
}

void setSpeed(int velocity){
  motd.setSpeed(velocity);
  motg.setSpeed(velocity);
}

void ledOn(){
  analogWrite(13, 128);
}
void ledOff(){
  analogWrite(13, 0);
}
