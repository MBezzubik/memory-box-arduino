//--------------------------------------------------------------------------------------
// >   >   >   >   >   >   >   M E M O R Y    B O X   <   <   <   <   <   <   <   <   <   
//--------------------------------------------------------------------------------------

// included libraries
#include <Keypad.h>
  
//======================================================================================
//========================== D E F I N I N G   P I N S =================================
//======================================================================================

// define digital pins for using LEDs
#define RED 5
#define GREEN 6
#define BLUE 9
#define YELLOW 11

// define digital pin for controlling passive buzzer
#define buzzerPin 10

//======================================================================================
//======================= D E C L A R I N G    V A R I A B L E S =======================
//======================================================================================

// buzzer - declare variables as sound frequencies
const int C = 264;  // Hz
const int D = 297;
const int E = 330;
const int F = 352;

//keypad - declare number of rows and columns 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//keypad - declare character names for each of the keypad's button , using two-dimensional array
//not really necessary, as the rest of my code doesn't utilize it, using ASCII standard instead
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// each row is controlled by two pins simultaniously
byte rowPins[ROWS] = {13, 8, 7, };  /* connect to the row pinouts of the keypad (array of pin numbers)
                                       the last row has no pin assigned to it (needed it for the buzzer) */
byte colPins[COLS] = {12, 4, 3, 2}; // connect to the column pinouts of the keypad (array of pin numbers)

//initialize an instance of a class NewKeypad (library Keypad)
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/* create arrays of integers for storing random sequence of numbers (intSequence) and user's input 
  (intAnswers) */
int intSequence[20];
int intAnswers[20];

// number of rounds in single game
int rounds = 7;

int count = 1;

// LED flasing time:
int millisec;

//======================================================================================
//================================= S  E  T  U  P ======================================
//======================================================================================

void setup()
{

  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT); // declaring buzzerPin as output
  pinMode(RED, OUTPUT);       // declaring LED pin as output
  pinMode(GREEN, OUTPUT);     // declaring LED pin as output
  pinMode(BLUE, OUTPUT);      // declaring LED pin as output
  pinMode(YELLOW, OUTPUT);    // declaring LED pin as output
  randomSeed(analogRead(0));  /* initializes pseudo-random number generator; an argument analogRead(0) 
                                 will make sure the generated values will be different each time */
  
}

//======================================================================================
//===================================== L  O  O  P =====================================
//======================================================================================

void loop()
{
  /* triggers 'an animation' indicating the device was just turned on or reseted, and the Arduino awaits
     choosing difficulty level */
  beginGameAnimation();
  
  // choosing difficulty level
  int lvl = customKeypad.waitForKey();  /* the loop() is suspended until a key is pressed; the key value
                                           is stored in lvl variable */
  difficultyLvl(lvl);  // lvl variable is passed to a difficultyLvl method body (4-6)
  
  // populates the intSequence array with pseudo-random numbers (range: 1-4)
  populateRandomSequence();
  
  
  ////////////////////////
  // M A I N    G A M E //
  ////////////////////////
  while (count <= rounds)     /* count always starts at 1, incremented by 1 each round; number of rounds 
                                 is specified in Declaring Variables section */
  
  {
    /* executes random sequence from intSequence to call a method ledFlashing with corresponding parameters 
       (more detailed explanation in Methods section) */
    executeIntSequence();
    
    // populates the array intAnswers with user input
    populateIntArray();
    
    count++;  // adds +1 to a counter, goes back to the beginning of while loop
  } // while loop (main game) ends

//======================================================================================
/* this part of the code is reached when condition for while loop is met (becomes  true) 
   [while (count <= rounds)], meaning that the user succesfully finished all rounds */
  
winningAnimation();   // LED lights will flash in a certain sequence, indicating that the user has won the game
resetFunction();      // program resets and the game starts over from the very beginning
        
} // (main) loop ends

//======================================================================================
//============================== M  E  T  H  O  D  S ===================================
//======================================================================================

