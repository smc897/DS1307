This is a library for the DS1307 real time clock. It comes with the CPP and H files in the Libraries folder, and a demo application entitled
"test". Here are the instructions for running test:

1) download Arduino here, install in default location: https://www.arduino.cc/en/Main/Software
2) In the Arduino install directory (mine was program files(x86)), find the "libraries" folder. Copy and Paste the "DS1307" folder into it. 
If it gives a permission error, open notepad, copy and paste from there. 
3) run the "test.ino" sketch in the IDE, with the board connected to the computer via USB, and connected to the DS1307 board as follows:
DS1307           Arduino
5v               5v
gnd              gnd
SDA              A4
SCL              A5
4) It should fire up and run at this point. 

