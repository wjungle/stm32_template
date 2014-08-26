/* Includes -------------------------------------------------------------------*/
#include "main.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"

#include "stm32f4xx_conf.h"

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

int main(void)
{
	__IO uint32_t i = 0;
	uint8_t buf[255];
	uint8_t len;

	RCC_ClocksTypeDef RCC_Clocks;
  
	/* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
  
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
  
	/* SysTick end of count event each 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

	
	USBD_Init(&USB_OTG_dev,
		  USB_OTG_FS_CORE_ID,
		  &USR_desc,
		  &USBD_CDC_cb,
		  &USR_cb);
/*
	VCP_send_str("USB serial DEMO\ndas-labor.org\n");
	buf[0] = 't';
	buf[1] = 'e';
	buf[2] = 's';
	buf[3] = 't';
	buf[4] = 0;

	VCP_send_str(&buf[0]);

	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = 0;
*/
	while (1) {
		len = VCP_get_string(&buf[0]);
		if (len)
			VCP_send_str(&buf[0]);
		if (i == 0x1000000) {
			VCP_put_char('b');
			STM_EVAL_LEDOff(LED4);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOff(LED6); 
		}
		if (i++ == 0x2000000) {
			i = 0;
			VCP_put_char('a');
			STM_EVAL_LEDOn(LED4);
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOn(LED5);
			STM_EVAL_LEDOn(LED6); 
		}
	}

#if 0
	int i;
	float a = 1.2;
	float b = 1.8;
	SysTick_Init();
	USART3_Config();
	//USART3_printf(USART3, "\r\n This is a USART3_printf demo \r\n");
	//USART3_printf(USART3, "\r\n ("__DATE__" - "__TIME__") \r\n");

	int c = a * 10;
	{
		USART3_printf(">>>>>\n");
		for(i=0; i<30000000; i++);
		printf("\r\n this is a printf demo \r\n");
		printf("\r\n ("__DATE__" - "__TIME__") \r\n");
		printf("10 = %d\r\n", 10);
		printf("10 = %d\r\n", -10);
		Delay(1000);	//unit is ms
		if(b > a) printf("b > a\n");
		printf("c = %d\n", c);
		printf("a = %d\n", (int)(a*10));
//		printf("b = %f\n", b);
//		printf("a + b = %f\n", a+b);
//		printf("a + b = %d\n", a+b);
	}
#endif
}


/**
 * @brief  This function handles the test program fail.
 * @param  None
 * @retval None
 */
void Fail_Handler(void)
{
	/* Erase last sector */ 
	FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
	/* Write FAIL code at last word in the flash memory */
	FLASH_ProgramWord(TESTRESULT_ADDRESS, ALLTEST_FAIL);

	while(1)
	{
		/* Toggle Red LED */
		STM_EVAL_LEDToggle(LED5);
		Delay(5);
	}
}


#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1) {
	}
}
#endif

