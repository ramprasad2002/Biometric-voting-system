#include <stdlib.h>
#include "../inc/stm32f401rbt6.h"
#include "../inc/gpio.h"
#include "../inc/lcd.h"
#include "../inc/systemticktimer.h"
#include "../inc/keypad.h"
#include "../inc/i2c.h"
#define page2 16
#define page9 16*9
#define page10 16*10
#define page11 16*11
unsigned int interrupt,id_flag = 0;
unsigned char arr[5],id[5];
unsigned char user_count[4] = {0};
int result_flag = 0,result_flag1 = 0;
unsigned int user_counter =0;
extern unsigned char user_buffer[100];
extern unsigned char vote_buffer[100];
unsigned char erase_eeprom[500] = {0};
unsigned char result[4],result_str[3],result_str1[3],result_str2[3];

void CONFIG_INIT()
{
	GPIO_PORT_PUPDR(C,PU,9);			//Configure PORTC 9th pin as PULL UP resistor
	GPIO_PORT_PUPDR(C,PU,8);			//Configure PORTC 8th pin as PULL UP resistor
	SYSCFG_EXTICR3 |= 0x00000022;	//Configure SYSCFG_EXTICR3 enable EXTI8 and EXTI9 for PORTC
	EXTI_IMR |= 0x01<<8;						//Enable EXTI_IMR for 8th pin
	EXTI_FTSR |= 0x01<<8;						//Enable EXTI_FTSR for 8th pin
	EXTI_PR |= 0x1<<8;						//Enable EXTI_PR for 8th pin
	EXTI_IMR |= 0x01<<9;						//Enable EXTI_IMR for 9th pin
	EXTI_FTSR |= 0x01<<9;						//Enable EXTI_FTSR for 9th pin
	EXTI_PR |= 0x1<<9;						//Enable EXTI_PR for 9th pin
	NVIC_ISER_CONF(23);
	GPIO_PORT_PUPDR(C,PU,10);
  SYSCFG_EXTICR3 |= 0x00000200;	//Configure SYSCFG_EXTICR3 enable it for PORTC
	EXTI_IMR |= 0x01<<10;						//Enable EXTI_IMR for 10th pin
	EXTI_FTSR |= 0x01<<10;						//Enable EXTI_FTSR for 10th pin
	EXTI_PR |= 0x1<<10;	
	//Enable EXTI_PR for 10th pin
	NVIC_ISER1 |= 0x00000100;			//Enable Interrupt for 40th Interrupt line
}

