/******************************************************************************
 *
 * U-Boot support for Goldfish MMC
 *
 * Copyright (c) 2015 Roger Ye.  All rights reserved.
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
#include <asm/io.h>
#include <mmc.h>

#define DRIVER_NAME "goldfish_mmc"

#define BUFFER_SIZE   16384

#define GOLDFISH_MMC_READ(host, addr)   (readl(host->reg_base + addr))
#define GOLDFISH_MMC_WRITE(host, addr, x)   (writel(x, host->reg_base + addr))


enum {
	/* status register */
	MMC_INT_STATUS	        = 0x00,
	/* set this to enable IRQ */
	MMC_INT_ENABLE	        = 0x04,
	/* set this to specify buffer address */
	MMC_SET_BUFFER          = 0x08,

	/* MMC command number */
	MMC_CMD	                = 0x0C,

	/* MMC argument */
	MMC_ARG	                = 0x10,

	/* MMC response (or R2 bits 0 - 31) */
	MMC_RESP_0		        = 0x14,

	/* MMC R2 response bits 32 - 63 */
	MMC_RESP_1		        = 0x18,

	/* MMC R2 response bits 64 - 95 */
	MMC_RESP_2		        = 0x1C,

	/* MMC R2 response bits 96 - 127 */
	MMC_RESP_3		        = 0x20,

	MMC_BLOCK_LENGTH        = 0x24,
	MMC_BLOCK_COUNT         = 0x28,

	/* MMC state flags */
	MMC_STATE               = 0x2C,

	/* MMC_INT_STATUS bits */

	MMC_STAT_END_OF_CMD     = 1U << 0,
	MMC_STAT_END_OF_DATA    = 1U << 1,
	MMC_STAT_STATE_CHANGE   = 1U << 2,
	MMC_STAT_CMD_TIMEOUT    = 1U << 3,

	/* MMC_STATE bits */
	MMC_STATE_INSERTED      = 1U << 0,
	MMC_STATE_READ_ONLY     = 1U << 1,
};

/*
 * Command types
 */
#define OMAP_MMC_CMDTYPE_BC	0
#define OMAP_MMC_CMDTYPE_BCR	1
#define OMAP_MMC_CMDTYPE_AC	2
#define OMAP_MMC_CMDTYPE_ADTC	3

struct goldfish_mmc_host {
#if 0
	struct mmc_request *	mrq;
	struct mmc_command *	cmd;
	struct mmc_data *		data;
	struct mmc_host *		mmc;
	struct device *			dev;
	unsigned char			id; /* 16xx chips have 2 MMC blocks */
	void __iomem			*virt_base;
	unsigned int			phys_base;
	int						irq;
	unsigned char			bus_mode;
	unsigned char			hw_bus_mode;

	unsigned int			sg_len;
	unsigned				dma_done:1;
	unsigned				dma_in_use:1;
#endif
	void __iomem			*reg_base;
};

/* Global variables for MMC support */
struct mmc g_mmc_dev;
struct goldfish_mmc_host g_mmc_host;

static void mmc_prepare_data(struct goldfish_mmc_host *host, struct mmc_data *data)
{
	debug(" mmc_prepare_data called\n");
}

static int mmc_send_cmd(struct mmc *my_mmc_dev, struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	struct goldfish_mmc_host *host = (struct goldfish_mmc_host *)my_mmc_dev->priv;
	int result;

	debug(" mmc_send_cmd called\n");

	if (result < 0)
		return result;

	if (data)
		mmc_prepare_data(host, data);

	debug("cmd->arg: %08x\n", cmd->cmdarg);

	if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
		return -1;

	udelay(1000);
	return 0;
}

static void mmc_set_ios(struct mmc *my_mmc_dev)
{
	struct goldfish_mmc_host *host = my_mmc_dev->priv;

	debug(" mmc_set_ios called\n");

	debug("bus_width: %x, clock: %d\n", my_mmc_dev->bus_width, my_mmc_dev->clock);
}

static int goldfish_mmc_getcd(struct mmc *my_mmc_dev)
{
	struct goldfish_mmc_host *host = (struct goldfish_mmc_host *)my_mmc_dev->priv;

	debug("goldfish_mmc_getcd called\n");

	return 1;
}

static int mmc_core_init(struct mmc *my_mmc_dev)
{
	struct goldfish_mmc_host *host = (struct goldfish_mmc_host *)my_mmc_dev->priv;
	unsigned int mask;
	debug(" mmc_core_init called\n");

	return 0;
}

/* this is a weak define that we are overriding */
int board_mmc_init(bd_t *bd)
{
	struct goldfish_mmc_host *host = &g_mmc_host;
	struct mmc *my_mmc_dev = &g_mmc_dev;

	debug("board_mmc_init called\n");

	sprintf(my_mmc_dev->name, DRIVER_NAME);
	my_mmc_dev->priv = host;
	my_mmc_dev->send_cmd = mmc_send_cmd;
	my_mmc_dev->set_ios = mmc_set_ios;
	my_mmc_dev->init = mmc_core_init;
	my_mmc_dev->getcd = goldfish_mmc_getcd;

	mmc_register(my_mmc_dev);

	return 0;
}
