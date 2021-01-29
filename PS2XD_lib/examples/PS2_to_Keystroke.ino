#include <Keyboard.h>
#include <PS2XD_lib.h>

// *** DEBUG OPTIONS *** //
boolean print_buttons_out = true; // independent of debugging tag in lib
//boolean print_buttons_out = false; // independent of debugging tag in lib
int delay_time = 10;
int send_keystrokes = true; //for intended use
//int send_keystrokes = false; //used for debugging without wreaking havoc on editor

// *** *** SET UP MICRO-CONTROLLER CIRCUIT *** *** //
// PINS ARE ON TEENSY 3.6 BOARD, ALL I/O AT 3.3V LOGIC
#define PS2XD_PIN_DAT1    22 // to PS2 PIN 1    // CONTROLLER 1 EXCLUSIVELY  
#define PS2XD_PIN_DAT2    21 // to PS2 PIN 1    // CONTROLLER 2 EXCLUSIVELY // CONNECT TO 3.3V IF UNUSED 
#define PS2XD_PIN_CMD     23 // to PS PIN 2     // COMMON SIGNAL SHARED BETWEEN TWO CONTROLLERS
#define PS2XD_PIN_ATT     29 // to PS PIN 6  // COMMON SIGNAL SHARED BETWEEN TWO CONTROLLERS
#define PS2XD_PIN_CLK     30 // to PS PIN 7  // COMMON SIGNAL SHARED BETWEEN TWO CONTROLLERS

// CONFIGURE CONTROLLER BUTTON EQUIVALENT KEYBOARD INPUTS
// CAN BE CUSTOMIZED USING THESE VALUES
// https://www.pjrc.com/teensy/td_keyboard.html

#define PS2XD1_KEY_SELECT      KEY_1
#define PS2XD1_KEY_L3          KEY_V
#define PS2XD1_KEY_R3          KEY_B
#define PS2XD1_KEY_START       KEY_3
#define PS2XD1_KEY_PAD_UP      KEY_W
#define PS2XD1_KEY_PAD_RIGHT   KEY_D
#define PS2XD1_KEY_PAD_DOWN    KEY_X
#define PS2XD1_KEY_PAD_LEFT    KEY_A
#define PS2XD1_KEY_L2          KEY_F
#define PS2XD1_KEY_R2          KEY_G
#define PS2XD1_KEY_L1          KEY_T
#define PS2XD1_KEY_R1          KEY_R
#define PS2XD1_KEY_TRIANGLE    KEY_C
#define PS2XD1_KEY_CIRCLE      KEY_Q
#define PS2XD1_KEY_CROSS       KEY_E
#define PS2XD1_KEY_SQUARE      KEY_Z

#define PS2XD2_KEY_SELECT      KEY_6
#define PS2XD2_KEY_L3          KEY_PERIOD
#define PS2XD2_KEY_R3          KEY_SLASH
#define PS2XD2_KEY_START       KEY_8
#define PS2XD2_KEY_PAD_UP      KEY_U
#define PS2XD2_KEY_PAD_RIGHT   KEY_K
#define PS2XD2_KEY_PAD_DOWN    KEY_M
#define PS2XD2_KEY_PAD_LEFT    KEY_H
#define PS2XD2_KEY_L2          KEY_L
#define PS2XD2_KEY_R2          KEY_SEMICOLON
#define PS2XD2_KEY_L1          KEY_O
#define PS2XD2_KEY_R1          KEY_P
#define PS2XD2_KEY_TRIANGLE    KEY_COMMA
#define PS2XD2_KEY_CIRCLE      KEY_T
#define PS2XD2_KEY_CROSS       KEY_Y
#define PS2XD2_KEY_SQUARE      KEY_N


// *** *** SET UP STATE TRACKING FOR EACH INPUT *** *** //
boolean ps2xd1_state_select = false;
boolean ps2xd1_state_l3 = false;
boolean ps2xd1_state_r3 = false;
boolean ps2xd1_state_start = false;
boolean ps2xd1_state_pad_up = false;
boolean ps2xd1_state_pad_right = false;
boolean ps2xd1_state_pad_down = false;
boolean ps2xd1_state_pad_left = false;
boolean ps2xd1_state_l2 = false;
boolean ps2xd1_state_r2 = false;
boolean ps2xd1_state_l1 = false;
boolean ps2xd1_state_r1 = false;
boolean ps2xd1_state_triangle = false;
boolean ps2xd1_state_circle = false;
boolean ps2xd1_state_cross = false;
boolean ps2xd1_state_square = false;

