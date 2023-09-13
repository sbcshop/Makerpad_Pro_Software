/*  Demo code to test onboard Buzzer of Makerpad Pro */

#define Buzzerpin 2   // Define the buzzer pin connected to GPIO2

void setup() {
  // Set up LED Control (LEDC) channel 0
  // Configuring LEDC channel 0 with a frequency of 100,000 Hz (1E5)
  // and a resolution of 12 bits.
  ledcSetup(0, 1E5, 12);

  // Attach LEDC channel 0 to the GPIO pin specified by Buzzerpin (GPIO2)
  ledcAttachPin(Buzzerpin, 0);
}

void loop() {
  // Generate a tone at 800 Hz using LEDC channel 0 (buzzer)
  ledcWriteTone(0, 800);
  
  // Delay for 1 second (1000 milliseconds)
  delay(1000);
  
  // Define the octave for the musical note
  uint8_t octave = 1;
  
  // Play the musical note C in the specified octave (octave 1)
  ledcWriteNote(0, NOTE_C, octave);
  
  // Delay for 1 second (1000 milliseconds)
  delay(1000);
}
