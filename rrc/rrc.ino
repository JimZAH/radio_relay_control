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
 * PORT C 1 Success lamp // NOT USED
 * PORT D 1 - 4 Radio 1
 * PORT D 5 - 7 PORT B 8 Radio 2
 */

// Options
#define DTMF_DELAY 500 // Delay before next digit in milliseconds
#define END_DIGIT 0xC // (#)
#define PASSCODE 1111 // Password
#define START_DIGIT 0xB // (*)

// Radio control array
int radioOne[16][2] =
{
  {0x0, 0b0000}, //D
  {0x1, 0b0001}, //1
  {0x2, 0b0010}, //2
  {0x3, 0b0011}, //3
  {0x4, 0b0100}, //4
  {0x5, 0b0101}, //5
  {0x6, 0b0110}, //6
  {0x7, 0b0111}, //7
  {0x8, 0b1000}, //8
  {0x9, 0b1001}, //9
  {0xA, 0b1010}, //0
  {0xB, 0b1011}, //*
  {0xC, 0b1100}, //#
  {0xD, 0b1101}, //A
  {0xE, 0b1110}, //B
  {0xF, 0b1111}  //C
};
int radioTwo[16][2] =
{
  {0x0, 0b0000},
  {0x1, 0b0001},
  {0x2, 0b0010},
  {0x3, 0b0011},
  {0x4, 0b0100},
  {0x5, 0b0101},
  {0x6, 0b0110},
  {0x7, 0b0111},
  {0x8, 0b1000},
  {0x9, 0b1001},
  {0xA, 0b1010},
  {0xB, 0b1011},
  {0xC, 0b1100},
  {0xD, 0b1101},
  {0xE, 0b1110},
  {0xF, 0b1111}
};
int radioThree[16][2] =
{
  {0x0, 0b0000},
  {0x1, 0b0001},
  {0x2, 0b0010},
  {0x3, 0b0011},
  {0x4, 0b0100},
  {0x5, 0b0101},
  {0x6, 0b0110},
  {0x7, 0b0111},
  {0x8, 0b1000},
  {0x9, 0b1001},
  {0xA, 0b1010},
  {0xB, 0b1011},
  {0xC, 0b1100},
  {0xD, 0b1101},
  {0xE, 0b1110},
  {0xF, 0b1111}
};


// MASKS
#define DTMF_DETECT 0x20 // DTMF DETECT LINE
#define DTMF_MASK 0x1E // DTMF Digits

int DTMF_CODE; // Digit store
int cc = 0; // Digit counter
int input[6] = {0}; // Input array
// [*,p,p,p,p,1/2/3,CMD]

void setup()
{
  DDRB = 0x1; // Set port B 9 - 13 to inputs
  DDRC = 0xFF; // // Set port C 0-5 to OUTPUTS
  DDRD = 0xFE; // Set port D 1-7 to OUTPUTS
  PORTB = 0x0; // PULLUPS LOW
  PORTC = 0x0; // Outputs LOW
  PORTD = 0x0; // Outputs low 
}

void loop()
{
  if ((PINB & DTMF_DETECT) == DTMF_DETECT)
  { // When we receive a valid digit
    PORTC = PORTC | 0x1; // Tone detect lamp 
    DTMF_CODE = ((PINB & DTMF_MASK) >> 1);
    input[cc] = DTMF_CODE; // Store our current digit into array

    if (cc >= 6) // If we've received at least 7 digits
    {
      int pass = 0;
      for (int i = 0; i < 4; i++) // Process the password
      {
        pass = pass*10;
        pass = pass + input[i+1];
      }
      if (pass == PASSCODE) // If the password is correct
      {
        switch (input[5])
        {
          case 0x1:
          PORTD = PORTD & 0xE0; // Only clear the 4 bits for radio 1
          PORTD = PORTD | (radioOne[input[6]][1] << 1) & 0xFF;
          break;
          case 0x2:
          PORTD = PORTD & 0xF << 1; // Only clear the 4 bits for radio 2
          PORTD = PORTD | (radioTwo[input[6]][1] << 5);
          PORTB = (radioTwo[input[6]][1] >> 3);
          break;
          case 0x3:
          PORTC = 0;
          PORTC = (radioThree[input[6]][1] << 2);
          break;
          default:
          // Radio not found
          break;
        }
      }
      cc = -1; // Reset digit counter
      DTMF_CODE = 0;
   }

     if ((cc < 1 && input[0] != START_DIGIT) || DTMF_CODE == END_DIGIT)
     { // Reset all if the first digit is not a (*) or we received a (#)
      for (int i=0; i < sizeof(input)/2; i++)
      {
        input[i] = 0;
      }
      cc = -1; // Reset digit counter
      DTMF_CODE = 0;
     }
     
     _delay_ms(DTMF_DELAY); // Max length of each digit
     PORTC = PORTC & 0x3C;
     ((PINB & DTMF_DETECT) == DTMF_DETECT) ? cc = 0 : cc++; // If we're still receiving a digit invalidate it
  }
}