boolean ps2xd2_state_select = false;
boolean ps2xd2_state_l3 = false;
boolean ps2xd2_state_r3 = false;
boolean ps2xd2_state_start = false;
boolean ps2xd2_state_pad_up = false;
boolean ps2xd2_state_pad_right = false;
boolean ps2xd2_state_pad_down = false;
boolean ps2xd2_state_pad_left = false;
boolean ps2xd2_state_l2 = false;
boolean ps2xd2_state_r2 = false;
boolean ps2xd2_state_l1 = false;
boolean ps2xd2_state_r1 = false;
boolean ps2xd2_state_triangle = false;
boolean ps2xd2_state_circle = false;
boolean ps2xd2_state_cross = false;
boolean ps2xd2_state_square = false;


// *** *** SET UP PS2 CONTROLLER *** *** //
PS2XD ps2xd;
bool_arr ps2xd_valid_reads;
int ps2xd1_keys_pressed;
int ps2xd2_keys_pressed;


// *** *** INITIALIZATION *** *** //
  void setup() {
  // initialize serial communication
  Serial.begin(9600);
  // delay before starting
  delay(3000);
  
  //initialize pins and settings for controller
  ps2xd.config_gamepad(PS2XD_PIN_CLK, PS2XD_PIN_CMD, PS2XD_PIN_ATT, PS2XD_PIN_DAT1, PS2XD_PIN_DAT2);
}


