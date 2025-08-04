#include "stm32f401rbt6.h"

extern unsigned int TIMER;
void KM_SYSTICK_INTERRUPT(void);
void KM_SYSTICK_POLLING(void);
void KM_mdelay(int n);
void KM_udelay(int n);
void KM_mdelay_IT(int n);
void KM_Delay_ms(int n);
void KM_udelay_IT(int n);
void KM_1sec_delay(void);

