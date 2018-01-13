/*
 * clock.c
 *
 *  Created on: 2017��12��15��
 *      Author: hailong.li
 */
#include "derivative.h"

/*
 * initialize FXOSC
 */
static void FXOSC_Init(void)
{
	XOSC.CTL.R = 0x00FF0080u;
}

/*
 * initialize SXOSC
 */
static void SXOSC_Init(void)
{
	SXOSC.CTL.R = 0x40FF0000u;
}

/*
 * Disable CMU.
 */
static void CMU_Init(void)
{
	CMU.CSR.R = 0;
}

static void CGM_Init(void)
{
	MC_CGM.CLKOUT1_SC.R = 0;
	MC_CGM.CLKOUT1_DC.R = 0x80000000u;
	MC_CGM.AC2_SC.R		= 0x0u;				// select the current clock source for the ENET0 and ENET1 timer clock. F40
	MC_CGM.AC4_SC.R		= 0x0u;				// select the current clock source for the uSDHC clock. F40
	MC_CGM.AC5_SC.R 	= 0x01000000u;		// select FXOSC as clock source for PLL.
	MC_CGM.AC6_SC.R		= 0x0u;				// select the current clock source for the CLKOUT_0. FXOSC
	MC_CGM.AC8_SC.R		= 0x0u;				// select clock source for the SPI0(F40).
	MC_CGM.AC9_SC.R		= 0x0u;				// select clock source for the FlexCAN0(FS80).

	MC_CGM.SC_DC0.R		= 0x80000000u;		// controls system clock divider 0 - S160 clock. divided by 1.
	MC_CGM.SC_DC1.R		= 0x80010000u;		// control selected modules working on system clock divider 1 - S80 clock.
	MC_CGM.SC_DC2.R		= 0x80030000u;		// controls system clock divider 2 - S40 clock.	divided by 4.
	MC_CGM.SC_DC5.R		= 0x80010000u;		// controls system clock divider 5 - FS80 peripheral clock clock.

#if 0
	MC_CGM.PCS_SDUR.R	= 0x10u;			// duration of one PCS clock switch step in terms of 16MHz int. RC osc cycles.
	MC_CGM.PCS_DIVC1.R	= 0x03E70000u;		// default value.
	MC_CGM.PCS_DIVE1.R	= 0x000003E7u;		// default value.
#endif
}

void PLL_Init(void)
{
	PLLDIG.PLLCAL1.R 	= 0x0u;
	PLLDIG.PLLCAL3.R 	= 4095u << 17;
	PLLDIG.PLLCR.R		= 0x0u;
	PLLDIG.PLLDV.R		= (0x2u << 25) | (0x1u << 16) | (0x0u << 12) | 16u;
	PLLDIG.PLLFD.R		= 0x0u;
	PLLDIG.PLLFM.R		= 0x0u;
}

void ClockSource_Init(void)
{
	FXOSC_Init();
	SXOSC_Init();
	CMU_Init();
	CGM_Init();
	PLL_Init();
}
