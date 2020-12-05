/* clkinit.c - clkinit (x86) */

#include <xinu.h>

uint32	clktime;		/* Seconds since boot			*/
uint32	ctr1000 = 0;		/* Milliseconds since boot		*/
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/

// Added by cohen50 for Lab 3
uint32 clktimemilli;            /* Number of system timer interrupts    */
uint32 currentgrosscpu;         /* Number CPU ms currproc has used      */
uint64 currentgrosscputick;     /* Number CPU ticks currproc has used   */

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup (x86)
 *------------------------------------------------------------------------
 */
void	clkinit(void)
{
	uint16	intv;		/* Clock rate in KHz			*/

	/* Allocate a queue to hold the delta list of sleeping processes*/

	sleepq = newqueue();

	/* Initialize the preemption count */

	preempt = QUANTUM;

	/* Initialize the time since boot to zero */

	clktime = 0;

        // Added by cohen50 for Lab 3
        clktimemilli = 0;
        currentgrosscpu = 0;
        currentgrosscputick = getticks();
        // End added code by cohen50 for Lab 3

	/* Set interrupt vector for the clock to invoke clkdisp */

	set_evec(IRQBASE, (uint32)clkdisp);

	/* Set the hardware clock: timer 0, 16-bit counter, rate */
	/*   generator mode, and counter runs in binary		 */

	outb(CLKCNTL, 0x34);

	/* Set the clock rate to 1.190 Mhz; this is 1 ms interrupt rate */

        intv = 1193;	/* Using 1193 instead of 1190 to fix clock skew	*/

	/* Must write LSB first, then MSB */

	outb(CLOCK0, (char) (0xff & intv) );
	outb(CLOCK0, (char) (0xff & (intv>>8)));

	return;
}
