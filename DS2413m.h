// DS2413m.h - Support for DS2413 dual-channel PIO 1-wire chip via DS2482-800.
//
// Author: Gary Fariss at Gizmology.com
//
// This library provides an interface to the DS2413 as accessed through
// the DS2482-100 or the DS2482-800 single or 8-channel 1-wire master chip.
//
// This library utilizes the Paeae Technologies DS2482 Library that is posted at
// https://github.com/paeaetech/paeae/tree/master/Libraries/ds2482 by Mikko 
// Sivulainen.
//
// This library is based on the DS2413 functions by Michal Zachwieja 2011 v0.7
// as posted in http://forum.arduino.cc/index.php/topic,59689.0.html on 
// 27 April 2011. The "m" appended to the chip name indicates that the access 
// to the DS2413 is via a 1-wire "Master".
//

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

#ifndef DS2413m_h
#define DS2413m_h
#include <DS2482.h>

#include "Arduino.h"

class DS2413m
{
  public:

	// Constructor. Saves pointer to DS2482 object. (See EXAMPLE sketch)
    //
    DS2413m(DS2482*);   
	

    // setAdr(ch, adr) - Set the address to use.  Persists until changed.
    //
    //  ch = 0-7 = Channel of DS2482-800 to select for this device.
    //        >7 = Connection is via DS2482-100, don't issue channel select.
    //
    //  adr[8]   = ROM address of device
	//
	void setAdr(uint8_t ch, uint8_t adr[8]);


    // read(A, B) - Read the state of both PIO pins.
    //
    // Parameters:
    //   A = PIOA pin state is returned here.
    //   B = PIOB pin state is returned here.
    //
    // Error Bits returned:
	//   bit03 = If DS2482-800 and Channel Select error
    //   bit02 = No presence pulse returned after final reset
    //   bit01 = 0
    //   bit00 = No presence pulse returned after initial reset
	//    
	uint8_t read(bool& A, bool& B);


    // write(A, B) - Set the state of both PIO output latches.
    //
    // Parameters:
    //   A = Boolean value to write to PIO Latch A.
    //   B = Boolean value to write to PIO Latch B.
    //
    // Error Bits returned:
	//   bit03 = If DS2482-800 and Channel Select error
    //   bit02 = No presence pulse returned after final reset
    //   bit01 = Write error (0xAA not returned)
    //   bit00 = No presence pulse returned after initial reset
    //
	uint8_t write(bool A, bool B);


  private:
    uint8_t _chan;      // Saves DS2482-800 channel to use
    uint8_t _addr[8];   // Saves 1-wire device address
    DS2482* _m;         // Saves pointer to DS2482 object
};

#endif