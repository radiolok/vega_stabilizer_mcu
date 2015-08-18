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
#include "pinout.h"


void ButtonsSetup(void){

	//AutoStop select
	P1DIR &= ~(BIT6);

	//Speed33 select
	P1DIR &= ~(BIT7);

	//Start select
	P2DIR &= ~(BIT5);

	//Stop select
	P2DIR &= ~(BIT4);


}


char ButtonAutoRead(void){
	char status = NONE;

	if (0 == (P1IN & BIT6)){
		status = AUTO;
	}

	return status;
}

char ButtonStartRead(void){
	char status = NONE;

	if (0 == (P2IN & BIT5)){
		status = START;
	}

	return status;
}

char ButtonStopRead(void){
	char status = NONE;

	if (0 == (P2IN & BIT4)){
		status = STOP;
	}


	return status;
}

char ButtonHSpeedRead(void){
	char status = NONE;

	if (1 == (P1IN & BIT7)){
		status = HSPEED;
	}

	return status;
}


void ElevatorSetup(void){

	//output
	P2DIR |= (BIT3);

}

char ElevatorRead(void){//OFF or ON
	char status = OFF;
	if (P2IN & BIT3){
		status = ON;
	}
	return status;
}

void ElevatorWrite(char status){// OFF or ON
	if (ON == status){
		P2OUT |= (BIT3);
	}
	else{
		P2OUT &= ~(BIT3);
	}
}

void TonarmSetup(void){

	P2DIR |= (BIT1);
	TonarmWrite(OFF);

}

char TonarmRead(void){
	char status = OFF;
	if (P2IN & BIT1){
		status = ON;
	}
	return status;
}

void TonarmWrite(char status){// OFF or ON
	if (OFF == status){
		P2OUT |= (BIT3);//we need to open trans to turn off
	}
	else{
		P2OUT &= ~(BIT3);
	}

}

void StopSensorSetup(void){

	P2DIR &= ~(BIT2);

}

char StopSensorRead(void){//NONE or STOP
	char status = LOW;
	if (P2IN & BIT2){
		status = HIGH;
	}
	return status;
}
