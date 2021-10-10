#define ANALOG_OUT A0 
#define ANALOG_IN_VC A1
#define ANALOG_IN_VR A2
#define ANALOG_IN_GROUND A3

float Vunloaded; 
float Vloaded; 
float Vr;  
float Vground; 
float sum = 0.0; 
float VR_average; 
float counter = 0.0; 
float counter2 = 0.0;
float SumVC; 
float average_VC;  
float Vunload_sum = 0;
float average_Vunload;
float Vtemp; 
float I; 
float R_th; 
int dataPoints; 
int dutyCycle; 
int printCount = 0;

void setup() { 
  // put your setup code here, to run once: 
  analogWriteResolution(10); //  
  analogReadResolution(12); 
  //SerialUSB.begin(9600); // sets the baud rate coming in  
  dataPoints = 0; 
  
} 

void loop() { 
  if(dataPoints > 1000 /*&& printCount < 10 */){
    // turn off the output voltage from the pin 
    analogWrite(ANALOG_OUT, 0);
    
    // calculate the difference in voltage between DUT and Vc (voltage measured from collector) 
    Vtemp = average_Vunload - average_VC;
    
    // calculate the current 
    I = (float)VR_average/100;

    // calculate the value for RTH
    R_th = (float)Vtemp/I;
    SerialUSB.print("This is the voltage diff between loaded and unloaded: ");
    SerialUSB.println(Vtemp, 4);
    SerialUSB.print("This is the loaded voltage: ");
    SerialUSB.println(average_VC, 4);
    SerialUSB.print("This is the unloaded voltage: ");
    SerialUSB.println(Vunloaded, 4);
    SerialUSB.print("This is the current: ");
    SerialUSB.println(I, 4);
    SerialUSB.print("This is RTH: ");
    SerialUSB.println(R_th);
    SerialUSB.print("voltage across the resistor?: ");
    SerialUSB.println(VR_average, 4);
    SerialUSB.println(" ");
    SerialUSB.println(" ");
    //printCount++;
    return;
  }

  // take data points for 21.75ms, analogWrite will be on 7.5ms and after it will turn off
  else if(dataPoints < 500){ 

    // turn on the voltage to the output pin, 1023 is 3.3V, setting this to 3v rn
    analogWrite(ANALOG_OUT, 341);

    // read the voltage coming in from the input pins
    Vr = analogRead(ANALOG_IN_VR) * 3.3 / 4096.0;
    Vground = analogRead(ANALOG_IN_GROUND) * 3.3 / 4096.0;
    Vloaded = analogRead(ANALOG_IN_VC) * 3.3 / 4096.0;

    // voltage across sense resistor
    // the reason why the difference is being taken here is because 
    // the ground on the output pin isn't actually 0, so the difference has to be removed (the voffset)
    float Vdiff = Vr - Vground;  


    // averaging all the voltages measured across the sense resistor and Vc (collector voltage into transistor) from the device under test 
    sum+=Vdiff; 
    counter = counter+1.0; 
    VR_average = (float)sum/counter; 
    SumVC += Vloaded;
    average_VC = (float)SumVC/counter;
    
    SerialUSB.print(0); 
    SerialUSB.print(" "); 
  
    // plotting the voltage diff
    SerialUSB.print(Vdiff * 1000); 
    SerialUSB.print(" "); 
  
    // plotting the average voltage across the resistor, it should be the same as the voltage diff from what it looks like
    SerialUSB.print(VR_average * 1000); 
    SerialUSB.print(" "); 


    // plotting the vc (unaveraged)
    SerialUSB.print(Vloaded /* 1000*/);
    SerialUSB.print(" ");
    
    // plotting the average Vc (voltage from dut)
    SerialUSB.print(average_VC /* 1000*/);
    SerialUSB.print(" ");
  
    // setting upper bound
    SerialUSB.println(5); 
    
    //SerialUSB.println(Vc_dut); 
    
    dataPoints++;
  
}
else{
    // calculate the unloaded voltage 
    analogWrite(ANALOG_OUT, 0);
    counter2 = counter2 + 1.0;
    Vunloaded = analogRead(ANALOG_IN_VC) * 3.3 / 4096.0;
    Vunload_sum += Vunloaded;
    average_Vunload = (float)Vunload_sum/counter2;


    dataPoints++;
}
  

}
  /*
      analogWrite(A0,1023);

      // take data points for 7.5ms, analogWrite will be on for 7.5ms and after it will turn off 
      delay(20);
      Vr = analogRead(ANALOG_IN_VR)* 3.3 / 4096.0; 
      Vground = analogRead(ANALOG_IN_GROUND) * 3.3 / 4096.0;
      Vc_dut = analogRead(ANALOG_IN_VC) * 3.3 / 4096.0;
      //SerialUSB.print("This is ground with write high: ");
      //SerialUSB.println(Vground, 4);
      analogWrite(A0,0);
      //SerialUSB.print("this is ground with write low: ");
      SerialUSB.println(Vground * 1000, 4);
      delay(50); */