// triggers a difficulty level "animation" (sequence of LEDs flashing) at the beginning of the game
void beginGameAnimation()
{
  for(int i = 0; i < 2; i++)    // i < 2 means the same sequence of LEDs flashing will be repeated twice
  {
    analogWrite(YELLOW, 255);   /* YELLOW = 11, which is a number of the pin connected to yellow LED; 
                                   255 - it will shine with full power... */
    delay(100);                 // ...for 100 milliseconds
    analogWrite(BLUE, 255);     // same as above, only with blue LED
    delay(100);                 // and so on
    analogWrite(GREEN, 255);
    delay(100);
    analogWrite(RED, 255);
    delay(100);
    analogWrite(RED, 0);
    delay(100);
    analogWrite(GREEN, 0);
    delay(100);
    analogWrite(BLUE, 0);
    delay(100);
    analogWrite(YELLOW, 0);
    delay(100);
  }
}

// chooses difficulty level
void difficultyLvl(int level)   /* parameter level is an integer number inputed by the user at the 
                                   beginning of each game */
{
  switch(level)                       // if imputed number == [4-6], execute one of the following cases:
  {
    case 52: millisec = 200; break;   // 4 (hardest)  (52 ASCII == 4)   // millisec variable is set 
                                                                        // to 200 milliseconds - this value
                                                                        // is then pased to a proper method 
                                                                        // (see above)
    case 53: millisec = 350; break;   // 5 (medium)   (53 ASCII == 5)   // and so on
    case 54: millisec = 750; break;   // 6 (easy)     (54 ASCII == 6)
    case 66: resetFunction(); break;  // reset        (66 ASCII == B)   // if 'B' is pressed, the program resets
  }
}

// populates the intSequence array with pseudo-random numbers (range: 1-4)
void populateRandomSequence()
{
  intSequence[0] = 3;
  intSequence[1] = 4;
  intSequence[2] = 1;
  intSequence[3] = 3;
  intSequence[4] = 2;
  intSequence[5] = 4;
  intSequence[6] = 3;
  
} // fillInRandomSequence ends

/* executes random sequence from intSequence number to call a method ledFlashing with corresponding parameters, 
   resulting in LED flasing and buzzer playing sounda */
void executeIntSequence()
{
    for (int i = 0; i < count; i++)       /* for loop repeated as many times as 'count' equals to (specified in
                                             while loop of the main game) */
    {
 
        if (intSequence[i] == 1)                    /* if the element of random sequence (stored in intSequence[]
                                                       array) == 1, then... */
        {
            ledFlashing(RED, F, millisec);          /* ... invoke ledFlashing method, passing the following 
                                                       arguments: RED (pin number connected to red LED), F ( equals 
                                                       to 352, which */
                                                    /* is a frequency for note F), millisec (value of this variable 
                                                       is assigned to it when difficulty level method is invoked) */
        }
        else if (intSequence[i] == 2)               // and so on
        {
            ledFlashing(GREEN, E, millisec);
        }
        else if (intSequence[i] == 3)
        {
            ledFlashing(BLUE, D, millisec);
        }
        else if (intSequence[i] == 4)
        {
            ledFlashing(YELLOW, C, millisec);
        }

    }
} // fillInSequence ends

/* method for triggering appropriate LED (colour = pin number), sound frequency and LED flash/buzzer sound time 
   (depends on the level of difficulty) triggered by executeIntSequence() method (see below) */
void ledFlashing(int colour, int frequency, int millisec)
{
  delay(50);
  analogWrite(colour, 255);   /* causes one of the LEDs to shine with full power (analogWrite method, using PWM 
                                 accepts values <= 0 (no power) and >= 255 (full power)) */
  tone(buzzerPin, frequency, millisec); /* causes buzzer connected to digital pin 10 (buzzerPin) to play a sound 
                             of a specified frequency, for x amount of milliseconds (depends on difficulty level) */
  delay(millisec);
  analogWrite(colour, 0); // after the delay (again: depends on the difficulty lvl method), turns off the LED
} // ledFlasing ends