void remove_0(unsigned char arr[],int n)
{
    int i,j,k;
	char temp;
  for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if (j==n-1 && arr[j]==0)
				continue;
			else if(arr[j]=='a')
			{
			  temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

int strreplace(unsigned char str1[],unsigned char sub[],char rep[])
{
	int ar = 0,flags = 0;
	int i=0,j,k,p,q,s;
	while(rep[ar])
	ar++;
	while(sub[i])
		i++;
	if(ar != i)
	   return 1;
	for(j=0;str1[j];j++)
	{
		if(str1[j]==sub[0])
		{
			for(k=0,p=j;k<i;k++,p++)
			{
				if (str1[p] != sub[k])
					break;
			}
			if(k==i)
			{
			   for(s=j,q=0;q<i;s++,q++)
			   {
			   	str1[s]=rep[q];
			   }
			   flags = 1;
			}
		}
	}
	if (flags == 1)
	    return 0;
	else 
		return 1;
}

void clear_array(unsigned char array[],int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		array[i] = 0;
	}
}



void Delete_func(void)
{
	char key,delete_buffer[5] = {'a','a','a','a','\0'};
	unsigned char arr[5];
	volatile int ret;
	int single_flag = 0,all_flag = 0,count = 0,flag1 = 0,flag2 = 0;
	arr[4] = 0;
	LCD_WRITE_CMD(0X01);
	LCD_WRITE_CMD(0X80);
	LCD_WRITE_STRING("1.DELETE A USER");
	LCD_WRITE_CMD(0XC0);
	LCD_WRITE_STRING("2.DELETE ALL");
	while(1)
	{
		key = Key_scan();
		KM_mdelay(150);
		if (key == '1')
		{
			single_flag = 1;
			break;
		}
		if (key == '2')
		{
			all_flag = 1;
			break;
		}
	}
	if (single_flag == 1)
	{
		LCD_WRITE_CMD(0X01);
		LCD_WRITE_CMD(0X80);
		LCD_WRITE_STRING("ENTER 4 DIGIT ID");
		while(count != 4)
		 {
			 key = Key_scan();
			 KM_mdelay(150);
			 if (key != 0 && key != '*' && key != '#')
			 {
				 LCD_WRITE_CMD(0XC0+count);
				 LCD_WRITE_DATA(key);
				 arr[count] = key;
				 count++;
			 }
			 if (key == '*')
			 {
				 count--;
				 LCD_WRITE_CMD(0XC0+count);
				 LCD_WRITE_DATA(' ');
				 //collecting id to a character array
				 arr[count] = 0;
			 }
		  }
		 key = 0;
		 count = 0;
		 LCD_WRITE_CMD(0X80);
		 LCD_WRITE_STRING("PRESS # TO ENTER");
		 //key scan function to press enter button
		 while(1)
		 {
			 key = Key_scan();
			 KM_mdelay(150);
				if (key == '#')
				{
					flag1 = 1;
					break;
				}
		 }
		 if (flag1 == 1)
		 {
			 LCD_WRITE_CMD(0X80);
		   LCD_WRITE_STRING("PRESS * CONFIRM ");
			 LCD_WRITE_CMD(0XC0);
		   LCD_WRITE_STRING("PRESS # STOP");
			 while(1)
			 {
				 key = Key_scan();
			   KM_mdelay(150);
				 if (key == '*')
				 {
					 flag1 = 1;
					 break;
				 }
				 if (key == '#')
				 {
					 flag1 = 0;
					 break;
				 }
		   }
			 if (flag1 == 0)
			 {
				 return;
			 }
			 if (flag1 == 1)
			 {
				 ret = strreplace(user_buffer,arr,delete_buffer);
					if (ret == 0)
					{
						LCD_WRITE_CMD(0X01);
						LCD_WRITE_CMD(0X80);
						LCD_WRITE_STRING("USER DELETED");
						remove_0(user_buffer,100);
						KM_I2C_Mem_Write(0x50,page2,0x01,user_buffer,100);
						//usercount decrementing
						KM_I2C_Mem_Read(0x50,0X06,0x01,user_count,4);
						user_counter = my_atoi(user_count);
						user_counter--;
						myitoa(user_counter,user_count);
						KM_I2C_Mem_Write(0x50,0x06,0x01,user_count,4);
						clear_array(arr,5);
						KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
					}
					if (ret == 1)
					{
						LCD_WRITE_CMD(0X01);
						LCD_WRITE_CMD(0X80);
						LCD_WRITE_STRING("USER NOT DELETED");
						KM_mdelay(300);
						clear_array(arr,5);
						KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
					}
					flag1 = 0;
			 }
		 }
		 single_flag = 0;
	}
	if (all_flag == 1)
	{
		 LCD_WRITE_CMD(0X80);
		 LCD_WRITE_STRING("PRESS * CONFIRM");
		 LCD_WRITE_CMD(0XC0);
		 LCD_WRITE_STRING("PRESS # STOP");
		while(1)
		{
			key = Key_scan();
			KM_mdelay(150);
			if (key == '*')
		  {
				flag2 = 1;
				break;
			}
			if (key == '#')
			{
				flag2 = 0;
				break;
			}
		}
		if (flag2 == 0)
		{
			return;
		}
		if (flag2 == 1)
		{
			LCD_WRITE_CMD(0X01);
			LCD_WRITE_STRING("DELETING DATA     WAIT.....");
			KM_I2C_Mem_Write(0x50,0x06,0x01,erase_eeprom,100);
			user_counter = 0;
			user_count[0] = user_counter;
			KM_I2C_Mem_Write(0x50,0x00,0x01,user_count,1);
			all_flag = 0;
			flag2 = 0;
			KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
		}
		all_flag = 0;
	}
	return;
}

void RESULT()
{
	if (result_flag == 1)
	{
		if (result_flag1 == 1)
		{
			KM_I2C_Mem_Read(0x50,page10,0x01,result,3);
			LCD_WRITE_CMD(0X01);
			LCD_WRITE_CMD(0X80);
			LCD_WRITE_STRING("CAN1:");
			myitoa(result[0],result_str);
			LCD_WRITE_STRING(result_str);
			LCD_WRITE_STRING("  ");
			LCD_WRITE_STRING("CAN3:");
			myitoa(result[2],result_str2);
			LCD_WRITE_STRING(result_str2);
			LCD_WRITE_CMD(0XC0);
			LCD_WRITE_STRING("CAN2:");
			myitoa(result[1],result_str1);
			LCD_WRITE_STRING(result_str1);
			KM_mdelay(20000);
			if ((result[0] > result[1]) && (result[0] > result[2]))
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("CONGRATULATIONS");
				LCD_WRITE_CMD(0XC0);
				LCD_WRITE_STRING("CANDIDATE 1 WINS");
			}
			else if (result[1] > result[2])
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("CONGRATULATIONS");
				LCD_WRITE_CMD(0XC0);
				LCD_WRITE_STRING("CANDIDATE 2 WINS");
			}
			else if (((result[0] == result[1]) && (result[1] == result[0])) || ((result[0] < result[1]) && (result[1] == result[2]))	|| ((result[1] < result[0]) && (result[0] == result[2])) || ((result[2] < result[1]) && (result[0] == result[1])))
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("ENDS UP IN TIE");
			}
			else
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("CONGRATULATIONS");
				LCD_WRITE_CMD(0XC0);
				LCD_WRITE_STRING("CANDIDATE 3 WINS");
			}
			KM_mdelay(20000);
			result_flag1 = 0;
		}
		result_flag = 0;
	}
}

