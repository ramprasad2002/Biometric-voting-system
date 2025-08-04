#include "../inc/stm32f401rbt6.h"


void GPIO_PORT_INIT(char port)
{
	if (port==0)
	  RCC_AHB1ENR |= 0x01;		//Enable the PORTA clock
	if (port==1)
		RCC_AHB1ENR |= 0x02;		//Enable the PORTB clock
	if (port==2)
		RCC_AHB1ENR |= 0x04;		//Enable the PORTC clock	
}


void GPIO_PORT_MODER(char port,char mode,int pin_num)
{
	switch(port)
	{
		case 0:
		{
			if (mode == 1)
			{
				GPIOA_MODER &= ~(0x01 << ((pin_num*2)+1));
				GPIOA_MODER |= 0x01 << (pin_num *2);
			}
			else if (mode == 2)
			{
				GPIOA_MODER &= ~(0x01 << (pin_num*2));
				GPIOA_MODER |= 0x01 << ((pin_num *2)+1);
			}
			else if (mode == 3)
			{
				GPIOA_MODER |= 0X01<< pin_num *2;
				GPIOA_MODER |= 0X01<< ((pin_num *2)+1);
			}
		}
		break;
		case 1:
		{
			if (mode == 1)
			{
				GPIOB_MODER &= ~(0x01 << ((pin_num*2)+1));
				GPIOB_MODER |= 0x01 << (pin_num *2);
			}
			else if (mode == 2)
			{
				GPIOB_MODER &= ~(0x01 << (pin_num*2));
				GPIOB_MODER |= 0x01 << ((pin_num *2)+1);
			}
			else if (mode == 3)
			{
				GPIOB_MODER |= 0X01<< pin_num *2;
				GPIOB_MODER |= 0X01<< ((pin_num *2)+1);
			}
		}
		break;
		case 2:
		{
			if (mode == 1)
		   	{
				GPIOC_MODER &= ~(0x01 << ((pin_num*2)+1));
				GPIOC_MODER |= 0x01 << (pin_num *2);
			}
			else if (mode == 2)
			{
				GPIOC_MODER &= ~(0x01 << (pin_num*2));
				GPIOC_MODER |= 0x01 << ((pin_num *2)+1);
			}
			else if (mode == 3)
			{
				GPIOC_MODER |= 0X01<< pin_num *2;
				GPIOC_MODER |= 0X01<< ((pin_num *2)+1);
			}
		}
	}
}



void GPIO_PORT_PUPDR(char port,char mode,int pin_num)
{
	if (port==0)
	{
		if (mode == 1)
		{
			GPIOA_PUPDR &= ~(0x01 << ((pin_num*2)+1));
			GPIOA_PUPDR |= 0x01 << (pin_num *2);
		}
		else if (mode == 0)
		{
			GPIOA_PUPDR &= ~(0x1<<(pin_num*2));
			GPIOA_PUPDR |= 0x01 << ((pin_num *2)+1);
		}
	}
	if (port==1)
	{
		if (mode == 1)
		{
			GPIOB_PUPDR &= ~(0x01 << ((pin_num*2)+1));
			GPIOB_PUPDR |= 0x01 << (pin_num *2);
		}
				else if (mode == 0)
		{
			GPIOA_PUPDR &= ~(0x1<<(pin_num*2));
			GPIOA_PUPDR |= 0x01 << ((pin_num *2)+1);
		}
	}	
	if (port==2)
	{
		if (mode == 1)
		{
			GPIOC_PUPDR &= ~(0x01 << ((pin_num*2)+1));
			GPIOC_PUPDR |= 0x01 << (pin_num *2);
		}
				else if (mode == 0)
		{
			GPIOA_PUPDR &= ~(0x1<<(pin_num*2));
			GPIOA_PUPDR |= 0x01 << ((pin_num *2)+1);
		}
	}
}

void NVIC_ISER_CONF(int pin_num)
{
//	if (pin_num >= 0 && pin_num <= 31)
//	{
//		NVIC_ISER0 |= 0x01 << pin_num;
//	}
//	if (pin_num >= 32 && pin_num <= 50)
//	{
//		pin_num -= 32;
//		NVIC_ISER1 |= 0x01 << pin_num;
//	}
		int iser = pin_num/32;
		int pin = pin_num%32;
		switch(iser)
		{
			case 0:NVIC_ISER0 |= 0X01<<pin;break;
			case 1:NVIC_ISER1 |= 0X01<<pin;break;
			case 2:NVIC_ISER2 |= 0X01<<pin;break;
			case 3:NVIC_ISER3 |= 0X01<<pin;break;
			case 4:NVIC_ISER4 |= 0X01<<pin;break;
			case 5:NVIC_ISER5 |= 0X01<<pin;break;
			case 6:NVIC_ISER6 |= 0X01<<pin;break;
			case 7:NVIC_ISER7 |= 0X01<<pin;break;
		}
}



void SYSCFG_EXTICR_CONF(char port,int pin_num)
{
	switch(port)
	{
		case 0:
		{
			if (pin_num >= 0 && pin_num <=3)
			{
				SYSCFG_EXTICR1 |= 0X00 << pin_num*4;
			}
			if (pin_num >=4 && pin_num <=7)
			{
				SYSCFG_EXTICR1 |= 0x00 << (pin_num-4)*4;
			}
			if (pin_num >=8 && pin_num <=11)
			{
				SYSCFG_EXTICR1 |= 0X00 << (pin_num-8)*4;
			}
		}
		break;
		case 1:
		{
			if (pin_num >= 0 && pin_num <=3)
			{
				SYSCFG_EXTICR2 |= 0X01 << pin_num*4;
			}
			if (pin_num >=4 && pin_num <=7)
			{
				SYSCFG_EXTICR2 |= 0x01 << (pin_num-4)*4;
			}
			if (pin_num >=8 && pin_num <=11)
			{
				SYSCFG_EXTICR2 |= 0X01 << (pin_num-8)*4;
			}
		}
		break;
		case 2:
		{
			if (pin_num >= 0 && pin_num <=3)
			{
				SYSCFG_EXTICR3 |= 0X02 << pin_num*4;
			}
			if (pin_num >=4 && pin_num <=7)
			{
				SYSCFG_EXTICR3 |= 0x02 << (pin_num-4)*4;
			}
			if (pin_num >=8 && pin_num <=11)
			{
				SYSCFG_EXTICR3 |= 0X02 << (pin_num-8)*4;
			}
		}
	}
}
