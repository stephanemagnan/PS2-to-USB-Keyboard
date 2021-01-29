# PS2XD-lib
PS2 Controller (digital mode) to USB Keyboard using a microcontroller [compatible with two simultaneous PS2 controllers]
Version 0.1

The purpose of this project was to develop a PS2 controller to computer keyboard input converter with primary use of connecting two PS2 Dance Dance Revolution (DDR) dance mats to a computer (mac or PC) to play StepMania (open source version of DDR).  Digital buttons on the  PS2 controller are mapped to customizable keyboard inputs. In StepMania, the user can assign these key presses to specific inputs (i.e., W_KEY interpretted as P1 UP). This library also works with some PS2 controllers to read button presses as digital inputs, but I had limited success when using Official Sony PS2 controllers. 

/****************** REFRENCES AND ORIGINAL SOURCE CODE ******************/

This library is a modified version of the Arduino PS2x (v1.8) library (https://github.com/madsci1016/Arduino-PS2X) with the following major changes:
- Exclusively supports digital mode in the controllers (controllers default to digital when plugged in)
- Allows two controllers to be used simultaneously
- Removed all compatibility functions (did not have ability to test these functions).

This project would not have happened without the hard work done by several key contributors (hopefully forgot no one).
- Arduino PS2x library creators http://www.billporter.info/?p=240
  - Original code by Shutter on Arduino Forums
  - Bill Proter (Revamped, made into lib by and supporting continued development) www.billporter.info
  - Eric Wetzel (Contributor)
  - Kurt Eckhardt (Contributor)
- I consulted these websites more times than I could count. (and others)
  - https://github.com/madsci1016/Arduino-PS2X
  - http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/comment-page-7/#comments
  - http://www.billporter.info/2011/03/27/arduino-playstation-2-controller-library-troubleshooting-guide/comment-page-1/#comments
  - https://electropeak.com/learn/tutorial-interface-wireless-playstation-ps2-controller-arduino/#:~:text=PS2%20Controllers%20Features,-Everyone%20knows%20Sony&text=The%20wireless%20controller%20works%20with,a%20range%20of%2010%20meters
  - https://gamesx.com/controldata/psxcont/psxcont.htm
  - https://blog.nearfuturelaboratory.com/2008/06/19/playstation2-logic-analysis/
  - http://www.emulatronia.com/reportajes/directpad/psxeng/index.htm
  - http://www.emulatronia.com/reportajes/directpad/psxeng/circuit.htm
  - http://www.emulatronia.com/reportajes/directpad/psxeng/index.htm 

/****************** License ******************/

GNU General Public License - see License file.

/****************** REQUIRED COMPONENTS ******************/

- Teensy 3.6 Microcontroller (can be bought with pins already soldred if you don't have soldering equipment)
  - Other boards may also work (no guarantees, I only had this one to play with)
  - Recommended 3.3V logic level, people suggest PS2 controllers can run on 5V but I didn't try it. (Try 5V at your own risk).
  - Ability to be recognized as keyboard (Ardino Uno probably not compatible from what I read).
- PS2 breakout connections (1 per controller)
  - I bought this one: https://www.robotshop.com/ca/en/cytron-breakout-board-ps2-connector.html
  - Solderless version: https://www.robotshop.com/ca/en/ps2-connector-breakout-board.html
- USB cable
- 5V power supply 
  - either with appropriate connectors to make it breadboard friendly
  - or cut off the ends and solder directly
- Solderless breadboard and jumper wires for prototype version
- Prototype PCB for a more finished version
- Soldering iron (unless components are purchased with pins already in place)

/****************** COMPATIBLE CONTROLLERS ******************/

Tested with the following controllers:
- Official Sony PS2 controllers
  - mixed success, mostly poor. Usually only half of controller worked
- NEX LUX PS2 Controller
  - worked well, analog joysticks mapped to digital buttons as well
- KONAMI PS2 DDR mat
  - worked well, my mats had problems with the select key, so I stopped reading those values and assigned its functionality to the O button.

/****************** WIRING DIAGRAM ******************/

/ *** WIRING DIAGRAM: TEENSY 3.6 (with external power supply) ***/

Notes: 
- Teensy 3.6 is a 3.3V logic microcontroller. 5V power can be provided to the board's Vin pin, but Vusb to Vin connection on the back of the board must be severed.
- you may not need an external power supply, but I had one set up wo I was using it. I did not test without. Your computer's USB port may provide sufficient amperage.

Wiring:
- PIN gnd TO: pin 4 [gnd] on ps2 controllers 1 and 2, power supply ground
- PIN 23 TO: pin 2 [command] on ps2 controllers 1 and 2
- PIN 22 TO: pin 1 [data] on ps2 controller 1
- PIN 21 TO: pin 2 [data] on ps2 controller 2 [connect to 3.3V if only using one controller]
- PIN 29 TO: pin 6 [attention] on ps2 controllers 1 and 2
- PIN 30 TO: pin 7 [clock] on ps2 controllers 1 and 2
- PIN Vin TO: 5V external supply
- PIN 3.3V TO: pin 5 [power] on ps2 controllers 1 and 2

/****************** INSTALLATION INSTRUCTIONS *****************/

You need:
- Arduino IDE
  - https://www.arduino.cc/en/software
- Teensyduino IDE 
  - https://www.pjrc.com/teensy/teensyduino.html

To install, unzip and place 'PS2xd_lib' folder into your Arduino IDE library folder.
Restart the Arduino IDE, and open up the example sketch. 

When running, you need to select a communication mode that includes Serial (if you want anything printed to the Serial Monitor), and Keyboard (if using any keyboard functions).


/****************** DEBUGGING INSTRUCTIONS ******************/

There are a few options for you if you want to debug the software or are having trouble with the library.

1. Although it has been modified heavily, the comments and discussions from the original Arduino PS2x_lib really helped me figure out this project (lots of pages of comments):
  - http://www.billporter.info/2011/03/27/arduino-playstation-2-controller-library-troubleshooting-guide/comment-page-1/#comments

2. Enable internal status to Serial Monitor output (controller signals sent and received) from the PS2xd_lib.h file
Change: 

//#define PS2XD_DEBUG

//#define PS2XD_COM_DEBUG

To:

#define PS2XD_DEBUG

#define PS2XD_COM_DEBUG


Note: the program always sends the command string: 

BTYE    SENT  RECEIVED

 01     0x01    0xFF
 
 02     0x42    0x41
 
 03     0xFF    0x5A   
 
 04     0xFF    DATA1   
 
 05     0xFF    DATA2   
 
where: 

Bit num     Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
        
DATA1   SLCT  L3   R3  STRT UP   RGHT DOWN LEFT

DATA     L2   R2    L1  R1   /\   O    X    []

3. Run the "PS2_read_verbose" example sketch that enables Serial Monitor output of interpreted key strokes (such as Start, Select, etc.)
  This code has no keyboard functions enables, so you can run it as a Serial type from the Arduino IDE.
  This example basically converts the hex output from "PS2XD_DEBUG" to more easily interpreted values.

4. Some controllers have specific buttons that do not work well (or are registered as repeated presses). You may have to comment out specific keystrokes.
for example, I had to comment out this line:

  ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_select,    PS2XD1_KEY_SELECT,      ps2xd1_keys_pressed, 0);

  by changing it to:

  //ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_select,    PS2XD1_KEY_SELECT,      ps2xd1_keys_pressed, 0);

  because my DDR Pad select key was sending keystrokes when it wasn't being pressed.

5. The timing of commands sent to the PS2 controller seems to be a really big factor. The following lines in the PS2xd_lib.h file may need to be tweaked for your controller.

  #define CTRL_CLK        20
  
  #define CTRL_BYTE_DELAY 10
  
  #define CTRL_CLK_HIGH   5
  
  If you find other values that work better, let me know and I will update this page.


/****************** I wish you good luck and much success, but above all: have fun! ******************/
