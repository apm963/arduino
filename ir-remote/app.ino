
// Read signal multiple times and store

#include <IRremote.h>

const int button_pin = 2U;
const int input_pin = 7U;
const int ir_emitter_pin = 10U;

IRsend irsend;
IRrecv irrecv(input_pin);
decode_results signals;

unsigned long last_signal1;
unsigned long last_signal2;
unsigned long stored_signal;
bool print_null_values = true;
bool button_state = false;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // enable input from IR receiver
  irrecv.blink13(true);
  pinMode(button_pin, INPUT);
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

void button_press()
{
  Serial.println("Button pressed");
  if (stored_signal > 0)
  {
    irsend.sendNEC(stored_signal, 32); // second param is how many bits to send
    // irsend.sendRaw(rawbuf, rawlen, frequency=38); TODO: Maybe transition to this
    Serial.println("Sent code");
    irrecv.enableIRIn(); // reenable input from IR receiver (see https://www.reddit.com/r/arduino/comments/2sgmhn/irremote_cant_receive_ir_after_sending/cnpblk9/)
  }
}

char *ir_vendor_name(char *buf, int vendor_enum)
{
  // Stupid archaic string manipulation in C: https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
  if (vendor_enum == UNKNOWN) { strcpy(buf, "UNKNOWN"); }
  else if (vendor_enum == UNUSED) { strcpy(buf, "UNUSED"); }
  else if (vendor_enum == RC5) { strcpy(buf, "RC5"); }
  else if (vendor_enum == RC6) { strcpy(buf, "RC6"); }
  else if (vendor_enum == NEC) { strcpy(buf, "NEC"); }
  else if (vendor_enum == SONY) { strcpy(buf, "SONY"); }
  else if (vendor_enum == PANASONIC) { strcpy(buf, "PANASONIC"); }
  else if (vendor_enum == JVC) { strcpy(buf, "JVC"); }
  else if (vendor_enum == SAMSUNG) { strcpy(buf, "SAMSUNG"); }
  else if (vendor_enum == WHYNTER) { strcpy(buf, "WHYNTER"); }
  else if (vendor_enum == AIWA_RC_T501) { strcpy(buf, "AIWA_RC_T501"); }
  else if (vendor_enum == LG) { strcpy(buf, "LG"); }
  else if (vendor_enum == SANYO) { strcpy(buf, "SANYO"); }
  else if (vendor_enum == MITSUBISHI) { strcpy(buf, "MITSUBISHI"); }
  else if (vendor_enum == DISH) { strcpy(buf, "DISH"); }
  else if (vendor_enum == SHARP) { strcpy(buf, "SHARP"); }
  else if (vendor_enum == DENON) { strcpy(buf, "DENON"); }
  else if (vendor_enum == PRONTO) { strcpy(buf, "PRONTO"); }
  else if (vendor_enum == LEGO_PF) { strcpy(buf, "LEGO_PF"); }
  return buf;
}

void loop()
{
  int button_state_int = digitalRead(button_pin);
  int temp_button_state = (button_state_int == HIGH);

  if (button_state != temp_button_state)
  {
    // Button state changed
    button_state = temp_button_state;
    if (button_state)
    {
      button_press();
    }
  }

  if (irrecv.decode(&signals))
  {
    
    unsigned long signal_val = signals.value;
    bool is_null = (signal_val == 4294967295);

    if (!is_null || print_null_values)
    {
      // Refer to: https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
      Serial.print("Received value: ");
      char vendor_name_buf[32] = "test";
      Serial.print(ir_vendor_name(vendor_name_buf, signals.decode_type));
      Serial.print(" ");
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
