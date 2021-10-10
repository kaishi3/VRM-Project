
 //THIS CODE IS GIVING 49.79 OHMS 
 //*  
#define ANALOG_OUT A0
#define ANALOG_IN_VC A1
#define ANALOG_IN_VR A2
#define ANALOG_IN_GROUND A3

float Vdiff; // voltage between unloaded and loaded
float Vunloaded; // voltage of device under test when the current through the base/gate is 0
float Vloaded;  // voltage of device under test when current through base/gate is non 0
float Vr;  // voltage of sense resistor
float Vground; // ground voltage value in the breadboard
float sum = 0.0;
float VR_average; // average of VR values
float counter;
float counter2;
float SumVload;
float average_Vload;
float Vunload_sum;
float average_Vunload;
float Vtemp;
float I;
float R_th;
float tempLoad, tempUnload;
int dataPoints;
int dutyCycle;
int printCount = 0;
int vOut;
//float Rsens = .9838;
float Rsens = .1952;

void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(10); //
  analogReadResolution(12);
  //SerialUSB.begin(9600); // sets the baud rate coming in
  dataPoints = 0;
  sum = 0;
  SumVload = 0.0;
  Vunload_sum = 0.0;
  counter = 0.0; counter2 = 0.0;
  vOut = 1000; // 2V is minimum to turn mosfet on, this is 2V

}

void loop() {
  analogWrite(ANALOG_OUT, 0);
  // 6 POWER LINE CYCLES, 16MS. with the read function being called 3 times, and 368 data points, that's 16ms. so we're doing 96ms
  delay(1);

  long iTimeOn_usec, iTimeOff_usec;
  float executionTimeOn_usec, executionTimeOff_usec;
  for (int i = 0; i < 2; i++) {

    // timing the on time

    analogWrite(ANALOG_OUT, vOut);
    delay(4);

    iTimeOn_usec = micros();
    while (micros() < 192000 + iTimeOn_usec) { // on for 96ms

      // read the voltage coming in from the input pins
      Vr = analogRead(ANALOG_IN_VR) * 3.3 / 4096.0;
      Vground = analogRead(ANALOG_IN_GROUND) * 3.3 / 4096.0;
      Vloaded = analogRead(ANALOG_IN_VC) * 5.766629 * 3.3 / 4096.0;

      // voltage across sense resistor
      // the reason why the difference is being taken here is because
      // the ground on the output pin isn't actually 0, so the difference has to be removed (the voffset)
      float Vdiff = Vr - Vground;


      // averaging all the voltages measured across the sense resistor and Vc (collector voltage into transistor) from the device under test
      sum += Vdiff;
      counter = counter + 1.0;
      SumVload += Vloaded;
      dataPoints++;

    }
    executionTimeOn_usec = (micros() - iTimeOn_usec);

    // calculate the unloaded voltage
    analogWrite(ANALOG_OUT, 0);
    delay(4);

    iTimeOff_usec = micros();
    while (micros() < 1728000 + iTimeOff_usec) { // off for 96ms
      Vunloaded = analogRead(ANALOG_IN_VC) * 5.766629 * 3.3 / 4096.0;
      counter2 = counter2 + 1.0;
      Vunload_sum += Vunloaded;

    }
    executionTimeOff_usec = (micros() - iTimeOff_usec);

  }
  /*
  if (vOut < 1000) {
    vOut = vOut + 10;
  } */

  VR_average = (float)sum / (counter + 1);
  average_Vload = (float)SumVload / (counter + 1);
  sum = VR_average;
  SumVload = average_Vload;
  counter = 1;

  average_Vunload = (float)Vunload_sum / (counter2 + 1);
  Vunload_sum = average_Vunload;
  counter2 = 1;



  // calculate the difference in voltage between DUT and Vc (voltage measured from collector)
  Vtemp = average_Vunload - average_Vload;

  // calculate the current
  I = (float)VR_average / Rsens; // using a 100 ohm resistor, but it's actually 98.4 ohms

  // calculate the value for RTH
  R_th = (float)Vtemp / I;
  //}


  // print all the values
  SerialUSB.println(executionTimeOn_usec, 3);
  SerialUSB.println(executionTimeOff_usec, 3);
  SerialUSB.print("This is the voltage diff between loaded and unloaded: ");
  SerialUSB.println(Vtemp, 4);
  SerialUSB.print("This is the loaded voltage: ");
  SerialUSB.println(average_Vload, 4);
  SerialUSB.print("This is the unloaded voltage: ");
  SerialUSB.println(average_Vunload, 4);
  SerialUSB.print("This is the current: ");
  SerialUSB.println(I, 4);
  SerialUSB.print("This is RTH: ");
  SerialUSB.println(R_th);
  SerialUSB.print("voltage across the resistor?: ");
  SerialUSB.println(VR_average, 4);
  SerialUSB.println(" ");
  SerialUSB.println(" ");

  delay(2);

} 
 