/*
  if(dataPoints > 500){
    return;
  }
  else{ 
    delay(9); 
    analogWrite(A0,1023); 

    // for a 10% duty cycle, (the graph takes up to 500 data points so taking 50 data points) 
    // each call to the analogRead function takes 14.5us so that's .725ms on pulse time 
    while( dutyCycle < 50){
      analogRead
    }
    Vc_dut = analogRead(A1)* 3.3 / 4096.0; 
    Vr1 = analogRead(A2)* 3.3 / 4096.0; 
    Vr2 = analogRead(A3)* 3.3 / 4096.0; 
    float Vdiff = Vr1 - Vr2;  
 
    sum+=Vdiff; 
    counter = counter+1.0; 
    average = (float)sum/counter; 
    delay(1); 
    analogWrite(A0,0); 
  
    //reading inputs

    SerialUSB.println(dataPoints); // check
    // printing the data in the serial plotter, only plotting 500 points
    // setting the lower bound, without setting the bounds the y axis scale is way too big
    
    SerialUSB.print(0); 
    SerialUSB.print(" "); 
  
    // plotting the voltage diff
    SerialUSB.print(Vdiff); 
    SerialUSB.print(" "); 
  
    // plotting the average voltage, it should be the same as the voltage diff from what it looks like
    SerialUSB.print(average); 
    SerialUSB.print(" "); 
  
    // setting upper bound
    SerialUSB.println(5); 
    
    //SerialUSB.println(Vc_dut); 
  } */
  














/*
#define aspdofkasdf11_2 A0 // Make code a bit more legible
 
float x = 0; // Value to take the sin of
float increment = 0.02;  // Value to increment x by each time
int frequency = 440; // Frequency of sine wave
 
void setup() 
{
  analogWriteResolution(10); // Set analog out resolution to max, 10-bits
  analogReadResolution(12); // Set analog input resolution to max, 12-bits
  SerialUSB.begin(9600);
}
 
void loop() 
{
  // Generate a voltage value between 0 and 1023. 
  // Let's scale a sin wave between those values:
  // Offset by 511.5, then multiply sin by 511.5.
  int dacVoltage = (int)(511.5 + 511.5 * sin(x));
  x += increment; // Increase value of x
 
  // Generate a voltage between 0 and 3.3V.
  // 0= 0V, 1023=3.3V, 512=1.65V, etc.
  analogWrite(aspdofkasdf11_2, dacVoltage);
 
  // Now read A1 (connected to A0), and convert that
  // 12-bit ADC value to a voltage between 0 and 3.3.
  float voltage = analogRead(A1) * 3.3 / 4096.0;
  SerialUSB.println(voltage); // Print the voltage.
  delay(1); // Delay 1ms
}
*/
