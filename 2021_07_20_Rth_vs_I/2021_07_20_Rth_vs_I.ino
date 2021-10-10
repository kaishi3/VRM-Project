// changing current and plotting against rth
// old resistor 98.4 ohms. placed in ad2 box
// new resistor .9838 ohms
// 

// mosfet is pretty hot, might want to lessen the time that data is being taken for

// 2021.july.21 - AGENDA: 
// * lessen the time it takes to generate resistance vs current plot - done july.21
// * control heat of mosfet - done july.21
// * only read from resistor if a certain voltage is going through it - done july.21
// analogWrite(ANALOG_OUT, 580) - .0038 current across resistor?
// try to keep power of mosfet below one watt. if want to reduce heating, might need to lower duty cycle/averaging time 
// if voltage across the resistor is too low, the xiao can't get accurate measurements (+-1 mV)

// steady voltage measurements
// 560, fluctuating a lot. around 1.7 mV
// 580, 3mV. still decent fluctuation
// 600, 6.1-6.2mV. still fluctuation
// 620, 12.7mV, still fluctuation.. around .2Rth - .4 rth? for function gen 
// 640, 26.6 mV, less fluctation, around .15 rTh

// pulsing
// 640, ~27mV across resistor, ~44 Rth
// 620, ~14mV across resistor, ~44 Rth
// 600, 7mV across resistor, ~44 Rth.... weird
// 580, 3mV across resistor, fluctuating 44-46Rth
// 560, 1.6mV across resistor, 44-50 Rth
// i guess i just start at 600. 

// Note about IRFZ34N // SWAPPED FOR IRL520N
// pretty sure i screwed the mosfet up by overheating it, it doesn't produce right results anymore
// tested against working code (used several times before) and didn't get same results. then switched out mosfet and got results i was expecting.
// now using IRL520N says min gate threshold 1V - 2V but doesn't seem to be the case 
// 500, around .02mV across resistor
// 550, around .06mV
// 600, around 1mV
// 650, around 5 mV
// 1010, 62mA of current, 62mV. around 51.3 ohms Rth for fun gen 


// 2021.July.22 
// AGENDA:
// * test more devices with program to see their Rth vs I curves. - still in progress
// * check current curves with prof bogatin - checked. going to try plotting the current/resistance of 9V supply with .2ohm resistor. need to keep duty cycle low to stop mosfet heating. 
// * ^ do above in excel
// * test sonal 555 timer 
// * see if can plot value of current on x axis - print to serial monitor, put into csv 
// * 


// 2021.July.25
// AGENDA:
// * Test 9V Supply (Rth vs I) - done 
// * Plot in excel - done
// * i think the .2ohm i have is around .195ohms? will try that value - done
// currently on for 96000, off for 2304000 (this is a 4% duty cycle. going to try 12V supplies now 

// 2021.July.27
// AGENDA:
// * Plot voltage on graph as well - done
// * Build transistor follower (5v into base of transistor, 1k ohms at emitter, read output from the emitter) 
// * need to get transistor, dunno where they are. done - in breadboard
// * test more power supplies


// 2021.July.28
// AGENDA:
// * test more power supplies
// * take pictures
// * organize presentation slides a bit

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
float Rsens = .195; // using this for .2ohm resistor to pull more current
int cycler = 0;

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
  vOut = 645; // 2V is minimum to turn mosfet on, this is 2V
  delay(5000);

}
 
void loop() {
  analogWrite(ANALOG_OUT, 0);
  //analogWrite(ANALOG_OUT, 0);
  // 6 POWER LINE CYCLES, 16MS. with the read function being called 3 times, and 368 data points, that's 16ms. so we're doing 96ms
  delay(1);

  long iTimeOn_usec, iTimeOff_usec;
  float executionTimeOn_usec, executionTimeOff_usec;
  //for (int i = 0; i < 2; i++) {

    // timing the on time
    //analogWrite(ANALOG_OUT, 1000);
    analogWrite(ANALOG_OUT, vOut);
    delay(4);

    iTimeOn_usec = micros();
    while (micros() < 96000 + iTimeOn_usec) { // on for 96ms. temporarily trying about 192ms (about .2s) 

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


    analogWrite(ANALOG_OUT, 0);
    // calculate the unloaded voltage
    //analogWrite(ANALOG_OUT, 0);
    delay(4);

    iTimeOff_usec = micros();
    while (micros() < 2304000 + iTimeOff_usec) { // off for 96ms. trying off for 1.7 seconds 
      Vunloaded = analogRead(ANALOG_IN_VC) * 5.766629 * 3.3 / 4096.0;
      counter2 = counter2 + 1.0;
      Vunload_sum += Vunloaded;

    }
    executionTimeOff_usec = (micros() - iTimeOff_usec);

  //}

  /*
  if (vOut < 1000 && (cycler % 2 == 0)) {
    vOut = vOut + 2;
    if(vOut >= 1000){
      cycler++;
    }
  } 
  if(vOut > 650 && (cycler % 2 == 1)){
    vOut = vOut - 2;
    if(vOut <= 650){
      cycler++;
    }
  } */ 

  
  if(vOut < 1000 && cycler != 1){
    vOut = vOut + 2;
  }
  if(vOut > 1000){
    cycler = 1;
    vOut = 0; // turning off all voltage from the dac
  }

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


  // plotting r vs I in serial monitor to see in sheets
  if(vOut < 1000 && cycler != 1){
    SerialUSB.print(I, 5);
    SerialUSB.print(",");  
    SerialUSB.print(R_th);   
    SerialUSB.print(",");
    SerialUSB.println(average_Vload);
  } 
  if(vOut == 0 && cycler == 1){
    SerialUSB.println("EXIT");
  }

  /*
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

  delay(2); */

} 
