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
#include <system.h>

char systemstate;


void SystemStopToRun(void){
	//start motion
	if (HSPEED == ButtonHSpeedRead()){
		DriveStart(SPEED45);
		SystemWriteState(RUN45);
	}
	else{
		DriveStart(SPEED33);
		SystemWriteState(RUN33);
	}
	//tonarm On
	TonarmWrite(ON);
	//elevator solenoid on
	ElevatorWrite(ON);
}

void SystemChangeSpeed(char speed){
	if (SPEED45 == speed){
		DriveStart(SPEED45);
		SystemWriteState(RUN45);
	}
	else{
		DriveStart(SPEED33);
		SystemWriteState(RUN33);
	}
}

void SystemRunToStop(void){
	ElevatorWrite(OFF);

	TonarmWrite(OFF);

	DriveStop();

	SystemWriteState(STOPPED);

}

char SystemReadState(void){

	return systemstate;
}

char SystemWriteState(char command){
	systemstate = command;
	return systemstate;
}

char SystemCheck(void){
	switch (SystemReadState()){//STOPPPED, RUN33, RUN45
	    	case STOPPED:
	    		if (START == ButtonStartRead()){
	    			SystemStopToRun();
	    		}
	    		break;

	    	case RUN33:
	    		//if HSpeed button pressed
	    		if (HSPEED == ButtonHSpeedRead()){
	    			SystemChangeSpeed(SPEED45);
	    		}
	    		else{//stop button or Auto Switch plus autostop sensor
	    			if ((STOP == ButtonStopRead())||((AUTO == ButtonAutoRead())&& (STOP == StopSensorRead()))){
	    				SystemRunToStop();
	    			}
	    			else{

	    			}
	    		}
	    		break;

	    	case RUN45:
	    		//if Hspeed button released
	    		if (HSPEED != ButtonHSpeedRead()){
	    			SystemChangeSpeed(SPEED33);
	    		}
	    		else{//stop button or Auto Switch plus autostop sensor
	    			if ((STOP == ButtonStopRead())||((AUTO == ButtonAutoRead())&& (STOP == StopSensorRead()))){
	    				SystemRunToStop();
	    			}
	    			else{

	    			}
	    		}
	    		break;

	    	default:

	    		break;
	    	}
	return systemstate;
}
