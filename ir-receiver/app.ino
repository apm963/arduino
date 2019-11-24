
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
#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}
*/

/*
// This sketch demonstrates the IrReceiverSampler.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <Arduino.h>
#include <IrReceiverPoll.h>
//#include <IrReceiverSampler.h>

#define RECEIVE_PIN 7U
#define BUFFERSIZE 200U
#define BAUD 9600

IrReceiver *receiver;

void setup()
{
  Serial.begin(BAUD);
  receiver = new IrReceiverPoll(BUFFERSIZE, RECEIVE_PIN);
  //receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
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
// NOTE: This appears to be the most reliable baseline to get reliable and readable results

#include <IRremote.h>
int input_pin = 7; //set D7 as input signal pin
IRrecv irrecv(input_pin);
decode_results signals;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // enable input from IR receiver
}
void loop()
{
  if (irrecv.decode(&signals))
  {
    Serial.println(signals.value, HEX);
    irrecv.resume(); // get the next signal
  }
}
*/

// Read signal multiple times and store

#include <IRremote.h>

const int input_pin = 7U;
IRrecv irrecv(input_pin);
decode_results signals;

unsigned long last_signal1;
unsigned long last_signal2;
unsigned long stored_signal;
bool print_null_values = false;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // enable input from IR receiver
  irrecv.blink13(true);
}

void process_valid_signal(unsigned long signal_val)
{
  // Compare values
  if (signal_val == last_signal1 && signal_val == last_signal2)
  {
    stored_signal = signal_val;
    Serial.print("Stored value: ");
    Serial.println(stored_signal, HEX);
    last_signal2 = 0;
    last_signal1 = 0;
  }
  else
  {
    // Push to history
    last_signal2 = last_signal1;
    last_signal1 = signal_val;
  }
}

void loop()
{
  if (irrecv.decode(&signals))
  {
    
    unsigned long signal_val = signals.value;
    bool is_null = (signal_val == 4294967295);

    if (!is_null || print_null_values)
    {
      Serial.print("Received value: ");
      Serial.print(signal_val, HEX);
      Serial.print(" (");
      Serial.print(signal_val);
      Serial.println(")");
    }
    
    if (!is_null)
    {
      process_valid_signal(signal_val);
    }
    
    irrecv.resume(); // get the next signal
  }
}
