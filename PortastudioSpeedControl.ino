/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 6; // Analog output pin that the LED is attached to


#define STATE_STILL_INITIAL_OUTPUT (1)
#define STATE_NORMAL (2)
int operating_state = STATE_STILL_INITIAL_OUTPUT;


float initSensorValue_V = 0;
#define THRESHOLD_VOLTS (0.2f) //for determining if we've departed from the initial reading
float prev_value = 0.0f;
float learn_fac = 0.25f;

float default_output_V = 1.67f;

#define OUTPUT_RANGE_V (5.0f)
#define OUTPUT_RANGE_COUNTS (256)
#define INPUT_RANGE_COUNTS (1024)
#define INPUT_RANGE_V (5.0f)

float convertInputCountsToVolts(int input_counts) {
  return map((float)input_counts,0.0f,(float)INPUT_RANGE_COUNTS,0.0f,INPUT_RANGE_V);
}

void writeOutputVoltage(int analogPin,float voltage) {
  float output_float = map(voltage,0.0f,OUTPUT_RANGE_V,0.0f,(float)OUTPUT_RANGE_COUNTS);
  int output_counts = (int)(output_float+0.5f);
  analogWrite(analogPin,output_counts);
}
  

void setup() {
  // initialize serial communications
  Serial.begin(115200); 
  
  //stall for a moment
  delay(10);
  
  //command the default output
  writeOutputVoltage(analogOutPin,default_output_V);
  
  //get the current potentiometer value
  initSensorValue_V = convertInputCountsToVolts(analogRead(analogInPin));
  delay(10);
}


void loop() {
  // read the analog in value:
  float sensorValue_V = convertInputCountsToVolts(analogRead(analogInPin));
  
  switch (operating_state) {
    case (STATE_STILL_INITIAL_OUTPUT):
      if (abs(sensorValue_V - initSensorValue_V) > THRESHOLD_VOLTS) {
        operating_state = STATE_NORMAL;  //switch to the normal mode of operation
      }
      break;
    case (STATE_NORMAL):
      writeOutputVoltage(analogOutPin,sensorValue_V);
  }
  
  delay(10); //keep the sample rate at a reasonable speed;
}



