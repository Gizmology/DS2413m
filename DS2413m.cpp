// DS2413m.cpp - Support for DS2413 dual-channel PIO 1-wire chip via DS2482-800.
//
// Author: Gary Fariss at Gizmology.com

//  Copyright 2014 Gary Fariss
//
//  This file is part of DS2413m.
//
//  DS2413m is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include "Arduino.h"
#include <DS2482.h>
#include <DS2413m.h>


// Constructor.  Saves pointer to DS2482 object.
//
DS2413m::DS2413m(DS2482* mObj){ 
  _m = mObj;
}


// Set 1-wire device channel and 8-byte address
//
void DS2413m::setAdr(uint8_t ch, uint8_t adr[8]){
  _chan = ch;
  for (uint8_t i=0; i<=7; i++)
	_addr[i] = adr[i];
}


// Read PIOA and PIOB pin states.
//
uint8_t DS2413m::read(bool& A, bool& B){
  uint8_t stat;

  if (_chan <= 7)                  // If there's a DS2482-800 connected.
    if (!_m->selectChannel(_chan))   // Select proper channel on the DS2482-800
      return 0x08;                     // Channel Select error
		
  if (!_m->wireReset())            // If no presence pulse
    return 0x01;    
  _m->wireSelect(_addr);           // Select the device
  _m->wireWriteByte(0xF5);         // Tell device we're reading

  stat = _m->wireReadByte();       // Get the status byte
  A = ((stat & 0x01) == 1);
  B = ((stat & 0x04) == 4);

  if (!_m->wireReset())            // If no presence pulse
    return 0x04;
	
  return 0x00;                     // Success!
}


// Write to PIO Output Latches.
//
uint8_t DS2413m::write(bool A, bool B){

  // Create command to send
  //
  uint8_t cmd = 0xFC;  // 1111 1100  
  if (A)  cmd |= 1;    // 1111 1101
  if (B)  cmd |= 2;    // 1111 111x	

  if (_chan <= 7)                  // If there's a DS2482-800 connected.
    if (!_m->selectChannel(_chan))   // Select proper channel on the DS2482-800
      return 0x08;                     // Channel Select error
		
  if (!_m->wireReset())            // If no presence pulse
    return 0x01;    
  _m->wireSelect(_addr);           // Select the device
  _m->wireWriteByte(0x5A);         // Tell device we're writing
  _m->wireWriteByte(cmd);          // Send latch data
  _m->wireWriteByte(~cmd);         // Send inverse of latch data
       
  if (_m->wireReadByte() != 0xAA)  // Get result, 0xAA=ok, 0xFF=err         
    return 0x02;
    
  _m->wireReadByte();              // PIO Status is returned, we ignore it.
  
  if (!_m->wireReset())            // If no presence pulse
    return 0x04;
    
  return 0x00;                     // Success!

}
 