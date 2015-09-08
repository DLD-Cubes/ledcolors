/*
2  * Author: Brendan Le Foll <brendan.le.foll@intel.com>
3  * Copyright (c) 2015 Intel Corporation.
4  *
5  * Permission is hereby granted, free of charge, to any person obtaining
6  * a copy of this software and associated documentation files (the
7  * "Software"), to deal in the Software without restriction, including
8  * without limitation the rights to use, copy, modify, merge, publish,
9  * distribute, sublicense, and/or sell copies of the Software, and to
10  * permit persons to whom the Software is furnished to do so, subject to
11  * the following conditions:
12  *
13  * The above copyright notice and this permission notice shall be
14  * included in all copies or substantial portions of the Software.
15  *
16  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
17  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
18  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
19  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
20  * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
21  * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
22  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
23  */


 #include <stdio.h>
 #include <unistd.h>
 #include <signal.h>
 #include <math.h>


 // libmraa is the library used to interface with the IO on iot-devkit
 // platforms. You use it by including the top level mraa.h header in
 // /usr/include and linking your application against -lmraa.


 // Typically the compilation line would look something like:
 // $ gcc myprog.c -lmraa
#include "mraa.h"
#include "mraa.hpp"


int running = 1;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("closing PWM nicely\n");
        running = -1;
    }
}

class ledColor
{
	float _red;
	float _green;
	float _blue;
    mraa::Pwm* _pwmR;
    mraa::Pwm* _pwmG;
    mraa::Pwm* _pwmB;

    float convertColor(int color)
    {
    	float out= (1.0f/255)*color;
    	return out;
    }

public:

	ledColor()
	{

	   char* platform_name = mraa_get_platform_name();
	   mraa_platform_t platform_type = mraa_get_platform_type();
	   const char* version = mraa_get_version();


	   fprintf(stdout, "hello mraa\n Version: %s\n Running on %s (%d)\n",
	            version, platform_name, platform_type);

	   _pwmR = new mraa::Pwm(3);
	   _pwmG = new mraa::Pwm(5);
	   _pwmB = new mraa::Pwm(6);

	   if ((_pwmR == NULL) || (_pwmR == NULL) || (_pwmR == NULL)) {
	    	     fprintf(stdout, "Error in pwm");
	         }
	         fprintf(stdout, "Sending colors to led \n");
	         _pwmR->enable(true);
	         _pwmG->enable(true);
	         _pwmB->enable(true);

	   _red = 0;
	   _green = 0;
	   _blue = 0;
	}


	~ledColor()
	{
        delete _pwmR;
        delete _pwmG;
        delete _pwmB;

	}

	void displayColorInt(int r, int g, int b, bool commonCathode)
	{
		//in common anode high => off low => on
		//in common cathode high => on low => off
		_red = convertColor(r);
		_green = convertColor(g);
		_blue = convertColor(b);
		if (!commonCathode)
		{
			_red = 1 - _red;
			_green = 1 - _green;
			_blue = 1 - _blue;
		}
		fprintf(stdout, "print values \n");
		fprintf(stdout, "%f, %f, %f", _red, _green, _blue);
        _pwmR->write(_red);
        _pwmB->write(_blue);
        _pwmG->write(_green);
	}

	void displayColor(float r, float g, float b, bool commonCathode)
	{
		//in common anode high => off low => on
		//in common cathode high => on low => off

		if (commonCathode)
		{
			_red = r;
			_green = g;
			_blue = b;
		}
		else
		{
			_red = 1 - r;
			_green = 1 - g;
			_blue = 1 - b;
		}
		_pwmR->write(_red);
		_pwmB->write(_blue);
	    _pwmG->write(_green);
	}

	void turnOfLed(bool commonCathode)
	{
		//in common anode high => off low => on
	    //in common cathode high => on low => off
		if (commonCathode)
		{
			_red = 0;
			_green = 0;
			_blue = 0;
		}
		else
		{
			_red = 1;
			_green = 1;
			_blue = 1;
		}
		_pwmR->write(_red);
		_pwmB->write(_blue);
		_pwmG->write(_green);
	}

};


 int main(int argc, char **argv)
 {

	 for (int i=0; i<10; i++)
	 {
	 //create a ledcolor instance
	 ledColor* led = new ledColor;
	 //display white
	 fprintf(stdout, "display red \n");
	 led->displayColor(1,1,1,false);
	 usleep(1000000);
	 //display white
     fprintf(stdout, "all turned off \n");
     led->displayColor(0,0,0,false);
	 usleep(1000000);
	 //display red
	 fprintf(stdout, "display red \n");
	 led->displayColor(1,0,0,false);
	 usleep(1000000);
	 //display blue
	 fprintf(stdout, "display blue \n");
	 led->displayColor(0,0,1,false);
	 usleep(1000000);
	 //display green
	 fprintf(stdout, "display green \n");
	 led->displayColor(0,1,0,false);
	 usleep(1000000);
	 //display color magenta
	 fprintf(stdout, "display magenta \n");
	 led->displayColorInt(128,255,0,false);
	 usleep(1000000);
	 //display color magenta
	 fprintf(stdout, "display purple \n");
	 led->displayColorInt(128,0,255,false);
	 usleep(1000000);
	 //display color orange
     fprintf(stdout, "display orange \n");
	 led->displayColorInt(237,120,6,false);
	 usleep(1000000);
	 //display color orange
	 fprintf(stdout, "display pink \n");
	 led->displayColorInt(175,75,148,false);
	 usleep(1000000);

	 led->turnOfLed(false);

	 }
     //write custom colors
     /*signal(SIGINT, sig_handler);


         fprintf(stdout, "Sending 0 \n");
         usleep(1000000);
         fprintf(stdout, "Sending 1 \n");
         pwmR->write(1);
         pwmB->write(1);
         pwmG->write(1);
*/
     return MRAA_SUCCESS;
 }
