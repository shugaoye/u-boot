/*
 * Copyright (c) 2014 Roger Ye.  All rights reserved.
 * Roger Ye <shugaoye@yahoo.com>
 * Configuration settings for google Android emulator
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __GOLDFISH_CONFIG_H
#define __GOLDFISH_CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM926EJS	1	/* This is an arm926ejs CPU core */
#define CONFIG_VERSATILE	1	/* in Versatile Platform Board	*/
#define CONFIG_ARCH_VERSATILE	1	/* Specifically, a Versatile	*/
#define CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ (4*1024)
#define CONFIG_STACKSIZE_FIQ (4*1024)

#define CONFIG_SYS_MEMTEST_START	0x100000
#define CONFIG_SYS_MEMTEST_END		0x10000000
#define CONFIG_SYS_HZ				100000
/* #define CONFIG_SYS_HZ				(1000000 / 256) */
#define CONFIG_SYS_TIMERBASE		IO_ADDRESS(GOLDFISH_TIMER_BASE)	/* Timer base */

#define CONFIG_SYS_TIMER_INTERVAL	10000
#define CONFIG_SYS_TIMER_RELOAD		(CONFIG_SYS_TIMER_INTERVAL >> 4)
#define CONFIG_SYS_TIMER_CTRL		0x84		/* Enable, Clock / 16 */

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_MISC_INIT_R		1
/*
 * Size of malloc() pool
 */
#define CONFIG_ENV_SIZE				8192
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 4 * 1024 * 1024)

/*
 * Hardware drivers
 */

#define CONFIG_SMC91111
#define CONFIG_SMC_USE_32_BIT
#define CONFIG_SMC91111_BASE		IO_ADDRESS(GOLDFISH_smc91x_BASE)
/* #undef CONFIG_SMC91111_EXT_PHY */
#define CONFIG_SMC91111_EXT_PHY

/*
 *  GOLDFISH serial
 */
#define CONFIG_GOLDFISH_SERIAL

#define CONFIG_CONS_INDEX	0

#define CONFIG_BAUDRATE			38400
#define CONFIG_SYS_SERIAL0		IO_ADDRESS(GOLDFISH_TTY1_BASE)
#define CONFIG_SYS_SERIAL1		IO_ADDRESS(GOLDFISH_TTY2_BASE)

/*
 * Command line configuration.
 */
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_FLASH
#define CONFIG_CMD_IMI
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_NAND		/* NAND support			*/
#define CONFIG_CMD_DATE

/*
 * Goldfish MMC Support
 */
#define CONFIG_MMC			1
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_GOLDFISH_MMC

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SUBNETMASK

#define CONFIG_BOOTDELAY	2
/* #define CONFIG_BOOTARGS		"root=/dev/nfs mem=128M ip=dhcp "\
				"netdev=25,0,0xf1010000,0xf1010010,eth0"
#define CONFIG_BOOTARGS "qemu.gles=1 qemu=1 console=ttyS0 android.qemud=ttyS1 androidboot.console=ttyS2 android.checkjni=1 ndns=1 root=/dev/ram mem=512M rdinit=/sbin/init"
*/
#define CONFIG_BOOTARGS "qemu.gles=1 qemu=1 console=ttyS0 android.qemud=ttyS1 androidboot.console=ttyS2 android.checkjni=1 ndns=1"
#define CONFIG_BOOTCOMMAND "bootm 0x210000 0x410000"
#define CONFIG_INITRD_TAG 1

/*
 * Static configuration when assigning fixed address
 */
#define CONFIG_BOOTFILE		"/tftpboot/uImage" /* file to load */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory */
#define CONFIG_SYS_CBSIZE	256		/* Console I/O Buffer Size */
/* Monitor Command Prompt	 */
# define CONFIG_SYS_PROMPT	"Goldfish # "
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE	\
			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_LOAD_ADDR	0x7fc0	/* default load address */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	/* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x00000000	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x20000000	/* 512 MB */
