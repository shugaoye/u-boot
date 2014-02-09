/*
 * (C) Copyright 2013
 * Roger Ye, <shugaoye@yahoo.com>
 * Board support functions.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>
#include <configs/goldfish.h>


DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USE_IRQ
struct _irq_handler {
	void                *m_data;
	void (*m_func)( void *data);
};

static struct _irq_handler IRQ_HANDLERS[N_IRQS];
#endif /* CONFIG_USE_IRQ */

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

#define COMP_MODE_ENABLE ((unsigned int)0x0000EAEF)

/*
 * Miscellaneous platform dependent initializations
 */

int board_early_init_f (void)
{
	return 0;
}

int board_init (void)
{
	/* arch number of GOLDFISH Board */
	gd->bd->bi_arch_number = MACH_TYPE_GOLDFISH;

	/* address of boot parameters */
	gd->bd->bi_boot_params = 0x00000100;

	gd->flags = 0;

	icache_enable ();

	return 0;
}

int misc_init_r (void)
{
	setenv("verify", "n");
	return (0);
}

/******************************
 Routine:
 Description:
******************************/
int dram_init (void)
{
	/* dram_init must store complete ramsize in gd->ram_size */
	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
				PHYS_SDRAM_1_SIZE);
	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC91111
	rc = smc91111_initialize(0, CONFIG_SMC91111_BASE);
#endif
	return rc;
}
#endif

#ifdef CONFIG_USE_IRQ
/*
 * GOLDFISH_INTERRUPT_DISABLE - at offset 0xC is a write-only register.
 * Writing an interrupt number to it will disable to specified interrupt.
 * */
void goldfish_mask_irq(unsigned int irq)
{
	writel(irq, (void *)IO_ADDRESS(GOLDFISH_INTERRUPT_BASE) + GOLDFISH_INTERRUPT_DISABLE);
}

/*
 * GOLDFISH_INTERRUPT_ENABLE - at offset 0x10 is a write-only register.
 * Writing an interrupt number to it will enable to specified interrupt.
 * */
void goldfish_unmask_irq(unsigned int irq)
{
	writel(irq, (void *)IO_ADDRESS(GOLDFISH_INTERRUPT_BASE) + GOLDFISH_INTERRUPT_ENABLE);
}

/*
 * GOLDFISH_INTERRUPT_DISABLE_ALL - at offset 0x8 is a write-only register.
 * Writing any value other than 0 to it will disable all interrupts.
 * */
void goldfish_disable_all_irq(void)
{
	writel(1, (void *)IO_ADDRESS(GOLDFISH_INTERRUPT_BASE) + GOLDFISH_INTERRUPT_DISABLE_ALL);
}

/*
 * GOLDFISH_INTERRUPT_NUMBER - at offset 0x4 contains the lowest pending,
 * enabled interrupt number. It is a read-only register.
 * */
int goldfish_get_irq_num(void)
{
	return readl((void *)IO_ADDRESS(GOLDFISH_INTERRUPT_BASE) + GOLDFISH_INTERRUPT_NUMBER);
}

/*
 * GOLDFISH_INTERRUPT_STATUS - at offset 0x0 contains the number of pending interrupt.
 * It is a read-only register.
 * */
int goldfish_irq_status(void)
{
	return readl((void *)IO_ADDRESS(GOLDFISH_INTERRUPT_BASE) + GOLDFISH_INTERRUPT_STATUS);
}

static void default_isr(void *data)
{
	printf("default_isr():  called for IRQ %d\n", (int)data);
}

static int next_irq(void)
{
	return goldfish_irq_status();
}

void do_irq (struct pt_regs *pt_regs)
{
	int irq = next_irq();

	IRQ_HANDLERS[irq].m_func(IRQ_HANDLERS[irq].m_data);
}

void irq_install_handler (int irq, interrupt_handler_t handle_irq, void *data)
{
	if (irq >= N_IRQS || !handle_irq)
		return;

	IRQ_HANDLERS[irq].m_data = data;
	IRQ_HANDLERS[irq].m_func = handle_irq;
}

int arch_interrupt_init (void)
{
	int i;

	/* install default interrupt handlers */
	for (i = 0; i < N_IRQS; i++)
		irq_install_handler(i, default_isr, (void *)i);

	return (0);
}
#endif /* CONFIG_USE_IRQ */