void USER_COUNT()
{
	int ret;
	char * string;
	if(result_flag == 1)
	{
		LCD_WRITE_CMD(0X80);
		KM_I2C_Mem_Read(0x50,0X06,0x01,user_count,4);
		LCD_WRITE_CMD(0X01);
		if (user_count[0] == 0)
		{
			LCD_WRITE_STRING("00");
		}
		else
		{
			LCD_WRITE_STRING(user_count);
		}
		KM_mdelay(2000);
		LCD_WRITE_CMD(0X01);
		result_flag = 0;
	}
}

void CAND_COUNT()
{
	char * string;
	if (result_flag1 == 1)
	{
		LCD_WRITE_CMD(0X01);
		LCD_WRITE_STRING("CANDIDATES:03");
		KM_mdelay(2000);
		LCD_WRITE_CMD(0X01);
		result_flag1 = 0;
	}
}

void CONFIG()
{
	int lock=3,count = 0,key,flag = 0,ret,config = 0,flag1=0,flag2=0,flag3=0,hash_flag = 0,return_flag = 0,flag4 = 0,flag5 = 0;
	unsigned char * ptr, *ptr1;
	if (interrupt == 1)
	{
		//authentication
		lock = 3;
		while(lock)
		{
			LCD_WRITE_CMD(0X01);
			LCD_WRITE_CMD(0X80);
			//enter the password
			LCD_WRITE_STRING("ENTER PASSWORD");
			//key scan function to enter the key
			while(count != 4)
			{
				 key = Key_scan();
					KM_mdelay(150);
				if (key != 0 && key != '*' && key != '#')
				{
					LCD_WRITE_CMD(0XC0+count);
					LCD_WRITE_DATA(key);
					arr[count] = key;
					count++;
				}
				if (key == '*')
				{
					count--;
					LCD_WRITE_CMD(0XC0+count);
					LCD_WRITE_DATA(' ');
					//collecting id to a character array
					arr[count] = 0;
				}
			}
			key = 0;
			count =0;
			LCD_WRITE_CMD(0X80);
			LCD_WRITE_STRING("PRESS # TO ENTER");
			//key scan function to press the enter button
			while(1)
			{
				key = Key_scan();
					KM_mdelay(150);
				if (key == '#')
				{
					flag = 1;
					break;
				}
			}
			//checking the flag based on the key press
		  if (flag == 1)
		  {
				//allocating memory for password authentication dynamically
				ptr1 = (unsigned char *)malloc(4);
				//reading the password from eeprom
				KM_I2C_Mem_Read(0X50,0X01,0X01,ptr1,4);
				//checking the password is correct or not
				ret = strcompare(arr,ptr1);
				if (ret == 0)
				{
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_STRING("ACCESS GRANTED");
					KM_mdelay(300);
					config = 1;
					//clear the password array
					clear_array(arr,5);
					break;
				}
				else
				{
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_STRING("ACCESS DENIED");
					KM_mdelay(300);
					config = 0;
					//clear the password array
					clear_array(arr,5);
				}
				//deallocating the memory
				free(ptr1);
				ptr1 = 0;
		  }
			//decrement the password entering times if enter the wrong password
			lock--;
		}
		//three times authentication over with wrong password
		if (config != 1)
		{
			interrupt = 0;
			EXTI_IMR |= 0X01<<10;
			LCD_WRITE_CMD(0X80);
			LCD_WRITE_STRING("ENTER 4 DIGIT ID");
			return;
		}
		if (config == 1)
		{
		//operation
			while(1)
			{
				LCD_WRITE_CMD(0X01);
				LCD_WRITE_CMD(0X80);
				LCD_WRITE_STRING("1.ENR");
				LCD_WRITE_CMD(0X86);
				LCD_WRITE_STRING("2.DEL");
				LCD_WRITE_CMD(0X8C);
				LCD_WRITE_STRING("3.ERA");
				LCD_WRITE_CMD(0XC8);
				LCD_WRITE_STRING(" 5.EXIT");
				LCD_WRITE_CMD(0XC0);
				LCD_WRITE_STRING("4.RES");
				while(1)
				{
					key = Key_scan();
						KM_mdelay(150);
					if (key == '1')
					{
						flag1 = 1;
						break;
					}
					if (key == '2')
					{
						flag2 = 1;
						break;
					}
					if (key == '4')
					{
						flag3 = 1;
						break;
					}
					if (key == '3')
					{
						flag4 = 1;
						break;
					}
					if (key == '5')
					{
						flag5 = 1;
						break;
					}
				}
				//checking the eeprom erase key
				if (flag4 == 1)
				{
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_STRING("ERASING DATA     WAIT.....");
					KM_I2C_Mem_Write(0x50,page9,0x01,erase_eeprom,90);
					KM_I2C_Mem_Read(0x50,page11,0x01,vote_buffer,100);
					flag4 = 0;
				}
				//Enrollment mode
				if (flag1 == 1)
				{
					KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
					LCD_WRITE_CMD(0X01);
					LCD_WRITE_CMD(0X80);
					LCD_WRITE_STRING("ENTER USER ID:");
					//key san function for user id entering
					while(count != 4)
					{
						key = Key_scan();
							KM_mdelay(150);
						if (key != 0 && key != '*' && key != '#')
						{
							LCD_WRITE_CMD(0XC0+count);
							LCD_WRITE_DATA(key);
							id[count] = key;
							count++;
						}
						if (key == '*')
						{
							count--;
							LCD_WRITE_CMD(0XC0+count);
							LCD_WRITE_DATA(' ');
							//collecting id to a character array
							arr[count] = 0;
						}
					}
					key = 0;
					count = 0;
					LCD_WRITE_CMD(0X80);
					LCD_WRITE_STRING("PRESS # TO ENTER");
					//key scan function to press enter button
					while(1)
					{
						key = Key_scan();
						KM_mdelay(150);
						if (key == '#')
						{
							hash_flag = 1;
							break;
						}
					}
					if (hash_flag == 1)
					{
						ret = strsubstr(user_buffer,id);
						if (ret == 1)
						{
							LCD_WRITE_CMD(0X01);
							LCD_WRITE_CMD(0X80);
							LCD_WRITE_STRING("USER ADDED");
							//user_counter = 0;
							KM_I2C_Mem_Read(0x50,0x06,0x01,user_count,4);
							user_counter = my_atoi(user_count);
							//id is writing to eeprom
							KM_I2C_Mem_Write(0x50,page2+(user_counter*4),0x01,id,4);
							//increment the count
								user_counter++;
							myitoat(user_counter,user_count);
							KM_I2C_Mem_Write(0x50,0x06,0x01,user_count,4);
							//clear the id array
							KM_I2C_Mem_Read(0x50,0x00,0x01,id,3);
							clear_array(id,5);
							KM_I2C_Mem_Read(0x50,page2,0x01,user_buffer,100);
						}
						else if (ret == 0)
						{
							LCD_WRITE_CMD(0X01);
							LCD_WRITE_CMD(0X80);
							LCD_WRITE_STRING("USER ALREADY");
							LCD_WRITE_CMD(0XC0);
							LCD_WRITE_STRING("EXISTS");
							KM_mdelay(1000);
							//clear the id array
							clear_array(id,5);
						}
					}
					flag1 = 0;
					ret = 0;
				}
				
				//Delete mode
				if (flag2 == 1)
				{
					Delete_func();
					flag2 = 0;
				}
				//Result mode
				if (flag3 == 1)
				{
					RESULT();
					flag3 = 0;
				}
				//EXIT mode
				if (flag5 == 1)
				{
					
					flag5 = 0;
					return_flag = 1;
					break;
				}
				config = 0;
			}
		}
		if (return_flag == 1)
		{
			return_flag = 0;
			LCD_WRITE_CMD(0X01);
			EXTI_IMR |= 0X01<<10;
	    interrupt = 0;
			return;
		}
	 EXTI_IMR |= 0X01<<10;
	 interrupt = 0;
	}
	
}


void EXTI9_5_IRQHandler()
{
	if (EXTI_PR & (0X01<<9))
	{
		result_flag = 1;
		EXTI_PR |= 0x01<<9;			//Clearing the EXTI_PR register
	}
	if(EXTI_PR & (0X01<<8))
	{
		result_flag1 = 1;
		EXTI_PR |= 0x01<<8;			//Clearing the EXTI_PR register
	}
}






void EXTI15_10_IRQHandler()
{
	if (EXTI_PR & (0x01<<10))		//checking the EXTI_PR register 
	{
		//KM_mdelay(300);
		interrupt = 1;							
		EXTI_IMR &= ~(0X01<<10);
		EXTI_PR |= 0x01<<10;			//Clearing the EXTI_PR register
	}
}