#define PHYS_FLASH_SIZE		0x04000000	/* 64MB */

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_INIT_RAM_ADDR	0x00800000
#define CONFIG_SYS_INIT_RAM_SIZE	0x000FFFFF
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - \
						GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_INIT_RAM_ADDR + \
						CONFIG_SYS_GBL_DATA_OFFSET)

#define CONFIG_BOARD_EARLY_INIT_F

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_TEXT_BASE		0x10000
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SYS_MONITOR_LEN		0x80000

/*
 * NAND configuration
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_NAND_GOLDFISH
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SYS_MAX_NAND_DEVICE	3		/* Max number of NAND devices. */
#endif

#define MTDIDS_DEFAULT          "nand0=system_nand,nand1=data_nand,nand2=cache_nand"
#define MTDPARTS_DEFAULT        "mtdparts=system_nand:197m(system);data_nand:194m(data);cache_nand:64m(cache)"

/*
 * File system
 */
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_YAFFS2
#define CONFIG_RBTREE
#define CONFIG_LZO

#define YAFFS2_DEBUG

/*
 * RTC driver configuration
 */
#ifdef CONFIG_CMD_DATE
#define CONFIG_RTC_GOLDFISH
#endif /* CONFIG_CMD_DATE */

/*
 * goldfish IO address definition, refer to <mach/hardware.h> in goldfish Linux kernel
 * Where in virtual memory the IO devices (timers, system controllers
 * and so on)
 */
/* include/asm-arm/arch-goldfish/irqs.h */
#define IRQ_PDEV_BUS    (1)
#define IRQ_TIMER       (3)
#define IRQ_TTY0		(4)
#define IRQ_RTC			(10)
#define IRQ_TTY1		(11)
#define IRQ_TTY2		(12)
#define IRQ_smc91x		(13)
#define IRQ_FB			(14)
#define IRQ_AUDIO		(15)
#define IRQ_EVENTS		(16)
#define IRQ_PIPE		(17)
#define IRQ_SWITCH0		(18)
#define IRQ_SWITCH1		(19)
#define IRQ_RANDOM 		(20)

#define LAST_IRQ RANDOM_IRQ
#define N_IRQS 21

/*
 * Where in virtual memory the IO devices (timers, system controllers
 * and so on)
 */
#define IO_BASE			0xfe000000                 // VA of IO
#define IO_SIZE			0x00800000                 // How much?
#define IO_START		0xff000000                 // PA of IO

#define GOLDFISH_INTERRUPT_BASE         (0x0)
#define GOLDFISH_INTERRUPT_STATUS       (0x00) // number of pending interrupts
#define GOLDFISH_INTERRUPT_NUMBER       (0x04)
#define GOLDFISH_INTERRUPT_DISABLE_ALL  (0x08)
#define GOLDFISH_INTERRUPT_DISABLE      (0x0c)
#define GOLDFISH_INTERRUPT_ENABLE       (0x10)

#define GOLDFISH_PDEV_BUS_BASE      (0x1000)
#define GOLDFISH_PDEV_BUS_END       (0x100)

#define GOLDFISH_TTY_BASE       (0x2000)
#define GOLDFISH_TIMER_BASE     (0x3000)
#define GOLDFISH_AUDIO_BASE     (0x4000)
#define GOLDFISH_MEMLOG_BASE    (0x6000)
#define GOLDFISH_RTC_BASE       (0x10000)
#define GOLDFISH_TTY1_BASE      (0x11000)
#define GOLDFISH_TTY2_BASE      (0x12000)
#define GOLDFISH_smc91x_BASE    (0x13000)
#define GOLDFISH_FB_BASE        (0x14000)
#define GOLDFISH_NAND_BASE      (0x17000)
#define GOLDFISH_SWITCH0_BASE   (0X19000)
#define GOLDFISH_SWITCH1_BASE   (0x1a000)

/* macro to get at IO space when running virtually */
#if 1
	#define IO_ADDRESS(x) ((x) + IO_START)
#else
	#define IO_ADDRESS(x) ((x) + IO_BASE)
#endif

#endif	/* __GOLDFISH_CONFIG_H */
