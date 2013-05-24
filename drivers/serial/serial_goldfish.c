/******************************************************************************
 *
 * serial_goldfish.c - header file for goldfish serial port
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
#include <common.h>
#include <serial.h>
#include "serial_goldfish.h"

void printascii(const char *s);
void *getbaseaddr(void);
static struct goldfish_tty gtty = {0, 0};

/*
void default_serial_puts(const char *s)
{
	printascii(s);
}
*/

static int goldfish_init(void)
{
	gtty.base = getbaseaddr();
	debug ("goldfish_init(), gtty.base=%x\n", gtty.base);
	return 0;
}

static void goldfish_putc(const char c)
{
	char tmp[2];

#if 0
	if (c == '\n') {
		tmp[0] = '\r';
	}
	else {
		tmp[0] = c;
	}
#endif

	tmp[0] = c;
	tmp[1] = 0;
	printascii(tmp);
}

static int goldfish_getc(void)
{
	char buf[2];
	uint32_t count;
	unsigned int data = 0;
	void *base = 0;

	base = gtty.base;
	if(!base) {
		gtty.base = getbaseaddr();
		base = gtty.base;
	}
	debug ("goldfish_getc(), gtty.base=%x, base=%x\n", gtty.base, base);
	if(base) {
		count = readl(base + GOLDFISH_TTY_BYTES_READY);
		if(count == 0) {
			return -1;
		}

		buf[0] = 0;
		buf[1] = 0;

		writel((uint32_t)buf, base + GOLDFISH_TTY_DATA_PTR);
		writel(count, base + GOLDFISH_TTY_DATA_LEN);
		writel(GOLDFISH_TTY_CMD_READ_BUFFER, base + GOLDFISH_TTY_CMD);
		data = buf[0];
	}

	return (int) data;
}

static int goldfish_tstc(void)
{
	int count = 0;
	void *base = 0;

	base = gtty.base;
	if(!base) {
		gtty.base = getbaseaddr();
		base = gtty.base;
	}

	if(base) {
		count = readl(gtty.base + GOLDFISH_TTY_BYTES_READY);
		if(count) {
			debug ("goldfish_tstc(), gtty.base=%x, base=%x, count=%d\n", gtty.base, base, count);
		}
	}

	return count;
}

static void goldfish_setbrg(void)
{
	default_serial_puts("goldfish_setbrg()\n");
}

static struct serial_device goldfish_drv = {
	.name	= "goldfish_serial",
	.start	= goldfish_init,
	.stop	= NULL,
	.setbrg	= goldfish_setbrg,
	.putc	= goldfish_putc,
	.puts	= default_serial_puts,
	.getc	= goldfish_getc,
	.tstc	= goldfish_tstc,
};

void goldfish_initialize(void)
{
	default_serial_puts("goldfish_initialize()\n");
	// serial_register(&goldfish_drv);
}

struct serial_device *default_serial_console(void)
{
	return &goldfish_drv;
}
