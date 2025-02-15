/* clock.h */

/* Intel 8254-2 clock chip constants */

#define	CLOCKBASE	0x40		/* I/O base port of clock chip	*/
#define	CLOCK0		CLOCKBASE
#define	CLKCNTL		(CLOCKBASE+3)	/* chip CSW I/O port		*/


#define CLKTICKS_PER_SEC  1000	/* clock timer resolution		*/

extern	uint32	clktime;	/* current time in secs since boot	*/

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	uint32	preempt;	/* preemption counter			*/

// Added by cohen50 2-25-20 for Lab 3
extern  uint32  clktimemilli;           /* Number of system timer interrupts  */
extern  uint32  currentgrosscpu;        /* Number ms currproc has used        */
extern  uint64  currentgrosscputick;    /* Number ticks currproc has used     */
