# radio_relay_control
DTMF control based on Bob G8SDU control. Ported from ASM, this is not a direct port!

# Example
Assuming the password is 1111:

 *111128
The command above would turn radio 2 binary output to 8

 *11111D
The command above would turn radio 1 binary output to 0

 *111139
The command above would turn radio 3 binary output to 9

Sending # will reset the counters so you can start again.


# Wiring

DTMF DECODER:
Q1: PB1
Q2: PB2
Q3: PB3
Q4: PB4
ESt: PB5

RADIO 1:
B1: PD1
B2: PD2
B3: PD3
B4: PD4

RADIO 2:
B1: PD5
B2: PD6
B3: PD7
B4: PB0

RADIO 3:
B1: PC2
B2: PC3
B3: PC4
B4: PC5

DCD LED:
PC0
