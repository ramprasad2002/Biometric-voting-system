/*
Project: SMART VOTING SYSTEM
Date : 31-12-24
Microcontroller : STM32F401RBT6
Core peripherals : 1.NVIC
									 2.SYSTICK TIMER
Controller Peripherals: 1.I2C
												2.GPIO
Peripherals: 1.EEPROM
						 2.16X2 MONOCHROME LCD
						 3.4X4 MEMBRANE KEYPAD
						 
Team Members:- 
1.P.Siddesh Kumar       KM56BESD28
2.Y.PAVAN KUMAR RAJ     KM56BESD01
3.A.RAMA DURGA PRASAD		KM56BESD41
4.P.VAMSHI            	KM56BESD48
5.K.HARI PRIYA          KM56BESD21
6.K.ROHINI REDDY        KM56BNF04
*/



#include <stdlib.h>
#include "../inc/stm32f401rbt6.h"
#include "../inc/gpio.h"
#include "../inc/systemticktimer.h"
#include "../inc/lcd.h"
#include "../inc/usart.h"
#include "../inc/keypad.h"
#include "../inc/i2c.h"
#include "cofig.h"


#define page2 16										//Defining EEPROM page2
#define page10 16*10								//Defining EEPROM page10
#define page11 16*11								//Defining EEPROM page11
extern int interrupt;								//externing the interrupt flag from configuration file
extern unsigned char * user_count;	//externing the user_count buffer form configuration file
extern unsigned int user_counter;		//externing the user_counter count from configuration
int voter_count = 0;								//Declaring a voter count variable
unsigned char buffer[50] = {0};			//declaring a buffer array with 50 size
unsigned char user_buffer[100];			//declare a user_buffer to store user data
unsigned char vote_buffer[100];			//declare a vote_buffer to store vvoting details
unsigned char candidate[5] = {0,0,0,0,0};			//initialize a buffer to store vote count

