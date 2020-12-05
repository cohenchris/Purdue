#include<stdio.h>
#include<wiringPi.h>
#include<softPwm.h>

#define RBUTTON 21
#define GBUTTON 22
#define BBUTTON 23
#define INCREASE 24
#define DECREASE 25
#define RLED 27
#define GLED 28
#define BLED 29

int main(void) {
	/* Makes sure GPIO pins can be initialzied */
	if (wiringPiSetup() == -1) {
		return 1;
	}

	/* Set all buttons (left to right) to inputs */
	pinMode(RBUTTON, INPUT);
	pinMode(GBUTTON, INPUT);
	pinMode(BBUTTON, INPUT);
	pinMode(INCREASE, INPUT);
	pinMode(DECREASE, INPUT);

	/* Set LEDs to outputs */
	pinMode(RLED, OUTPUT);
	pinMode(GLED, OUTPUT);
	pinMode(BLED, OUTPUT);

	/* Create a software controlled PWM pin for each LED.
	 * Returns 1 (error) if any of the creations fail. */
	if (softPwmCreate(RLED, 0, 100) != 0) {
		return 1;
	}
	if (softPwmCreate(GLED, 0, 100) != 0) {
		return 1;
	}
	if (softPwmCreate(BLED, 0, 100) != 0) {
		return 1;
	}

	/* Values to keep track of the current led brightness */
	int redVal = 0;
	int greenVal = 0;
	int blueVal = 0;
	
	while (1) {
		/* RBUTTON Controls */
		if (digitalRead(RBUTTON) == LOW && digitalRead(INCREASE) == LOW) {		//if red + increase is pressed...
			softPwmWrite(RLED, redVal + 5);		//increases the red led brightness value by 5
			redVal += 5;				//updates the red led value to make sure it's saved
			delay(100);
		}
		else if (digitalRead(RBUTTON) == LOW && digitalRead(DECREASE) == LOW) {		//if red + decrease is pressed...
			softPwmWrite(RLED, redVal - 5);		//decreases the red led brightness value by 5
			redVal -= 5;				//updates red led value to make sure it's saved
			delay(100);
		}
	
		/* GBUTTON Controls */
		if (digitalRead(GBUTTON) == LOW && digitalRead(INCREASE) == LOW) {		//if green + increase is pressed...
			softPwmWrite(GLED, greenVal + 5);	//increases the green led brightness value by 5
			greenVal += 5;				//updates green led value to make sure it's saved
			delay(100);
		}
		else if (digitalRead(GBUTTON) == LOW && digitalRead(DECREASE) == LOW) {		//if green + decrease is pressed...
			softPwmWrite(GLED, greenVal - 5);	//decreases the green led brightness value by 5
			greenVal -= 5;				//updates green led value to make sure it's saved
			delay(100);
		}
	
		/* BBUTTON Controls */
		if (digitalRead(BBUTTON) == LOW && digitalRead(INCREASE) == LOW) {		//if blue + increase is pressed
			softPwmWrite(BLED, blueVal + 5);	//increases the blue led brightness value by 5
			blueVal += 5;				//updates the blue led value to make sure it's saved
			delay(100);
		}
		else if (digitalRead(BBUTTON) == LOW && digitalRead(DECREASE) == LOW) {		//if blue + decrease is pressed...
			softPwmWrite(BLED, blueVal - 5);	//decreases the blue led brightness value by 5
			blueVal -= 5;				//updates the blue led value to make sure it's saved
			delay(100);
		}
	}

	return 0;
}