// *** *** EXECUTION LOOP *** *** //
void loop() {

//prevent keyboard event overflow by delaying loop time  
delay(delay_time);

//read the game pad
ps2xd_valid_reads = ps2xd.read_gamepad();

  //if the result is valid, parse buttons
  if (ps2xd_valid_reads.bool1){
    //check if any of the buttons returned a positive press state
     ps2xd1_state_select = ps2xd.Button(PSB1_SELECT);
     ps2xd1_state_l3 = ps2xd.Button(PSB1_L3);
     ps2xd1_state_r3 = ps2xd.Button(PSB1_R3);
     ps2xd1_state_start = ps2xd.Button(PSB1_START);
     ps2xd1_state_pad_up = ps2xd.Button(PSB1_PAD_UP);
     ps2xd1_state_pad_right = ps2xd.Button(PSB1_PAD_RIGHT);
     ps2xd1_state_pad_down = ps2xd.Button(PSB1_PAD_DOWN);
     ps2xd1_state_pad_left = ps2xd.Button(PSB1_PAD_LEFT);
     ps2xd1_state_l2 = ps2xd.Button(PSB1_L2);
     ps2xd1_state_r2 = ps2xd.Button(PSB1_R2);
     ps2xd1_state_l1 = ps2xd.Button(PSB1_L1);
     ps2xd1_state_r1 = ps2xd.Button(PSB1_R1);
     ps2xd1_state_triangle = ps2xd.Button(PSB1_TRIANGLE);
     ps2xd1_state_circle = ps2xd.Button(PSB1_CIRCLE);
     ps2xd1_state_cross = ps2xd.Button(PSB1_CROSS);
     ps2xd1_state_square = ps2xd.Button(PSB1_SQUARE);
  }
  else{
    //reset button states
     ps2xd1_state_select = false;
     ps2xd1_state_l3 = false;
     ps2xd1_state_r3 = false;
     ps2xd1_state_start = false;
     ps2xd1_state_pad_up = false;
     ps2xd1_state_pad_right = false;
     ps2xd1_state_pad_down = false;
     ps2xd1_state_pad_left = false;
     ps2xd1_state_l2 = false;
     ps2xd1_state_r2 = false;
     ps2xd1_state_l1 = false;
     ps2xd1_state_r1 = false;
     ps2xd1_state_triangle = false;
     ps2xd1_state_circle = false;
     ps2xd1_state_cross = false;
     ps2xd1_state_square = false;
  }

  //if the result is valid, parse buttons
  if (ps2xd_valid_reads.bool2){
    //check if any of the buttons returned a positive press state
     ps2xd2_state_select = ps2xd.Button(PSB2_SELECT);
     ps2xd2_state_l3 = ps2xd.Button(PSB2_L3);
     ps2xd2_state_r3 = ps2xd.Button(PSB2_R3);
     ps2xd2_state_start = ps2xd.Button(PSB2_START);
     ps2xd2_state_pad_up = ps2xd.Button(PSB2_PAD_UP);
     ps2xd2_state_pad_right = ps2xd.Button(PSB2_PAD_RIGHT);
     ps2xd2_state_pad_down = ps2xd.Button(PSB2_PAD_DOWN);
     ps2xd2_state_pad_left = ps2xd.Button(PSB2_PAD_LEFT);
     ps2xd2_state_l2 = ps2xd.Button(PSB2_L2);
     ps2xd2_state_r2 = ps2xd.Button(PSB2_R2);
     ps2xd2_state_l1 = ps2xd.Button(PSB2_L1);
     ps2xd2_state_r1 = ps2xd.Button(PSB2_R1);
     ps2xd2_state_triangle = ps2xd.Button(PSB2_TRIANGLE);
     ps2xd2_state_circle = ps2xd.Button(PSB2_CIRCLE);
     ps2xd2_state_cross = ps2xd.Button(PSB2_CROSS);
     ps2xd2_state_square = ps2xd.Button(PSB2_SQUARE);
  }
  else{
    //reset button states
     ps2xd2_state_select = false;
     ps2xd2_state_l3 = false;
     ps2xd2_state_r3 = false;
     ps2xd2_state_start = false;
     ps2xd2_state_pad_up = false;
     ps2xd2_state_pad_right = false;
     ps2xd2_state_pad_down = false;
     ps2xd2_state_pad_left = false;
     ps2xd2_state_l2 = false;
     ps2xd2_state_r2 = false;
     ps2xd2_state_l1 = false;
     ps2xd2_state_r1 = false;
     ps2xd2_state_triangle = false;
     ps2xd2_state_circle = false;
     ps2xd2_state_cross = false;
     ps2xd2_state_square = false;
  }

 if(print_buttons_out){
  Serial.println("");
    printKeyStateString(ps2xd1_state_select,"SEL");
    printKeyStateString(ps2xd1_state_l3,"L3");
    printKeyStateString(ps2xd1_state_r3,"R3");
    printKeyStateString(ps2xd1_state_start,"STR");
    printKeyStateString(ps2xd1_state_pad_up,"U");
    printKeyStateString(ps2xd1_state_pad_right,"R");
    printKeyStateString(ps2xd1_state_pad_down,"D");
    printKeyStateString(ps2xd1_state_pad_left,"L");
    printKeyStateString(ps2xd1_state_l2,"L2");
    printKeyStateString(ps2xd1_state_r2,"R2");
    printKeyStateString(ps2xd1_state_l1,"L1");
    printKeyStateString(ps2xd1_state_r1,"R1");
    printKeyStateString(ps2xd1_state_triangle,"/\\");
    printKeyStateString(ps2xd1_state_circle,"O");
    printKeyStateString(ps2xd1_state_cross,"X");
    printKeyStateString(ps2xd1_state_square,"[]");
    Serial.print("  :  ");
    printKeyStateString(ps2xd2_state_select,"SEL");
    printKeyStateString(ps2xd2_state_l3,"L3");
    printKeyStateString(ps2xd2_state_r3,"R3");
    printKeyStateString(ps2xd2_state_start,"STR");
    printKeyStateString(ps2xd2_state_pad_up,"U");
    printKeyStateString(ps2xd2_state_pad_right,"R");
    printKeyStateString(ps2xd2_state_pad_down,"D");
    printKeyStateString(ps2xd2_state_pad_left,"L");
    printKeyStateString(ps2xd2_state_l2,"L2");
    printKeyStateString(ps2xd2_state_r2,"R2");
    printKeyStateString(ps2xd2_state_l1,"L1");
    printKeyStateString(ps2xd2_state_r1,"R1");
    printKeyStateString(ps2xd2_state_triangle,"/\\");
    printKeyStateString(ps2xd2_state_circle,"O");
    printKeyStateString(ps2xd2_state_cross,"X");
    printKeyStateString(ps2xd2_state_square,"[]");
    
  Serial.println("");
 }



  
  if(send_keystrokes){
    //can only send three keystrokes per controller without buffer
    //could be modified for 6 keys from one controller
    ps2xd1_keys_pressed = 0;
    ps2xd2_keys_pressed = 0;

    //reset keystrokes
    Keyboard.set_key1(0);
    Keyboard.set_key2(0);
    Keyboard.set_key3(0);
    Keyboard.set_key4(0);
    Keyboard.set_key5(0);
    Keyboard.set_key6(0);

    // loop through all keys and determine first three to add to send command
    //ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_select,    PS2XD1_KEY_SELECT,      ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_l3,        PS2XD1_KEY_L3,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_r3,        PS2XD1_KEY_R3,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_start,     PS2XD1_KEY_START,       ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_pad_up,    PS2XD1_KEY_PAD_UP,      ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_pad_right, PS2XD1_KEY_PAD_RIGHT,   ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_pad_down,  PS2XD1_KEY_PAD_DOWN,    ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_pad_left,  PS2XD1_KEY_PAD_LEFT,    ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_l2,        PS2XD1_KEY_L2,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_r2,        PS2XD1_KEY_R2,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_l1,        PS2XD1_KEY_L1,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_r1,        PS2XD1_KEY_R1,          ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_triangle,  PS2XD1_KEY_TRIANGLE,    ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_circle,    PS2XD1_KEY_CIRCLE,      ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_cross,     PS2XD1_KEY_CROSS,       ps2xd1_keys_pressed, 0);
    ps2xd1_keys_pressed = assignKeyboardKey(ps2xd1_state_square,    PS2XD1_KEY_SQUARE,      ps2xd1_keys_pressed, 0);

    //ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_select,    PS2XD2_KEY_SELECT,      ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_l3,        PS2XD2_KEY_L3,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_r3,        PS2XD2_KEY_R3,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_start,     PS2XD2_KEY_START,       ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_pad_up,    PS2XD2_KEY_PAD_UP,      ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_pad_right, PS2XD2_KEY_PAD_RIGHT,   ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_pad_down,  PS2XD2_KEY_PAD_DOWN,    ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_pad_left,  PS2XD2_KEY_PAD_LEFT,    ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_l2,        PS2XD2_KEY_L2,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_r2,        PS2XD2_KEY_R2,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_l1,        PS2XD2_KEY_L1,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_r1,        PS2XD2_KEY_R1,          ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_triangle,  PS2XD2_KEY_TRIANGLE,    ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_circle,    PS2XD2_KEY_CIRCLE,      ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_cross,     PS2XD2_KEY_CROSS,       ps2xd2_keys_pressed, 1);
    ps2xd2_keys_pressed = assignKeyboardKey(ps2xd2_state_square,    PS2XD2_KEY_SQUARE,      ps2xd2_keys_pressed, 1);
    
    //command has been built, send now
    Keyboard.send_now();
    delay(2);
  }
  

}
 void printKeyStateString(boolean btnState, String btnTag){
  //function to print button states to console
  if(btnState){
    Serial.print("[");
    Serial.print(btnTag);
    Serial.print("]");
  }
  else{
   Serial.print(" ");
   Serial.print(btnTag);
   Serial.print(" ");
  }
 }

 int assignKeyboardKey(boolean btnState,int KEY_CODE, int buttons_pressed, int controller_code){
    // if the button is pressed, add keystroke to next send. 
    // Max three buttons per controller.
    // controller 0 (keys 1,2,3), else (keys 4,5,6)
    if(btnState){
        switch (buttons_pressed){
          case 0:
            if(controller_code == 0){
              Keyboard.set_key1(KEY_CODE);
            }
            else{
              Keyboard.set_key4(KEY_CODE);
            }
            buttons_pressed+=1;
            break;
          case 1:
            if(controller_code == 0){
              Keyboard.set_key2(KEY_CODE);
            }
            else{
              Keyboard.set_key5(KEY_CODE);
            }
            buttons_pressed+=1;
            break;
          case 2:
            if(controller_code == 0){
              Keyboard.set_key3(KEY_CODE);
            }
            else{
              Keyboard.set_key6(KEY_CODE);
            }
            buttons_pressed+=1;
            break;
          default:
            break;
        }
    }  
    return buttons_pressed;
  }
