/**
 * DMTF RELAY CONTROL Jim M0ZAH Ported
 * 8 relays with password protection
 * 1 enable output, 0 disable output
 * EG: 31 will enable relay 3, 30 will disable relay 3
 */

// Options
#define DTMF_DELAY 25 // Delay before next digit in milliseconds
#define PASSCODE 1234 // Password
#define RELAY_1 0x1 // Relay 1
#define RELAY_2 0x2 // Relay 2
#define RELAY_3 0x3 // Relay 3
#define RELAY_4 0x4 // Relay 4
#define RELAY_5 0x5 // Relay 5
#define RELAY_6 0x6 // Relay 6
#define RELAY_7 0x7 // Relay 7
#define RELAY_8 0x8 // Relay 8

// MASKS
#define DTMF_DETECT 0x1 // DTMF DETECT LINE
#define DTMF_MASK 0x1E // DTMF Digits

int DTMF_CODE; // Digit store
int cc = 0; // Digit counter
int input[5] = {0}; // Input array

void setup()
{
  DDRB = 0x0; // Set port B to inputs
  DDRD = 0xFE; // Set port D to 1-7 OUTPUTS
  PORTD = 0x0; // Outputs low 
  DDRB = 0x0; // PULLUPS LOW
}

void loop()
{
  while ((PINB & DTMF_DETECT) == DTMF_DETECT)
  { // When we receive a valid digit
    DTMF_CODE = (PINB & DTMF_MASK);
    input[cc] = DTMF_CODE; // Store our current digit into array

    if (cc >= 5) // If we've received at least 6 digits
    {
      int pass = 0;
      for (int i = 0; i < 4; i++) // Process the password
      {
        pass = pass*10;
        pass = pass + input[i];
      }
      if (pass == PASSCODE) // If the password is correct
      {
        switch (input[4]) // Relay selection
        {
          case RELAY_1:
          //relay 1
          break;
          case RELAY_2:
          //realy 2
          break;
          case RELAY_3:
          //relay 3
          break;
          case RELAY_4:
          //realy 4
          break;
          case RELAY_5:
          //relay 5
          break;
          case RELAY_6:
          //realy 6
          break;
          case RELAY_7:
          //relay 7
          break;
          case RELAY_8:
          //realy 8
          break;
          default:
          // invalid selection
          break;
        }
      }
      cc = 0; // Reset digit counter
    }

     _delay_ms(DTMF_DELAY); // Max length of each digit
     ((PINB & DTMF_DETECT) == DTMF_DETECT) ? cc = 0 : cc++; // If we're still receiving a digit invalidate it
  }
}