// populates the array intAnswers with user input
void populateIntArray()
{
    for (int j = 0; j < count; j++)     /* as mentioned above, count starts at 1 and increments by +1 with each 
                                           passing round (while loop), meaning the intAnswers array will accept 
                                           one more array element each round (starting from 1 to [rounds] variable */
    {
      Serial.println(intSequence[j]);   //debugging
      
      intAnswers[j] = customKeypad.waitForKey(); /* the loop() is suspended until a key is pressed; the key value is 
                                                    stored in the array intAnswers[j];
                                                    'j' increments by +1 (j++) with each passing for loop */
      delay(200);   // wait for 200 milliseconds
      
      // Serial.println(intAnswers[j]);  //debugging

      // assigns ASCII values to more convienient decimal numbers [e.g. 51 (ASCII) == 3 (decimal)]
      switch(intAnswers[j])
      {
        case 49: intAnswers[j] = 1; break;      // assings 1 to key button 49 (ASCII)
        case 50: intAnswers[j] = 2; break;
        case 51: intAnswers[j] = 3; break;
        case 65: intAnswers[j] = 4; break;
        case 66: resetFunction();   break;      // if 'B' is pressed, the game resets (calls for reset function)
      }
      
      Serial.println(intAnswers[j]);  //debugging
      
      // VALIDATES USER INPUT
      if (intSequence[j] != intAnswers[j])    // if user input doesn't match random sequence of flashing LEDs...
      {
        loosingAnimation();                   // ...loosing "animation" is displayed...
        resetFunction(); //call reset             // ...and the game resets
      }

    }
} // end of populateIntArray()

/* if user input doesn't match random sequence of flashing LEDs, this method is invoked, causing the LEDs to flash 
   in order specified in this method's body: */

// creates the pulsing effect (fade in/out), using Pulse Width Modulation technique
void loosingAnimation()
{
    for(int i = 0; i < 2; i++)    // i < 2 - will be repeated twice
    {
      int j = 0;
      while(j < 255)              // while loop will repeat until j is greater than 255
      {
          analogWrite(RED, j);    // red led will start with no power (int j = 0)
          analogWrite(GREEN, j);  // green led will start with no power (int j = 0)
          analogWrite(BLUE, j);   // blue led will start with no power (int j = 0)
          analogWrite(YELLOW, j); // yellow led will start with no power (int j = 0)
          j = j + 15;             // increments j by 15 with each while loop
          delay(30);              // wait for 30 milliseconds to see the dimming effect
      }
      while(j > 0)                /* does almost exactly the same thing as while loop above, only in opposite
                                     direction (starts off LEDs shining full power, then decreases its power 
                                     by 10 every 30 milliseconds until it reaches 0 */
      {
          analogWrite(RED, j);
          analogWrite(GREEN, j);
          analogWrite(BLUE, j);
          analogWrite(YELLOW, j);
          j = j - 10;
          delay(30);
      }
    }
} // loosingAnimation ends

// LED lights will flash in a sequence specified in the method's body, indicating that the user has won the game
void winningAnimation()
{
    delay(500);                 // waits for half a second after user has won the game, then starts the for loop
    for(int i = 0; i < 8; i++)  // loop will be repeated 8 times
    {
        analogWrite(RED, 255);  // red LED, shining with full power...
        delay(100);             // ... for 100 milliseconds...
        analogWrite(RED, 0);    // ... then turns off
        analogWrite(GREEN, 255);// same as above, only with green LED
        delay(100);
        analogWrite(GREEN, 0);
        analogWrite(BLUE, 255);
        delay(100);
        analogWrite(BLUE, 0);   // and so on
        analogWrite(YELLOW, 255);
        delay(100);
        analogWrite(YELLOW, 0);
    }
} // winningAnimation ends

// resets the program
void resetFunction()
{
  count = 1;      // sets the starting value of count back to 1
  loop();         // goes back to the beginning of the loop() method
}

