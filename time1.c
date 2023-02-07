#include "time1.h"
#include "syschip.h"
extern unsigned int time1Times;

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)  
{
	TMR1 = 0;
	IFS0bits.T1IF = 0;			// Clear Timer1 interrupt flag

	time1Times++;
}