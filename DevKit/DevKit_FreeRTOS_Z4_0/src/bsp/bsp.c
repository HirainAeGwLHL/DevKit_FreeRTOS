#include "derivative.h"

#define MC_ME_MCTRL_KEY				0x5AF0U
#define MC_ME_MCTRL_INVKEY		0xA50FU

extern void ClockSource_Init(void);

typedef struct {
  uint8_t   port;
  uint8_t   pin;
  uint8_t   active;
} Led_PortConfigTypeDef;

#define LED_PORT(port)    (port - 'A')

#if DEVKIT_MPC5748G
#define LED_NUM_MAX     8

const Led_PortConfigTypeDef LedPortConfig_PC[LED_NUM_MAX] = {
    { .port = LED_PORT('J'), .pin = 4 , .active = 0 },
    { .port = LED_PORT('A'), .pin = 0 , .active = 0 },
    { .port = LED_PORT('A'), .pin = 10, .active = 0 },
    { .port = LED_PORT('A'), .pin = 4 , .active = 0 },
    { .port = LED_PORT('A'), .pin = 7 , .active = 0 },
    { .port = LED_PORT('C'), .pin = 4 , .active = 0 },
    { .port = LED_PORT('H'), .pin = 13, .active = 0 },
    { .port = LED_PORT('H'), .pin = 5 , .active = 0 },
};
#else
#define LED_NUM_MAX     4

const Led_PortConfigTypeDef LedPortConfig_PC[LED_NUM_MAX] = {
    { .port = LED_PORT('G'), .pin = 2 , .active = 0 },
    { .port = LED_PORT('G'), .pin = 3 , .active = 0 },
    { .port = LED_PORT('G'), .pin = 4 , .active = 0 },
    { .port = LED_PORT('G'), .pin = 5 , .active = 0 }
};
#endif

void Led_Init(void)
{
  int led;
  uint16_t port, pin;

  for(led = 0; led < LED_NUM_MAX; led++)
  {
    port = LedPortConfig_PC[led].port;
    pin  = LedPortConfig_PC[led].pin;

    SIUL2.MSCR[port*16 + pin].B.OBE = 0x01U;
    SIUL2.GPDO[port*16 + pin].R 		= !LedPortConfig_PC[led].active;
  }
}

void Led_On(int led)
{
  if (led < LED_NUM_MAX)
  {
    uint16_t port = LedPortConfig_PC[led].port;
    uint16_t pin  = LedPortConfig_PC[led].pin;

    SIUL2.GPDO[port*16 + pin].R = LedPortConfig_PC[led].active;
  }
}

void Led_Off(int led)
{
  if (led < LED_NUM_MAX)
  {
    uint16_t port = LedPortConfig_PC[led].port;
    uint16_t pin  = LedPortConfig_PC[led].pin;

    SIUL2.GPDO[port*16 + pin].R = !LedPortConfig_PC[led].active;
  }
}

void Led_Toggle(int led)
{
  if (led < LED_NUM_MAX)
  {
    uint16_t port = LedPortConfig_PC[led].port;
    uint16_t pin  = LedPortConfig_PC[led].pin;

    SIUL2.GPDO[port*16 + pin].R ^= 0x01U;
  }
}

void BSP_Init(void)
{
	int timeout = 5000;

	ClockSource_Init();

	Led_Init();

	Led_On(0);		// light LED1 to indicate Initialization.

	MC_ME.ME.R 				= 0x0000A11DU;	// enable SAFE, DRUN, RUN0, STANDBY0 mode only.
	MC_ME.DRUN_MC.R		= 0x00130010U;	// FIRC select as system clock source.
	MC_ME.RUN_MC[0].R = 0x00130072U;	// PLL_PHI_0 select as system clock source.

	MC_ME.RUN_PC[0].R = 0x00000000U;	// Run Peripheral Configuration 0 : disable in all mode.
	MC_ME.RUN_PC[1].R = 0x00000008U;	// Run Peripheral Configuration 1 : enable in DRUN mode only.
	MC_ME.RUN_PC[2].R = 0x00000010U;	// Run Peripheral Configuration 2 : enable in RUN0 mode only.
	MC_ME.RUN_PC[3].R = 0x00000018U;	// Run Peripheral Configuration 3 : enable in both DRUN and RUN0 mode.

	MC_ME.LP_PC[0].R  = 0x00000000U;	// Low power Peripheral Configuration 0 : disable in all low power mode.
	MC_ME.LP_PC[1].R  = 0x00002000U;	// Low power Peripheral Configuration 1 : enable in standby0 low power mode.

	MC_ME.PCTL[91].R	= 0x43U;				// PIT_RTI_0 module setting.

	MC_ME.IS.R			 |= 0x00000001U;	// clear mode transition complete interrupt flag.

	MC_ME.MCTL.R			= (0x04U << 28) | MC_ME_MCTRL_KEY;
	MC_ME.MCTL.R			= (0x04U << 28) | MC_ME_MCTRL_INVKEY;

	while ((MC_ME.IS.B.I_MTC == 0) && (--timeout > 0))
		;		// wait transition finish.

	if (timeout != 0)
	{
		MC_ME.IS.B.I_MTC = 0;						// clear flag.
	}
	else
	{
		while(1);		// dead here.
	}

	Led_Off(0);		// dark LED1 to indicate Initialization finished.
}




