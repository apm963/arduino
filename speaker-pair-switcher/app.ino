
const int button_pin = 2U;
const int switch_pins_alpha[2] = {3U, 4U};
const int switch_pins_beta[2] = {5U, 6U};

bool button_state = false;
int switch_state = 0; // 0 = both, 1 = alpha, 2 = beta

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_pin, INPUT);
  for (byte i = 0; i < 2; i++)
  {
    pinMode(switch_pins_alpha[i], OUTPUT);
    pinMode(switch_pins_beta[i], OUTPUT);
  }
  refresh_pins();
}

void button_press()
{
  // Cycle
  switch_state++;
  
  if (switch_state > 2)
  {
    switch_state = 0;
  }

  refresh_pins();
}

void refresh_pins()
{
  bool switch_on_alpha = true;
  bool switch_on_beta = true;
  
  if (switch_state == 1)
  {
    switch_on_beta = false;
  }
  else if (switch_state == 2)
  {
    switch_on_alpha = false;
  }
  
  // Reset all pins
  for (byte i = 0; i < 2; i++)
  {
    digitalWrite(switch_pins_alpha[i], LOW);
    digitalWrite(switch_pins_beta[i], LOW);
  }
  
  // Update pins
  if (switch_on_alpha)
  {
    for (byte i = 0; i < 2; i++)
    {
      digitalWrite(switch_pins_alpha[i], HIGH);
    }
  }
  
  if (switch_on_beta)
  {
    for (byte i = 0; i < 2; i++)
    {
      digitalWrite(switch_pins_beta[i], HIGH);
    }
  }
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
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
  
}
