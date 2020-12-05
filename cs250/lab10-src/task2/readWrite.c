#include<stdio.h>
#include<wiringPi.h>

/* Set Pin Numbers */
#define LED 28
#define BUTTON 29

int main(void) {
	/* Initialize GPIO pins to map to wiringPi numbers */
if (wiringPiSetup() == -1)
	return 1; //return with error status (initialization failed)

	/* Set modes for the GPIO pins */
pinMode(LED, OUTPUT);
pinMode(BUTTON, INPUT);

int value = LOW;
while(1) {
	/* Read from the pin number defined for button */
value = digitalRead(BUTTON);

	/* Write the value to positive pin of the led */
digitalWrite(LED, value);

	/* Use of delay is important as there is a small lag in
	 * reading/writing values each time */
delay(100);
}

return 0;
}
