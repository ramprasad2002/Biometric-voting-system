#include "../inc/stm32f401rbt6.h"

int COUNTER;
unsigned int TIMER;

/**
 * @brief Initializes the SYSTICK timer in polling mode.
 * @details This function configures the SYSTICK timer to generate a periodic interrupt.
 *          The timer is loaded with a value of 0X0123, and the COUNTFLAG is enabled.
 *          The function does not return anything.
 */
void KM_SYSTICK_POLLING(void)			//initializing the systicktimer in polling mode
{
	STK_CTRL |= 0X05;
	STK_VAL |= 0X0123;
}

/**
 * @brief Initializes the SYSTICK timer in interrupt mode.
 * @details This function configures the SYSTICK timer to generate a periodic interrupt.
 *          The timer is loaded with a value of 0X0123, and the COUNTFLAG and TICKINT are enabled.
 *          The function does not return anything.
 */
void KM_SYSTICK_INTERRUPT(void)			//initializing the systicktimer in interrupt mode
{
	STK_CTRL |= 0X07;
	STK_VAL |= 0X0123;
}


/**
 * @brief Delays execution for a specified number of milliseconds.
 * @details This function uses the SysTick timer to create a blocking delay.
 *          It loads the timer with a value corresponding to 1 millisecond and 
 *          increments a counter until the desired delay time is reached.
 * @param n The number of milliseconds to delay.
 */
//void  KM_mdelay(int n)
//{ 
//	int count = 0;
//	STK_LOAD = 16000 - 1;		//load delay value of 1 milli seconds
//	while(count != n)
//	{
//	  while(!(STK_CTRL & (0X01<<16)));
//			count++;
//			TIMER++;
//	}
//}	

void KM_1sec_delay()
{
	int count = 1;
	STK_LOAD = 16000;		//load delay value of 1 milli seconds
//	while(count != 1000){
	while(!(STK_CTRL & (0X01<<16))){;}
//		count++;}
}	
	
void  KM_mdelay(int n)
{ 
		int  j=0;
		STK_LOAD = 16000;
		j = COUNTER + n;
	  while(j > COUNTER)
		{
			;
		}	
}	


/**
 * @brief Delays execution for a specified number of microseconds.
 * @details This function uses the SysTick timer to create a blocking delay.
 *          It loads the timer with a value corresponding to 1 microsecond and 
 *          increments a counter until the desired delay time is reached.
 * @param n The number of microseconds to delay.
 */
void  KM_udelay(int n)
{ 
	int count = 0;
	STK_LOAD = 16;	//load delay of 1 micro seconds
	while(count != n)
	{
	  while(!(STK_CTRL & (0X01<<16)));	
		count++;
	}
}	


/**
 * @brief Delays execution for a specified number of milliseconds using interrupts.
 * @details This function initializes the SysTick timer to create a non-blocking delay.
 *          It sets a global counter to zero and loads the timer with a value corresponding 
 *          to one millisecond. The function waits until the counter reaches the desired 
 *          delay duration, which is decremented by one to account for initial setup.
 * @param n The number of milliseconds to delay.
 */
void KM_mdelay_IT(int n)
{
		int  j=0;
		STK_LOAD = 16;
		j = COUNTER + n;
	  while(j > COUNTER)
		{
			;
		}
		
}


void KM_Delay_ms(int n)
{
		int  j=0;
		STK_LOAD = 16000;
		j = COUNTER + n;
	  while(j > COUNTER)
		{
			;
		}
}

//void KM_Delay_ms(int n)
//{
//		int count = 0;
//	STK_LOAD = 16000 - 1;		//load delay value of 1 milli seconds
//	while(count != n)
//	{
//	  while(!(STK_CTRL & (0X01<<16)));
//			count++;
//	}
//}


/**
 * @brief Delays execution for a specified number of microseconds using interrupts.
 * @details This function initializes the SysTick timer to create a non-blocking delay.
 *          It sets a global counter to zero and loads the timer with a value corresponding 
 *          to one microsecond. The function waits until the counter reaches the desired 
 *          delay duration, which is decremented by one to account for initial setup.
 * @param n The number of microseconds to delay.
 */
void KM_udelay_IT(int n)
{
  COUNTER = 0;
	STK_LOAD = 16;				//load delay of 1 micro seconds
	while(COUNTER != n-1);		
}


/**
 * @brief Handles the SysTick interrupt.
 * @details This function is called when the SysTick timer reaches zero.
 *          It increments the global counter COUNTER.
 */
void SysTick_Handler(void)		//this interrupt occur when the STK_VAL reaches to 0
{
	COUNTER++;
	TIMER++;
}
