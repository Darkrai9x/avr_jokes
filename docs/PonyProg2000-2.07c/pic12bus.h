//=========================================================================//
//                                                                         //
//  PonyProg - Serial Device Programmer                                    //
//                                                                         //
//  Copyright (C) 1997-2007   Claudio Lanconelli                           //
//                                                                         //
//  http://ponyprog.sourceforge.net                                        //
//                                                                         //
//-------------------------------------------------------------------------//
// $Id: pic12bus.h,v 1.2 2007/04/20 10:58:22 lancos Exp $
//-------------------------------------------------------------------------//
//                                                                         //
// This program is free software; you can redistribute it and/or           //
// modify it under the terms of the GNU  General Public License            //
// as published by the Free Software Foundation; either version2 of        //
// the License, or (at your option) any later version.                     //
//                                                                         //
// This program is distributed in the hope that it will be useful,         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of          //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       //
// General Public License for more details.                                //
//                                                                         //
// You should have received a copy of the GNU  General Public License      //
// along with this program (see COPYING);     if not, write to the         //
// Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. //
//                                                                         //
//-------------------------------------------------------------------------//
//=========================================================================//

#ifndef	_PIC12BUS_H
#define	_PIC12BUS_H

#include "types.h"
#include "busio.h"
#include "pgminter.h"

class Pic12Bus : public BusIO
{
 public:		//------------------------------- public
	Pic12Bus(BusInterface *ptr = 0);
	virtual ~Pic12Bus();

	long Read(int addr, UBYTE *data, long length);
	long Write(int addr, UBYTE const *data, long length);

//	int Erase(int type = ALL_TYPE);

	int Reset();

	long ReadConfig(UWORD &data);
	long WriteConfig(UWORD data);

	void DisableCodeProtect();

	int CompareSingleWord(UWORD data1, UWORD data2, UWORD mask);
	int CompareMultiWord(UBYTE *data1, UBYTE *data2, ULONG length, int split);

	void SetDelay();

	long BlankCheck(long length);
	void IncAddress(int n);

 protected:		//------------------------------- protected

	int SendDataWord(long wo, int wlen = 16);
	long RecDataWord(int wlen = 16);
	int WaitReadyAfterWrite(long timeout = 5000);

	int WriteProgWord(UWORD val, long rc_addr);
	int ProgramPulse(UWORD val, int verify = 0, int width = 100);	//100uSec default pulse width

	int SendCmdCode(int opcode)
		{ return SendDataWord(opcode, 6); }
	int SendProgCode(UWORD data)
	{
		//the code is 14 bit data with leading and trailing 0's
		data &= ProgMask;
		data <<= 1;
		return SendDataWord(data);
	}
	UWORD RecvProgCode()
	{
		return (UWORD)(RecDataWord() >> 1) & ProgMask;
	}
 /**
	int SendDataCode(UWORD data)
	{
		//the code is 8 bit data with leading and trailing 0's
		data &= DataMask;
		data <<= 1;
		return SendDataWord(data);
	}
	UWORD RecvDataCode()
	{
		return (UWORD)(RecDataWord() >> 1) & DataMask;
	}
 **/

	void SetMCLR()
		{ busI->SetControlLine(1); }
	void ClearMCLR()
		{ busI->SetControlLine(0); }

	long current_address;

	int OverProgrammingMult;
	int OverProgrammingAdd;

//	const UWORD DataMask;
	const UWORD ProgMask;

	//Command Opcode
	const UBYTE ReadProgCode;
	const UBYTE LoadProgCode;
//	const UBYTE ReadDataCode;
//	const UBYTE LoadDataCode;
//	const UBYTE LoadConfigCode;
	const UBYTE IncAddressCode;
//	const UBYTE EraseProgMem;
//	const UBYTE EraseDataMem;
	const UBYTE BeginProgCode;
//	const UBYTE BeginProgOnlyCode;
	const UBYTE EndProgCode;

 private:		//------------------------------- private

	int SendDataBit(int b);
	int RecDataBit();

	void bitDI(int b)
		{ busI->SetDataOut(!b); }

	void setDI()
		{ busI->SetDataOut(0); }

	void clearDI()
		{ busI->SetDataOut(1); }

	void setCLK()
		{ busI->SetClock(1); }

	void clearCLK()
		{ busI->SetClock(0); }

	int getDO() const
		{ return busI->GetDataIn(); }

};

#endif
