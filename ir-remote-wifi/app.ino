#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <IRremote.h>

// WiFi vars
const char *ssid = "Chodie";
const char *password = "ApM.963#";

WebServer server(80);

const int led = 13;

// IR vars
const int input_pin = 7U;
const int ir_emitter_pin = 10U;

IRsend irsend;
IRrecv irrecv(input_pin);
decode_results signals;

unsigned long last_signal1;
unsigned long last_signal2;
unsigned long stored_signal;
bool print_null_values = true;
bool is_recording = false;

void handleRoot()
{
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
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
    is_recording = false;
  }
  else
  {
    // Push to history
    last_signal2 = last_signal1;
    last_signal1 = signal_val;
  }
}

void emit_ir()
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

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  
  irrecv.enableIRIn(); // enable input from IR receiver
  irrecv.blink13(true);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  
  server.on("/ir/record", []() {
    is_recording = true;
    server.send(200, "text/plain", "recording started");
  });

  server.on("/ir/record/status", []() {
    // NOTE: This ternary has not been tested and may not work
    server.send(200, "text/plain", (is_recording ? "currently recording" : "not recording"));
  });

  server.on("/ir/record/stop", []() {
    is_recording = false;
    server.send(200, "text/plain", "recording stopped");
  });
  
  server.on("/ir/emit", []() {
    // TODO: Make this work for multiple signals
    emit_ir();
    server.send(200, "text/plain", "signal emitted");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  
  server.handleClient();

  if (is_recording && irrecv.decode(&signals))
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
