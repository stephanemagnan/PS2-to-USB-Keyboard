/******************************************************************
*  Super amazing PS2 controller Arduino Library v1.8
*		details and example sketch: 
*			http://www.billporter.info/?p=240
*
*    Original code by Shutter on Arduino Forums
*
*    Revamped, made into lib by and supporting continued development:
*              Bill Porter
*              www.billporter.info
*
*	 Contributers:
*		Eric Wetzel (thewetzel@gmail.com)
*		Kurt Eckhardt
*
*  Lib version history
*
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*  
******************************************************************/

// $$$$$$$$$$$$ DEBUG ENABLE SECTION $$$$$$$$$$$$$$$$
// to debug ps2 controller, uncomment these two lines to print out debug to uart
#define PS2XD_DEBUG
#define PS2XD_COM_DEBUG


#ifndef PS2XD_lib_h
  #define PS2XD_lib_h

#include "Arduino.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string>


// 20 and 10 worked for sony controllers, but only RHS
// these delays need to be tuned for ?every? controller used
  #define CTRL_CLK        20
  #define CTRL_BYTE_DELAY 10
  #define CTRL_CLK_HIGH   5

//These are our button constants
// correspond to addresses in "buttons" and "last_buttons" where
// each button's state is saved as one bit
#define PSB1_SELECT      0x00000001
#define PSB1_L3          0x00000002
#define PSB1_R3          0x00000004
#define PSB1_START       0x00000008
#define PSB1_PAD_UP      0x00000010
#define PSB1_PAD_RIGHT   0x00000020
#define PSB1_PAD_DOWN    0x00000040
#define PSB1_PAD_LEFT    0x00000080
#define PSB1_L2          0x00000100
#define PSB1_R2          0x00000200
#define PSB1_L1          0x00000400
#define PSB1_R1          0x00000800
#define PSB1_TRIANGLE    0x00001000
#define PSB1_CIRCLE      0x00002000
#define PSB1_CROSS       0x00004000
#define PSB1_SQUARE      0x00008000

#define PSB2_SELECT      0x00010000
#define PSB2_L3          0x00020000
#define PSB2_R3          0x00040000
#define PSB2_START       0x00080000
#define PSB2_PAD_UP      0x00100000
#define PSB2_PAD_RIGHT   0x00200000
#define PSB2_PAD_DOWN    0x00400000
#define PSB2_PAD_LEFT    0x00800000
#define PSB2_L2          0x01000000
#define PSB2_R2          0x02000000
#define PSB2_L1          0x04000000
#define PSB2_R1          0x08000000
#define PSB2_TRIANGLE    0x10000000
#define PSB2_CIRCLE      0x20000000
#define PSB2_CROSS       0x40000000
#define PSB2_SQUARE      0x80000000


#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))


//create structure required to return one char for each of two controllers
struct unsigned_char_arr {
    unsigned char char1;
    unsigned char char2;
};
//create structure required to return one bool for each of two controllers
  struct bool_arr {
    boolean bool1;
    boolean bool2;
};


class PS2XD {
  public:
    boolean Button(uint32_t);                //will be TRUE if button is being pressed
    unsigned int ButtonDataByte();
    boolean NewButtonState();
    boolean NewButtonState(unsigned long int);    //will be TRUE if button was JUST pressed OR released
    boolean ButtonPressed(unsigned long int);     //will be TRUE if button was JUST pressed
    boolean ButtonReleased(unsigned long int);    //will be TRUE if button was JUST released

    //functions defined in lib's .cpp file
    bool_arr read_gamepad();
    byte readType();
    byte config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void reconfig_gamepad();

  private:
    inline void CLK_SET(void);
    inline void CLK_CLR(void);
    inline void CMD_SET(void);
    inline void CMD_CLR(void);
    inline void ATT_SET(void);
    inline void ATT_CLR(void);
    inline bool DAT1_CHK(void);
    inline bool DAT2_CHK(void);
    

    // functions and variables for storing button states and querying controllers
    unsigned_char_arr _gamepad_shiftinout (char);
    unsigned char PS2data1[5];
    unsigned char PS2data2[5];
    void sendCommandString(byte*, byte);
    unsigned char i;
    unsigned int btns1;
    unsigned int btns2;
    unsigned long int last_buttons;
    unsigned long int buttons;
	

    // masks for data
    uint8_t maskToBitNum(uint8_t);
    uint8_t _clk_mask; 
    volatile uint8_t *_clk_oreg;
    uint8_t _cmd_mask; 
    volatile uint8_t *_cmd_oreg;
    uint8_t _att_mask; 
    volatile uint8_t *_att_oreg;
    uint8_t _dat1_mask; 
    volatile uint8_t *_dat1_ireg;
    uint8_t _dat2_mask; 
    volatile uint8_t *_dat2_ireg;

    byte read_delay;
};

#endif



