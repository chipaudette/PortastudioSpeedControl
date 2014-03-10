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

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int initSensorValue = 0;

float default_output_V = 1.67f;

#define OUTPUT_RANGE_V (5.0f)
#define OUTPUT_RANGE_COUNTS (256)
void writeOutputVoltage(int analogPin,float voltage) {
  float output_float = map(voltage,0.0f,OUTPUT_RANGE_V,0.0f,(float)OUTPUT_RANGE_COUNTS);
  analogWrite(analogPin,(int)(output_float+0.5f));
}
  

void setup() {
  // initialize serial communications
  Serial.begin(115200); 
  
  //stall for a moment
  delay(10);
  
  //command the default output
  writeOutputVoltage(analogOutPin,default_output_V);
  
  //get the current potentiometer value
  initSensorValue = analogRead(analogInPin);
  delay(10);
}

float prev_value = 0.0f;
float learn_fac = 0.25f;
void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin); 

  //compute smoothed version
  float value = (float)sensorValue;
  value = (1.0-learn_fac)*prev_value + learn_fac*value;
  prev_value = value;
  sensorValue = (int)value;
  
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);           

  // print the results to the serial monitor:
//  Serial.print("sensor = " );                       
//  Serial.print(sensorValue);      
//  Serial.print("\t output = ");      
//  Serial.println(outputValue);   

  delay(10);                     
}


