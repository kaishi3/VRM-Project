# VRM-Project
This is the code for a semi-automated VRM (Voltage Regulator Module) characterizer. In the summer of 2021, I breadboarded a fully functional VRM characterizer capable of measuring the internal thevenin resistances of devices with output voltages ranging from 3V to 15V. The significance of this project lies in the fact that the internal resistances of many devices are not labeled in their data sheets. When designing circuits that require high precision, it is necessary to know how the behavior of your device based off its current pull. When pulling varying degrees of current from a device, the internal resistances of all devices do not respond the same (they are nonlinear); this project serves to sheath light on that ambiguity and provide a better understanding of the internal resistances lying in various devices. 

Below feature some images of the finalized circuit. For those who are curious about the components used/concept behind the design, please feel free to message me. 

P.S. The microcontroller used is the Seeeduino Xiao with a SAMD21 microchip.

![IMG_3934](https://user-images.githubusercontent.com/91425318/136706592-d1a9e353-4a54-4534-9d10-da412839c5d5.jpg)
![IMG_3931](https://user-images.githubusercontent.com/91425318/136706596-2588a546-709a-4d72-aca5-e4202352087d.jpg)
