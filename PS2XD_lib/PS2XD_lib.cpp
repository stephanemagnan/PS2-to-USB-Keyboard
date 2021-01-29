#include "PS2XD_lib.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <avr/io.h>
#include "Arduino.h"

//command for reading latest button values
static byte read_status[5] = {0x01,0x42,0,0xFF,0xFF};

/****************************************************************************************/
boolean PS2XD::NewButtonState() {
  //compare bits for each button between latest two states
  return ((last_buttons ^ buttons) > 0);
}

/****************************************************************************************/
boolean PS2XD::NewButtonState(unsigned long int button) {
  //compare bits for each button between latest two states and check if button is clicked
  return (((last_buttons ^ buttons) & button) > 0);
}

/****************************************************************************************/
boolean PS2XD::ButtonPressed(unsigned long int button) {
  return(NewButtonState(button) & Button(button));
}

/****************************************************************************************/
boolean PS2XD::ButtonReleased(unsigned long int button) {
  return((NewButtonState(button)) & ((~last_buttons & button) > 0));
}

/****************************************************************************************/
boolean PS2XD::Button(uint32_t button) {
  return ((~buttons & button) > 0);
}

/****************************************************************************************/
unsigned int PS2XD::ButtonDataByte() {
   return (~buttons);
}

/****************************************************************************************/
unsigned_char_arr PS2XD::_gamepad_shiftinout (char byte) {
   //this function sends and receives data from two controls as 8 bit chars each

   unsigned char tmp1 = 0;
   unsigned char tmp2 = 0;

   //loop through 8 command bits and check for valid return bits.
   for(unsigned char i=0;i<8;i++) {
      if(CHK(byte,i)) CMD_SET();
      else CMD_CLR();
	  
      CLK_CLR();
      delayMicroseconds(CTRL_CLK);

      if(DAT1_CHK()) bitSet(tmp1,i);
      if(DAT2_CHK()) bitSet(tmp2,i);

      CLK_SET();
      //this delay seems to prevent some issues with polling the controller too quickly.
      // if controller returns button states changing when they should be constant
      // try increasing the value of CTRL_CLK_HIGH in PS2XD_lib.h
    #if CTRL_CLK_HIGH
        delayMicroseconds(CTRL_CLK_HIGH);
    #endif
   }
   CMD_SET();
   delayMicroseconds(CTRL_BYTE_DELAY);
   
   struct unsigned_char_arr tmp;
   tmp.char1 = tmp1;
   tmp.char2 = tmp2;

   return tmp;
}


/****************************************************************************************/

bool_arr PS2XD::read_gamepad() {
//this is the frequently called polling wrapper
// every time you want to read button states, call this function
// returns two booleans - valid states for controllers 1 and 2

  CMD_SET();
  CLK_SET();
  ATT_CLR(); 

  delayMicroseconds(CTRL_BYTE_DELAY);

  //send the commands to controller (2 bytes at a time)
  struct unsigned_char_arr tmp_ret;
  //Send the command to get button status;
  for (int i = 0; i<5; i++) {
     tmp_ret = _gamepad_shiftinout(read_status[i]);
     PS2data1[i] = tmp_ret.char1;
     PS2data2[i] = tmp_ret.char2;
  }

  ATT_SET(); 

  //Optional print hex values (i/o) to serial monitor
  // toggle option in PS2XD_lib.h
  #ifdef PS2XD_COM_DEBUG
   Serial.println(" ");
   Serial.print("OUT:IN1:IN2 ");
   for(int i=0; i<5; i++){
      Serial.print(read_status[i], HEX);
      Serial.print(":");
      Serial.print(PS2data1[i], HEX);
      Serial.print(":");
      Serial.print(PS2data2[i], HEX);
      Serial.print(" ");
   }
   Serial.println("");
  #endif

  last_buttons = buttons;

  // each of these are 16 bit values containing the individual button states 
  // of controllers 1 and 2 bit addresses are shown in header file 
  // for example: #define PSB1_SELECT      0x00000001
  btns1 = *(uint16_t*)(PS2data1+3);
  btns2 = *(uint16_t*)(PS2data2+3);

  //create a 32 bit variable containing the 16 states of controller 1 then 
  // the 16 states of controller 2
  buttons = (btns2<<16)+btns1;



// verify the header tags for a valid response
// if the received data had the correct for, return true for valid state  
struct bool_arr bools;
bools.bool1 = ((PS2data1[0] == 0xFF) && (PS2data1[1] == 0x41) && (PS2data1[2] == 0x5A));
bools.bool2 = ((PS2data2[0] == 0xFF) && (PS2data2[1] == 0x41) && (PS2data2[2] == 0x5A));

   return bools;
}

