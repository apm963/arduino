/*
const int sensorPin = A0;
const int ledPin = 13;
const int cycleMsOn = 10;
const int cycleMsOff = 10;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  //Serial.print("test");
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(20);
  //digitalWrite(sensorPin, HIGH);
  //delay(cycleMsOn);
  //digitalWrite(sensorPin, LOW);
  //delay(cycleMsOff);
}
*/

/*
const int sensorPin = 9;
const int ledPin = 13;
const int cycleMsOn = 10;
const int cycleMsOff = 10;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  //Serial.print("test");
}

void loop() {
  sensorValue = digitalRead(sensorPin);
  Serial.println(sensorValue);
  delay(20);
  //digitalWrite(sensorPin, HIGH);
  //delay(cycleMsOn);
  //digitalWrite(sensorPin, LOW);
  //delay(cycleMsOff);
}
*/

/*
// This sketch demonstrates the IrReceiverPoll.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <Arduino.h>
#include <IrReceiverPoll.h>

#define RECEIVE_PIN 9U
#define BUFFERSIZE 200U
#define BAUD 9600

IrReceiver *receiver;

void setup()
{
  Serial.begin(BAUD);
  receiver = new IrReceiverPoll(BUFFERSIZE, RECEIVE_PIN);
}

void loop()
{
  receiver->receive(); // combines enable, loop, disable

  if (receiver->isEmpty())
    Serial.println(F("timeout"));
  else
    receiver->dump(Serial);
}
*/

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

/*
#include <IRremote.h>

int RECV_PIN = 9;

IRsend irsend;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop()
{

  int khz = 38;                                                                                                                                                                                                                                                                                                                                                                                                        // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {9000, 4500, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 39416, 9000, 2210, 560}; //AnalysIR Batch Export (IRremote) - RAW

  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  //delay(100);
  delay(5000); //In this example, the signal will be repeated every 5 seconds, approximately.
}
*/

/*
#include <Arduino.h>

const int irPin = A0;
int ifRead;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
}

void loop() {
  ifRead = analogRead(irPin);
  Serial.println(ifRead);
  delay(10);
}
*/

#include <Arduino.h>
#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  //irrecv.blink13(true);
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}
