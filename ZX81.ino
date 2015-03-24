// ZX81 USB Keyboard for Leonardo
// (c) Dave Curran
// 2013-04-27

// Modified with Function keys by Tony Smith
// 2014-02-15

#define NUM_ROWS 8
#define NUM_COLS 5

// Keymap for normal use

byte keyMap[NUM_ROWS][NUM_COLS] = {
  {'5', '4', '3', '2', '1'}, 
  {'t', 'r', 'e', 'w', 'q'}, 
  {'6', '7', '8', '9', '0'}, 
  {'g', 'f', 'd', 's', 'a'}, 
  {'y', 'u', 'i', 'o', 'p'},
  {'v', 'c', 'x', 'z', 0}, 
  {'h', 'j', 'k', 'l', KEY_RETURN},
  {'b', 'n', 'm', '.', ' '}
};

// Keymap if Shift is pressed

byte keyMapShifted[NUM_ROWS][NUM_COLS] = {
  {KEY_LEFT_ARROW, '$', '\\', '@', KEY_ESC}, 
  {'T', 'R', 'E', 'W', 'Q'}, 
  {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_F6, KEY_BACKSPACE}, 
  {'G', 'F', 'D', 'S', 'A'}, 
  {'Y', 'U', 'I', 'O', 'P'},
  {'V', 'C', 'X', 'Z', 0}, 
  {'H', 'J', 'K', 'L', KEY_F5},
  {'B', 'N', 'M', ',', ' '}
};

// keymap if Shift is pressed plus New Line
// *Next* key read should be from this table

byte keyMapAlt[NUM_ROWS][NUM_COLS] = {
  {KEY_LEFT_ARROW , '}' , '{' , ']' , '['}, 
  {'=' , '<' , 0 , '~' , 64}, 
  {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_F6, KEY_BACKSPACE}, 
  {0 , 0 , 0 , '|' , 156}, 
  {'>', '$', '(', ')', '\"'},
  {'/', '?', ';', ':', 0}, 
  {'_', '-', '+', '=', KEY_RETURN},
  {'*', '<', '>', '\'', '#'}
};


// Global variables

int debounceCount[NUM_ROWS][NUM_COLS];
int altKeyFlag;
int ctrlKeyFlag;

// Define the row and column pins

byte colPins[NUM_COLS] = {13, 12, 11, 10, 9};
byte rowPins[NUM_ROWS] = {7, 6, 5, 4, 3, 2, 1, 0};

// Where is the Shift key

#define SHIFT_COL 4
#define SHIFT_ROW 5

// How many times does a key need to register as pressed?

#define DEBOUNCE_VALUE 250
#define REPEAT_DELAY 500



void setup()
{
  // set all pins as inputs and activate pullups

  for (byte c = 0 ; c < NUM_COLS ; c++)
  {
    pinMode(colPins[c], INPUT);
    digitalWrite(colPins[c], HIGH);

    // Clear debounce counts
    
    for (byte r = 0 ; r < NUM_ROWS ; r++)
    {
      debounceCount[r][c] = 0;
    }
  }

  // Set all pins as inputs

  for (byte r = 0 ; r < NUM_ROWS ; r++)
  {
    pinMode(rowPins[r], INPUT);
  }

  // Zero Function modifier flag

  altKeyFlag = 0;
  ctrlKeyFlag = 0;

  // Initialize control over the keyboard
  // This is an Arduino Leonardo routine

  Keyboard.begin();
}



void loop()
{
  // Check for Shift
  
  bool shifted = false;
  bool keyPressed = false;
  
  pinMode(rowPins[SHIFT_ROW], OUTPUT);

  if (digitalRead(colPins[SHIFT_COL]) == LOW)
  {
    shifted = true;
  }

  if (shifted == true && altKeyFlag == 255)
  {
    // NOP in order to prevent Function selection from auto-repeating
  }
  else
  {
    pinMode(rowPins[SHIFT_ROW], INPUT);

    // For each row
    
    for (byte r = 0 ; r < NUM_ROWS ; r++)
    {
      // Turn the row on
      
      pinMode(rowPins[r], OUTPUT);
      digitalWrite(rowPins[r], LOW);

      for (byte c = 0 ; c < NUM_COLS ; c++)
      {
        if (digitalRead(colPins[c]) == LOW)
        {
          // Increase the debounce count

          debounceCount[r][c]++;

          // Has the switch been pressed continually for long enough?

          int count = debounceCount[r][c];
          if (count == DEBOUNCE_VALUE)
          {
            // First press
            
            keyPressed = true;          
            pressKey(r, c, shifted);
          }
          else if (count > DEBOUNCE_VALUE)
          {
            // Check for repeats
            
            count -= DEBOUNCE_VALUE;
            if (count % REPEAT_DELAY == 0)
            {
              // Send repeat press
              
              keyPressed = true;          
              pressKey(r, c, shifted);
            }  
          }
        }
        else
        {
          // No key pressed, so reset debounce count

          debounceCount[r][c] = 0;
        }
      }

      // Turn the row back off
      
      pinMode(rowPins[r], INPUT);
    }

    digitalWrite(rowPins[SHIFT_ROW], LOW);
  }
}


// Send the keypress

void pressKey(byte r, byte c, bool shifted)
{  
  byte key = shifted ? keyMapShifted[r][c] : keyMap[r][c];

  if (altKeyFlag == 255)
  {
    // Get the Alt key pressed after Function has been selected
    
    key = keyMapAlt[r][c];
    altKeyFlag = 0;
  }

  if (key == KEY_F5)
  {
    // If the Function key pressed (Shift + New Line)
    
    altKeyFlag = 255;
    key = 0;
    debounceCount[r][c] = 0;
  }
  
  if (key == KEY_F6)
  {
    ctrlKeyFlag = 255;
    key = 0;
    debounceCount[r][c] = 0;
  }

  if (key > 0)
  {
    // send the key
    
    if (ctrlKeyFlag == 255)
    {
      ctrlKeyFlag = 0;
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(key);
      delay(100);
      Keyboard.releaseAll();
      return;
    }
    
    Keyboard.write(key);
  }
}
