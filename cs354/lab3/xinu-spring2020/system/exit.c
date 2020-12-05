/* exit.c - exit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  exit  -  Cause the calling process to exit
 *------------------------------------------------------------------------
 */
void	exit(void)
{
  kprintf("GETPID before kill\n");
	kill(getpid());		/* Kill the current process */
}
