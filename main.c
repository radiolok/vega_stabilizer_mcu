/*Copyright (c) 2015, Artem Kashkanov ask@urlnn.ru (c)
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <msp430g2553.h>
#include <system.h>

#include "system.h"
#include "pinout.h"

#define abs(x) ((x)>0?(x):-(x))

void ClockSetup(){
	//sensor crystal clock ACLK
	BCSCTL1 |= XTS;// High-frequency XT1 clock
	BCSCTL2 |= SELM1 | SELM0 ;//LFXT source for MCLK
	BCSCTL3 |= LFXT1S1 ;//4MHz crystal

	//PWM DCO clock - SMCLK
	DCOCTL = DCO2 | DCO1 | DCO0 ;
	BCSCTL1 |= RSEL3 | RSEL2 | RSEL1 | RSEL0;
}

void SystemSetup(){
	ClockSetup();

	ButtonsSetup();

	ElevatorSetup();

	TonarmSetup();

	StopSensorSetup();

	DriveSetup();
}
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    SystemSetup();
	long previoustime = 0;
	long currenttime = 0;
    while(1){
    	currenttime = Millis();
    	if (100 > abs(currenttime - previoustime)){
    		//every 100ms;
    		SystemCheck();//every 0.1 s
    		previoustime = currenttime;
    	}


    }//while (1)
	return 0;
}
