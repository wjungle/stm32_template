#include "delay.h" 

/* Private variables */
__IO uint32_t TimingDelay;

/* Private function prototypes */
void Delay(__IO uint32_t nTime);

/*
 * SystemCoreClock / 1000	1ms/interrupt
 * SystemCoreClock / 100000	10us/interrupt
 * SystemCoreClock / 1000000	1us/interrupt
 * T = ticks x (1/f)
 *   = (168000000 / 1000) x ( 1 / 168000000 )
 *   = 168000 x ( 1 / 168000000 )
 *   = 1 / 1000
 *   = 10^(-3)
 *   = 1ms
 */
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock / 1000))
	{	
		/* Capture error */
		while(1);
	}
	/* Disable SysTick */
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;

	/* Enable SysTick */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

