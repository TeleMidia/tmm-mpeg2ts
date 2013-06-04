/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "Stc.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Stc::Stc() {
	reference        = 0;
	stc              = 0;
	clockRef.tv_sec  = 0;
	clockRef.tv_usec = 0;
#ifdef _WIN32
	processorFrequency();
#endif
}

Stc::~Stc() {

}

#ifdef _WIN32
void Stc::processorFrequency() {
	LARGE_INTEGER proc_freq;

	if (!::QueryPerformanceFrequency(&proc_freq)) {
		period = 0.0;
		return;
	}

	period = (double) proc_freq.QuadPart / 1000000.;
}
#endif

int Stc::timevalSubtract(
		struct timeval *result, struct timeval *x, struct timeval *y) {

	int nsec;

	if (x->tv_usec < y->tv_usec) {
		nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec  += nsec;

	}

	if (x->tv_usec - y->tv_usec > 1000000) {
		nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec  -= nsec;
	}

	result->tv_sec  = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	return x->tv_sec < y->tv_sec;
}

void Stc::getClock(struct timeval* usrClk) {
#ifdef _WIN32
	if ((unsigned int) period) {
		LARGE_INTEGER st;
		DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
		::QueryPerformanceCounter(&st);
		::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
		instant = ((double) st.QuadPart / period) + 0.5f;
		usrClk->tv_sec = instant / 1000000;
		usrClk->tv_usec = instant % 1000000;
	} else {
		double temp = (double) clock() / CLOCKS_PER_SEC;
		usrClk->tv_sec = (long) temp;
		usrClk->tv_usec = 1000000*(temp - usrClk->tv_sec);
	}
#else
	/*
	 * To use clock_gettime you need to link to librt.a.
	 * If you use gcc just make sure you add -lrt to your list of arguments.
	 */
	timespec st;
	if (clock_gettime(CLOCK_MONOTONIC, &st) != 0) {
		cout << "Stc::getClock - CLOCK_MONOTONIC not supported." << endl;
		exit(EXIT_FAILURE);
	}
	usrClk->tv_sec = st->tv_sec;
	usrClk->tv_usec = st->tv_nsec / 1000;
#endif
}

int64_t Stc::baseExtToStc(int64_t base, short int ext) {
	return (base * 300) + ext;
}

int64_t Stc::stcToBase(int64_t stc) {
	return (stc / 300) + 0.5f;
}

int64_t Stc::stcToExt(int64_t stc) {
	return stc % 300;
}

double Stc::stcToSecond(int64_t stc) {
	return (double) stc / SYSTEM_CLOCK_FREQUENCY;
}

double Stc::baseToSecond(int64_t base) {
	return (double) base / SYSTEM_CLOCK_FREQUENCY_90;
}

int64_t Stc::secondToStc(double seconds) {
	return (seconds * SYSTEM_CLOCK_FREQUENCY) + 0.5f;
}

int64_t Stc::secondToBase(double seconds) {
	return (seconds * SYSTEM_CLOCK_FREQUENCY_90) + 0.5f;
}

int64_t Stc::calcDiff90khz(int64_t a, int64_t b) {
	if (a >= b) {
		return a - b;
	} else {
		return (0x1FFFFFFFF - b) + a;
	}
}

int64_t Stc::calcDiff27Mhz(int64_t a, int64_t b) {
	if (a >= b) {
		return a - b;
	} else {
		return (0x3FFFFFFFFFF - b) + a;
	}
}

void Stc::waitFor(int64_t stcPeriod) {
	if (stcPeriod <= 0) return;

	int t = stcPeriod / 27;
	while (t) {
		if (t >= 1000000) {
			usleep(999999);
			t -= 999999;
		} else {
			usleep(t);
			t = 0;
		}
	}
}

void Stc::refreshStcSample() {
	struct timeval currentRef;
	struct timeval result;
	uint64_t clockedSec, clockedUsec;

	getClock(&currentRef);
	timevalSubtract(&result, &currentRef, &clockRef);
	clockedSec  = ((uint64_t) result.tv_sec) * SYSTEM_CLOCK_FREQUENCY;
	clockedUsec = result.tv_usec * 27;
	stc = clockedSec + clockedUsec;
}

int64_t Stc::getReference() {
	return reference;
}

void Stc::setReference(int64_t stc) {
	reference = stc;
	getClock(&clockRef);
}

void Stc::setReference(int64_t base, short int ext) {
	reference = (base * 300) + ext;
	getClock(&clockRef);
}

int64_t Stc::getStc() {
	refreshStcSample();
	stc = stc + reference;
	return stc;
}

int64_t Stc::getStcBase() {
	refreshStcSample();
	stc = stc + reference;
	return stcToBase(stc);
}

int64_t Stc::getStcExt() {
	refreshStcSample();
	stc = stc + reference;
	return stc % 300;
}

double Stc::getSecond() {
	return stcToSecond(getStc());
}

}
}
}
}