int main()
{
	//unsigned char buffer[5] = {'2','0','2','5'};
	int i,count = 0,flag = 0,flag1 = 0,ret,config_flag = 0,ret1 = 0; 	//declaring and inituializing variables
	unsigned char *ptr;		
	char key;
	unsigned char arr[5];
	GPIO_PORT_INIT(A);		//enable clock for portA
	GPIO_PORT_INIT(B);		//Enable clock for portB
	GPIO_PORT_INIT(C);		//Enable clock for portC
	RCC_APB2ENR |= (0X01<<14);	//Enable clock for systemconfiguration
	GPIO_PORT_MODER(B,OP,12);
	GPIO_PORT_MODER(B,OP,13);
	KM_SYSTICK_INTERRUPT();		//initializing the systicktimer with interrupt
	KM_I2C1_Init();						//initializing the i2c1
	KM_I2C1_Config();					//configuring the i2c1 function
	CONFIG_INIT();						//configuration mode initialization
	KM_LCD_INIT();					//LCD initialization
	matrix_keypad_init();			//Keypad initialization	
  //KM_I2C_Mem_Write(0x50,0x01,0x01,buffer,5);
	KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);	//taking the user details from EEPROM to user_buffer   
	KM_I2C_Mem_Read(0x50,page11,0x01,vote_buffer,100);	//taking the voting details from EEPROM to vote_buffer
	clear_array(arr,5);			//clearing the array
	while(1)
	{
		LCD_WRITE_CMD(0X01);	//clearing the display
		while(1)
		{
			LCD_WRITE_CMD(0X82);	//command to start at 1st row 1st column
			LCD_WRITE_STRING("SMART VOTING");
			LCD_WRITE_CMD(0XC4);
      LCD_WRITE_STRING("SYSTEM");
			key = Key_scan();			//calling key scsn function
			KM_mdelay(150);				//delay of 150msec
			if (key == 'D')			//checking the key is 'D'
				break;
		 if (interrupt == 1)
		 {
			CONFIG();
			config_flag = 1;
		 }
			USER_COUNT();
			CAND_COUNT();
		}
		//Enter the 2 digit user id
	 	LCD_WRITE_CMD(0X01);			//clearing the display
		LCD_WRITE_CMD(0X80);			//command to start at 1st row 1st column
		LCD_WRITE_STRING("ENTER 4 DIGIT ID");		//prompt to user to enter the user id 
		while(count != 4)					//loop run until user enter the user id	
		{
			key = Key_scan();			//calling the key scan function 
			KM_mdelay(150);				//delay of 150msec
			if (key != 0 && key != '*' && key != '#' && count != 4)	//check the key is not equal to 0,*,#, and count != 4 
			{
				LCD_WRITE_CMD(0XC0+count);		////command to display the id
				LCD_WRITE_DATA(key);					//display the id
				//collecting id to a character array	
				arr[count] = key;					
				count++;						//increment the count
			 }
			 if (key == '*')			//checking the key is * to backspce the id
			 {
				count--;
				LCD_WRITE_CMD(0XC0+count);
				LCD_WRITE_DATA(' ');
				//collecting id to a character array
				arr[count] = 0;
			 }
			 if (interrupt == 1)
			 {
			  CONFIG();
			  config_flag = 1;
			 }
			 	USER_COUNT();
				CAND_COUNT();
		}
		if (config_flag == 1)
		{
			KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
			config_flag = 0;
		}
		key = 0;
		count = 0;
		LCD_WRITE_CMD(0X80);
		LCD_WRITE_STRING("PRESS # TO ENTER");
		while(1)
		{
			key = Key_scan();
			KM_mdelay(150);
			if (key == '#')
			{
				flag = 1;
				break;
			}
			CONFIG();
		}
		//checking the user exists or not
		if (flag == 1)
		{
			KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
			ret = strsubstr(user_buffer,arr);
			if (ret == 0)
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("USER FOUND");
				KM_mdelay(1000);
				ret = strsubstr(vote_buffer,arr);
				if (ret == 0)
				{
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_STRING("ALREADY VOTED");
					GPIOB_ODR &= ~(0X01<<12);
					GPIOB_ODR &= ~(0X01<<13);
					KM_mdelay(3000);
					GPIOB_ODR |= (0X01<<12);
					GPIOB_ODR |= (0X01<<13);
				}
				else
				{
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_CMD(0X80);
					LCD_WRITE_STRING("CAND1:A CAND2:B"); 
					LCD_WRITE_CMD(0XC0);	
					LCD_WRITE_STRING("CAND3:C"); 
					KM_mdelay(3000);
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_CMD(0X80);
					LCD_WRITE_STRING("PRESS A/B/C TO VOTE:");
					while(1)
					{
						key = Key_scan();
						KM_mdelay(150);
						if (key == 'A')
						{
							candidate[0]++;
							LCD_WRITE_DATA(key);
							KM_mdelay(500);
							LCD_WRITE_CMD(0X01);
							LCD_WRITE_CMD(0X80);
							LCD_WRITE_STRING("VOTED FOR");         
							LCD_WRITE_CMD(0XC5);
							LCD_WRITE_STRING("CANDIDATE 1");
							break;
						}
						if (key == 'B')
						{
							candidate[1]++;
							LCD_WRITE_DATA(key);
							KM_mdelay(500);
							LCD_WRITE_CMD(0X01);
							LCD_WRITE_CMD(0X80);
							LCD_WRITE_STRING("VOTED FOR");         
							LCD_WRITE_CMD(0XC5);
							LCD_WRITE_STRING("CANDIDATE 2");
							break;
						}
						if (key == 'C')
						{
							candidate[2]++;
							LCD_WRITE_DATA(key);
							KM_mdelay(500);
							LCD_WRITE_CMD(0X01);
							LCD_WRITE_CMD(0X80);
							LCD_WRITE_STRING("VOTED FOR");         
							LCD_WRITE_CMD(0XC5);
							LCD_WRITE_STRING("CANDIDATE 3");
							break;
						}
					}
					KM_I2C_Mem_Write(0x50,page10,0x01,candidate,3);
					//id is writing to eeprom
					KM_I2C_Mem_Write(0x50,page11+(voter_count*4),0x01,arr,4);
					//increment the count 
					candidate[3] = voter_count++;
					KM_I2C_Mem_Write(0x50,page10,0x01,candidate,4);
					//clear the id array
					clear_array(arr,5);
					KM_I2C_Mem_Read(0x50,page11,0x01,vote_buffer,100);
				}
				ret = 0;
			}
			if (ret == 1)
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("USER NOT FOUND");
				KM_mdelay(1000);
				clear_array(arr,5);
			}
		}
		CONFIG();
	}
}