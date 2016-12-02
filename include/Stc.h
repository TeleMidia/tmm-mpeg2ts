/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#ifndef STC_H_
#define STC_H_

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <inttypes.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Stc {

	#define SYSTEM_CLOCK_FREQUENCY 27000000
	#define	SYSTEM_CLOCK_FREQUENCY_90 90000

	private:
#ifdef _WIN32
		int64_t instant;
		double period; // microsecond
		void processorFrequency();
#endif
	protected:
		int64_t stc;
		struct timeval clockRef;

		int64_t reference;

		virtual void refreshStcSample();

	public:
		Stc();
		virtual ~Stc();

		static int timevalSubtract(
				struct timeval *result, struct timeval *x, struct timeval *y);
		static int64_t baseExtToStc(int64_t base, short int ext);
		static int64_t stcToBase(int64_t stc);
		static int64_t stcToExt(int64_t stc);
		static double stcToSecond(int64_t stc);
		static double baseToSecond(int64_t base);
		static int64_t secondToStc(double seconds);
		static int64_t secondToBase(double seconds);
		static int64_t calcDiff90khz(int64_t a, int64_t b);
		static int64_t calcDiff27Mhz(int64_t a, int64_t b);
		static void waitFor(int64_t stcPeriod);

		void getClock(struct timeval* usrClk);
		int64_t getReference();
		void setReference(int64_t stc);
		void setReference(int64_t base, short int ext);
		int64_t getStc();
		int64_t getStcBase();
		int64_t getStcExt();
		double getSecond();
};

}
}
}
}

#endif /* STC_H_ */
