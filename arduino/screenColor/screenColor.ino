
/*
 * Reads 4 different rgb colours from serial and sets leds to those colours using pseudo PWM.
 *
 */

int colors[12] = {0}; // stores colors read from serial

int val[12] = {0}; // stores gamma corrected colors

int i, j;

// gamma correction lookup table for led brightness
unsigned int gamma[] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

void setup()
{
  DDRD = DDRD | B11111100;  // set pins 2 to 7 as outputs without changing the value of pins 0 & 1, which are Rx & Tx
  DDRB = B11111111;         // set pins 8 to 13 as outputs
  Serial.begin(9600);
}

void loop() 
{
  // turn all pins off
  PORTD = B00000000;
  PORTB = B00000000;
  
  // check if new colours are available
  if (Serial.available() >= 13)
  {
    // wait for sync marker
    if (Serial.read() == 0xff)
    {
      for (i = 0; i < 12; i++)
      {
        colors[i] = Serial.read();
        
        // Make color warmer by tuning down green and blue
        if ((i+2)%3 == 0)
        {
          colors[i] = (int)((float)colors[i]*(-0.15/255*colors[i]+1));
          //colors[i] = (int)((float)colors[i]*(0.85));
        }
        else if ((i+1)%3 == 0)
        {
          colors[i] = (int)((float)colors[i]*(-0.3/255*colors[i]+1));
          //colors[i] = (int)((float)colors[i]*(0.7));
        }
        
        val[i] = gamma[colors[i]]; // led's values according to gamma-table
      }
    }
  }

  // Pseudo PWM loop turns leds on at right time
  for (i = 255; i > 0; i--) // skip 0 to keep black black
  {
    for (j = 2; j < 14; j++) // start from 2 to skip ports 0 and 1
    {
      if ( i == val[j-2] )
      {
        // bitshift a one into the proper place
        if (j < 8)
        {
          PORTD = PORTD | ((1 << j));
        }
        else
        {
          PORTB = PORTB | ((1 << (j-8)));
        }
      }
    }
  }

}
