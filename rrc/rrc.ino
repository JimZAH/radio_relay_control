/*
 * DMTF RELAY CONTROL Jim M0ZAH
 * Based on G8SDU ASM
 * 2021
 */

/*
 * PORT B 9 - 13 DTMF decoder; active high
 * - 9 D1
 * - 10 D2
 * - 11 D3
 * - 12 D4
 * - 13 Tone detected
 * PORT C 0 Tone detect lamp
 * PORT C 1 Success lamp
 * PORT D 1 - 4 Radio 1
 * PORT D 5 - 7 PORT B 8 Radio 2
 */

// Options
#define DTMF_DELAY 500 // Delay before next digit in milliseconds
#define END_DIGIT 0xC // (#)
#define PASSCODE 1111 // Password
#define START_DIGIT 0xB // (*)

// MASKS
#define DTMF_DETECT 0x20 // DTMF DETECT LINE
#define DTMF_MASK 0x1E // DTMF Digits

int DTMF_CODE; // Digit store
int cc = 0; // Digit counter
int input[7] = {0}; // Input array
// [*,p,p,p,p,1/2/3,CMD,CMD2]

void setup()
{
  DDRB = 0x0; // Set port B to inputs
  DDRD = 0xFE; // Set port D to 1-7 OUTPUTS
  DDRC = 0x1F; // // Set port C to 0-5 OUTPUTS
  PORTD = 0x0; // Outputs low 
  PINB = 0x0; // PULLUPS LOW
}

void loop()
{
  if ((PINB & DTMF_DETECT) == DTMF_DETECT)
  { // When we receive a valid digit
    PORTC = 0x1; // Tone detect lamp 
    DTMF_CODE = ((PINB & DTMF_MASK) >> 1);
    input[cc] = DTMF_CODE; // Store our current digit into array

    if (cc >= 7) // If we've received at least 8 digits
    {
      if (input[0] != START_DIGIT) // Check the command started with *
      {
        // TODO: command is malformed
      } else {
      int pass = 0;
      for (int i = 0; i < 4; i++) // Process the password
      {
        pass = pass*10;
        pass = pass + input[i+1];
      }
      if (pass == PASSCODE) // If the password is correct
      {
        PORTC = 0x2; // Success lamp to show access granted
      }
      cc = -1; // Reset digit counter
      DTMF_CODE = 0;
    }
   }
     _delay_ms(DTMF_DELAY); // Max length of each digit
     ((PINB & DTMF_DETECT) == DTMF_DETECT) ? cc = 0 : cc++; // If we're still receiving a digit invalidate it
  }
  if (PORTC != 0x0){ // Reset status lamps
    PORTC = 0x0;
  }
}