/****************************************************************************************/

byte PS2XD::config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat1, uint8_t dat2) {
  //this function configures the pins required to run both controllers
  // PS2 pins:
  // 1 dat1/dat2
  // 2 command
  // 3 [unused]
  // 4 ground
  // 5 power (3.3V)
  // 6 attention
  // 7 clck
  // 8 [unused]
  // 9 [unused]

  // set masks for pins
  _clk_mask = digitalPinToBitMask(clk);
  _clk_oreg = portOutputRegister(digitalPinToPort(clk));
  _cmd_mask = digitalPinToBitMask(cmd);
  _cmd_oreg = portOutputRegister(digitalPinToPort(cmd));
  _att_mask = digitalPinToBitMask(att);
  _att_oreg = portOutputRegister(digitalPinToPort(att));
  _dat1_mask = digitalPinToBitMask(dat1);
  _dat1_ireg = portInputRegister(digitalPinToPort(dat1));
  _dat2_mask = digitalPinToBitMask(dat2);
  _dat2_ireg = portInputRegister(digitalPinToPort(dat2));


  // configure pins
  pinMode(clk, OUTPUT);
  pinMode(att, OUTPUT);
  pinMode(cmd, OUTPUT);
  pinMode(dat1, INPUT);
  pinMode(dat2, INPUT);
  digitalWrite(dat1, HIGH);
  digitalWrite(dat2, HIGH);

  CMD_SET(); 
  CLK_SET();

  
  return 0; //no error if here
}

/****************************************************************************************/
void PS2XD::sendCommandString(byte string[], byte len) {
  //a more general command sequence in case you do want other commands
  // this includes potentially changing mode to analog and other commands
  // does not return any values. only prints to serial

  struct unsigned_char_arr tmp_out;
  byte temp1[len];
  byte temp2[len];

  ATT_CLR(); // low enable joystick
  delayMicroseconds(CTRL_BYTE_DELAY);

  for (int y=0; y < len; y++){
  tmp_out = _gamepad_shiftinout(string[y]);
  temp1[y] = tmp_out.char1;
  temp2[y] = tmp_out.char2;
}

  ATT_SET(); //high disable joystick
  delay(read_delay); //wait a few

#ifdef PS2XD_COM_DEBUG
  Serial.println("OUT:IN1:IN2 Configure");
  for(int i=0; i<len; i++) {
    Serial.print(string[i], HEX);
    Serial.print(":");
    Serial.print(temp1[i], HEX);
    Serial.print(":");
    Serial.print(temp2[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");  
#endif

}



/****************************************************************************************/

// all functions for sending/receiving indiviual data bits 
// for all channels

inline void  PS2XD::CLK_SET(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_clk_oreg |= _clk_mask;
  SREG = old_sreg;
}

inline void  PS2XD::CLK_CLR(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_clk_oreg &= ~_clk_mask;
  SREG = old_sreg;
}

inline void  PS2XD::CMD_SET(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_cmd_oreg |= _cmd_mask; // SET(*_cmd_oreg,_cmd_mask);
  SREG = old_sreg;
}

inline void  PS2XD::CMD_CLR(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_cmd_oreg &= ~_cmd_mask; // SET(*_cmd_oreg,_cmd_mask);
  SREG = old_sreg;
}

inline void  PS2XD::ATT_SET(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_att_oreg |= _att_mask ;
  SREG = old_sreg;
}

inline void PS2XD::ATT_CLR(void) {
  register uint8_t old_sreg = SREG;
  cli();
  *_att_oreg &= ~_att_mask;
  SREG = old_sreg;
}

inline bool PS2XD::DAT1_CHK(void) {
  return (*_dat1_ireg & _dat1_mask) ? true : false;
}

inline bool PS2XD::DAT2_CHK(void) {
  return (*_dat2_ireg & _dat2_mask) ? true : false;
}

