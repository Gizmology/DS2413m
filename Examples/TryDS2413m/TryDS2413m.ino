// TryDS2413m.ino - Example of use of the DS2413m Library.
//

// This sketch assumes a DS2413 with:
//
//   1. An LED connected to PIO-A (TSOC pin 6) via a current-limiting resistor to +5v.
//   2. A switch connected between PIO-B (TSOC pin 4) and ground.
//   3. A pull-up resistor (10k) connected between PIO-B and +5v.
//
//   A circuit diagram of this configuration is shown in the DS2413 datasheet: 
//     http://datasheets.maximintegrated.com/en/ds/DS2413.pdf
//
// The DS2413 is connected to channel 7 of a DS2482-800 1-wire Master chip.
//
// The 8-byte address of my DS2413 chip is replaced with your address in the rom[8]
// array (see setup function below.)
//
// The DS2482 is configured for I2C address zero (pins 6,7,8 grounded).
//
// The DS2482 is connected to the SDA and SCL pins of your Arduino.
//

// With the sketch running, you can see the LED flash and watch the PIO-B pin state
// change when you toggle the switch. If you disconnect the DS2413 from the 1-wire
// line, you will see the write and read functions return a "No presence pulse returned
// after initial reset" error (bit0=1).
//

#include <Wire.h>        // DS2482 uses I2C functions

#include <DS2482.h>
DS2482 ds82(0);          // Create DS2482 object (ds82) and pass I2C address (zero)

#include <DS2413m.h>
DS2413m ds13(&ds82);     // Create DS2413 object (ds13) and pass pointer to DS2482 object.


void setup(void){
  byte rom[8] = {0x3A, 0x22, 0xDD, 0x04, 0x00, 0x00, 0x00, 0xFB};
  uint8_t chan = 7; 

  Serial.begin(9600);         // For comms with PC
  Wire.begin();               // For I2C comms
  
  ds13.setAdr(chan,rom);      // Tell DS2413m the DS2482 channel and DS2413 device address to use.
}


void loop(void){
  bool A, B, C, D;
  uint8_t i, j;
  
// Turn the LED off, read the state of the switch.
// 
  A=HIGH;                 // LED off, PIO pin=1=HIGH=true
  B=HIGH;                 // Switch pin output latch always HIGH.
  i = ds13.write(A, B);
  Serial.print("PIO Write function result: ");
  Serial.println(i, HEX);
  
  j = ds13.read(C, D);
  Serial.print("PIO Read function result:  ");
  Serial.print(j, HEX);
  Serial.print(",  PIO Pin States (AB): ");
  Serial.print(C, HEX);
  Serial.println(D, HEX);  
  delay(1000);             // Delay 1 second


// Turn the LED on, read the state of the switch.
//
  A=LOW;                  // LED on, PIOA pin=0=LOW=false
  // B=HIGH;              // Switch pin output latch always HIGH.  
  i = ds13.write(A, B);
  Serial.print("PIO Write function result: ");
  Serial.println(i, HEX);
  
  j = ds13.read(C, D);
  Serial.print("PIO Read function result:  ");
  Serial.print(j, HEX);
  Serial.print(",  PIO Pin States (AB): ");
  Serial.print(C, HEX);
  Serial.println(D, HEX);
  delay(1000);             // Delay 1 second
}

