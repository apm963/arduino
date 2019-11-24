/*
Supposedly 2048 steps / revolution
  (note: alternate sources say 2038 although that may be accent)
  See https://www.seeedstudio.com/blog/2019/03/04/driving-a-28byj-48-stepper-motor-with-a-uln2003-driver-board-and-arduino/
2ms minimum delay between steps
Max speed is 500 steps/s meaning 4 seconds for full rotation
Microstepping "might" be torquier
*/

/*
#include <AccelStepper.h>
// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
void setup()
{
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(200);
  stepper.setAcceleration(120);
  //stepper.moveTo(500);
}
void loop()
{
  if (stepper.distanceToGo() == 0)
  {
    // Random change to speed, position and acceleration
    // Make sure we dont get 0 speed or accelerations
    delay(1000);
    stepper.moveTo(rand() % 200);
    stepper.setMaxSpeed((rand() % 200) + 1);
    stepper.setAcceleration((rand() % 200) + 1);
  }
  stepper.run();
}
 */

/* 
#include <Stepper.h>

const int stepsPerRevolution = 64;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 2 through 5:
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one step:
  myStepper.step(1);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(50);
}
 */


#include <Stepper.h>

const int stepsPerRevolution = 2048; // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 2 through 5:
Stepper stepper(stepsPerRevolution, 2, 3, 4, 5);

int stepCount = 0;  // number of steps the motor has taken

void setup() {
  // nothing to do inside the setup
}

void loop() {
  // read the sensor value:
  //int sensorReading = analogRead(A0);
  // map it to a range from 0 to 100:
  //int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
  int motorSpeed = 1;
  /*
  // set the motor speed:
  if (motorSpeed > 0) {
    stepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    stepper.step(stepsPerRevolution / 100);
  }
  */
  stepper.setSpeed(6); // 1 rpm
  stepper.step(stepsPerRevolution / 10);  // do 2038 steps -- corresponds to one revolution in one minute
  delay(1000);         // wait for one second
  //stepper.setSpeed(6); // 6 rpm
  //stepper.step(-stepsPerRevolution); // do 2038 steps in the other direction with faster speed -- corresponds to one revolution in 10 seconds
}


/* 
//int ledPin = 13;
int stepperA = 2;
int stepperB = 3;
int stepperC = 4;
int stepperD = 5;

int stepHighDelay = 5;
int stepTimeBuffer = 20;

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledPin, OUTPUT);
  pinMode(stepperA, OUTPUT);
  pinMode(stepperB, OUTPUT);
  pinMode(stepperC, OUTPUT);
  pinMode(stepperD, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(stepperA, HIGH);
  delay(stepHighDelay);
  digitalWrite(stepperA, LOW);
  delay(stepTimeBuffer);

  digitalWrite(stepperB, HIGH);
  delay(stepHighDelay);
  digitalWrite(stepperB, LOW);
  delay(stepTimeBuffer);

  digitalWrite(stepperC, HIGH);
  delay(stepHighDelay);
  digitalWrite(stepperC, LOW);
  delay(stepTimeBuffer);

  digitalWrite(stepperD, HIGH);
  delay(stepHighDelay);
  digitalWrite(stepperD, LOW);
  delay(stepTimeBuffer);
}
 */