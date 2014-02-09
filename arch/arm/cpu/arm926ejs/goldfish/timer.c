/******************************************************************************
 *
 * timer.c - timer implementation for goldfish
 *
 * Copyright (c) 2013 Roger Ye.  All rights reserved.
 * Software License Agreement
 *
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
 * NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
 * NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. The AUTHOR SHALL NOT, UNDER
 * ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *****************************************************************************/

#ifndef __BARE_METAL__
#include <common.h>
#include <asm/io.h>
#include <configs/goldfish.h>
#else
#include <hardware.h>
#include <bsp.h>
#endif /* __BARE_METAL__ */

/* Refer to arch/arm/mach-goldfish/include/mach/timer.h */
enum {
	TIMER_TIME_LOW          = 0x00,
	TIMER_TIME_HIGH         = 0x04,
	TIMER_ALARM_LOW         = 0x08,
	TIMER_ALARM_HIGH        = 0x0c,
	TIMER_CLEAR_INTERRUPT   = 0x10,
	TIMER_CLEAR_ALARM       = 0x14,
};


#define TIMER_LOAD_VAL 0xffffffff

#ifndef __BARE_METAL__
DECLARE_GLOBAL_DATA_PTR;

#define timestamp gd->tbl
#define lastdec gd->lastinc
#else
ulong timestamp;
ulong lastdec;
#endif /* __BARE_METAL__ */

int timer_init (void)
{
	/* initialize the timestamp and lastdec value */
	reset_timer_masked();

	return 0;
}

/*
 * timer without interrupts
 */
ulong get_timer (ulong base)
{
	return get_timer_masked () - base;
}

/* delay x useconds AND preserve advance timestamp value */
void __udelay (unsigned long usec)
{
	ulong tmo, tmp;

#if 0
	if(usec >= 1000){					/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;				/* start to normalize for usec to ticks per sec */
		tmo *= CONFIG_SYS_HZ;			/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;					/* finish normalize. */
	}else{								/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CONFIG_SYS_HZ;
		tmo /= (1000*1000);
	}
#else
	tmo = usec * CONFIG_SYS_HZ;
#endif

	tmp = get_timer (0);				/* get current timestamp */
	if( (tmo + tmp + 1) < tmp ) {		/* if setting this forward will roll time stamp */
		reset_timer_masked ();			/* reset "advancing" timestamp to 0, set lastdec value */
	}
	else
		tmo += tmp;						/* else, set advancing stamp wake up time */

	while (get_timer_masked () < tmo)	/* loop till event */
		/*NOP*/;
}

void reset_timer_masked (void)
{
	uint32_t timer_base = IO_ADDRESS(GOLDFISH_TIMER_BASE);
	ulong rv;

	rv = readl(timer_base + TIMER_TIME_LOW);
	rv |= (int64_t)readl(timer_base + TIMER_TIME_HIGH) << 32;

	/* reset time */
	lastdec = rv;  	/* capture current decrementer value time */
	timestamp = 0;	/* start "advancing" time stamp from 0 */
}

ulong get_timer_masked (void)
{
	uint32_t timer_base = IO_ADDRESS(GOLDFISH_TIMER_BASE);
	ulong now = 0;				/* current tick value */

	now = readl(timer_base + TIMER_TIME_LOW);
	now |= (ulong)readl(timer_base + TIMER_TIME_HIGH) << 32;

	if (now >= lastdec) {		/* normal mode (non roll) */
		/* normal mode */
		timestamp += now - lastdec; /* move stamp forward with absolute diff ticks */
		/* printf("lastdec(%u) >= now(%u), timestamp=%u\n", lastdec, now, timestamp); */
	} else {						/* we have overflow of the count down timer */
		/* nts = ts + ld + (TLV - now)
		 * ts=old stamp, ld=time that passed before passing through -1
		 * (TLV-now) amount of time after passing though -1
		 * nts = new "advancing time stamp"...it could also roll and cause problems.
		 */
		timestamp += now + TIMER_LOAD_VAL - lastdec;
		/* printf("lastdec(%u) < now(%u) (overflow), timestamp=%u\n", lastdec, now, timestamp); */
	}
	lastdec = now;

	return timestamp;
}

/* waits specified delay value and resets timestamp */
void udelay_masked (unsigned long usec)
{
	ulong tmo;
	ulong endtime;
	signed long diff;

#if 0
	if (usec >= 1000) {		/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;	/* start to normalize for usec to ticks per sec */
		tmo *= CONFIG_SYS_HZ;		/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;		/* finish normalize. */
	} else {			/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CONFIG_SYS_HZ;
		tmo /= (1000*1000);
	}
#else
	tmo = usec * CONFIG_SYS_HZ;
#endif

	endtime = get_timer_masked () + tmo;

	do {
		ulong now = get_timer_masked ();
		diff = endtime - now;
	} while (diff >= 0);
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk (void)
{
	ulong tbclk;

	tbclk = CONFIG_SYS_HZ;
	return tbclk;
}
