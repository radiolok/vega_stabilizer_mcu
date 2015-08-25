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
#include "drive.h"

unsigned short drivework = OFF;

//store current speed target period to stabilize
unsigned short targetperiod = 0;

unsigned short currentperiod = 0;

unsigned short currenterror = 0;
unsigned short lasterror = 0;

static float KP = 1.0;
static float KI = 1.0;
static float KD = 0.0;

static float t_timer = 2.5e-7;

float integralspeed = 0.0;

unsigned short drivespeed = 0;

void DriveSetup(void){
	TimerSetup();
	DriveSensorSetup();
	DriveStop();
}

void TimerSetup(void){
	//PWM timer:
	TA0CTL |= TASSEL1 | MC0 | TAIE;//SMLCK as source, Up mode to TACCR0
	TA0CCTL1 |= OUTMOD2 | OUTMOD1 | OUTMOD0;// Reset/Set mode
	TA0CTL |= TACLR;//clear TAR
	TA0CCR0 = TA0CCR0MAX;
	//Sensor timer:
	TA1CTL |= TASSEL0 |MC1 |TAIE ;//ACLK as source, Up mode to 0xFFFFh, interrupt enabled
	TA1CCTL1 |= CM0 | CCIS0 | SCS | CAP |CCIE;//capture on rising edge, CCI0 as input|sync capture, interrupted
	TA1CTL |= TACLR;//clear TAR
}

void DriveSensorSetup(void){
	P2DIR &= ~(BIT0);
	P2SEL |= (BIT0);//TA0CCI0 input

}

void DriveStart(char speed){
	if (SPEED45 == speed){
		targetperiod = SPEED45REG;
	}
	else{
		targetperiod = SPEED33REG;
	}
	//turn on pin:
	P1DIR |= (BIT6);
	P1SEL |= (BIT6);
	drivework = ON;
}

void DriveSpeed(char speed){
	if (SPEED45 == speed){
		targetperiod = SPEED45REG;
	}
	else{
		targetperiod = SPEED33REG;
	}
}

void DriveStop(void){
	drivework = OFF;
	//turn off pin:
	P1SEL &= ~(BIT6);
	P1DIR &= ~(BIT6);
}

void CorrectSpeed(void){
	//called only if drivework is ON
	currenterror = currentperiod - targetperiod;
	integralspeed += (float)(currenterror);
	float  drivespeedtemp = KP * currenterror* (t_timer * currentperiod) + KI * integralspeed + KD * (currenterror - lasterror)/  (t_timer * currentperiod);
	if (drivespeedtemp > TA0MAX){
		drivespeedtemp = (float)(TA0MAX);
	}
	if (drivespeedtemp < 5){
		drivespeedtemp = (float)(5);
	}
	drivespeed = (short)(drivespeedtemp);
	TA1CCR1 = drivespeed;
	lasterror = currenterror;
}

// Timer A1 CCI service routine
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer_A_CCI (void)
{
	if (ON == drivework){
		currentperiod = TA1CCR0;
		CorrectSpeed();
	}
}

// Timer A1 OVF service routine
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer_A_OVF (void)
{
	if (ON == drivework){
		currentperiod = SENSORPERIODMAX;
		CorrectSpeed();
	}
}



