#include "../inc/stm32f401rbt6.h"
#include "../inc/lcd.h"
#include "../inc/gpio.h"
#include "../inc/systemticktimer.h"

char keypad[4][4] = {{'1','2','3','A'},
					 {'4','5','6','B'},
					 {'7','8','9','C'},
					 {'*','0','#','D'}};


void matrix_keypad_init()
{
	GPIO_PORT_PUPDR(A,PU,0);
	GPIO_PORT_PUPDR(A,PU,1);
	GPIO_PORT_PUPDR(A,PU,4);
	GPIO_PORT_PUPDR(A,PU,5);
	GPIO_PORT_MODER(C,OP,0);
	GPIO_PORT_MODER(C,OP,1);
	GPIO_PORT_MODER(C,OP,2);
	GPIO_PORT_MODER(C,OP,3);
	GPIOC_ODR |= 0XF;
}

char Key_scan()
{
	int i,j,row = 4,col = 4,arr[5]={0,1,4,5};
	for(i=0;i<col;i++)
	{
		GPIOC_ODR &= ~(0X01<<i);
		for(j=0;j<row;j++)
		{
			if ((GPIOA_IDR & (0X01<<(arr[j]))) == 0)
			{
				GPIOC_ODR |= (0X01<<i);
				return keypad[j][i];
			} 
		}
		GPIOC_ODR |= (0x01<<i);
	}
	return 0;
}



int atoii(unsigned char str[])
{
	int k,i,j = 0;
		int l = j;
	int arr[3],sum=0,pro=1;
	while(str[j])
		j++;
	for(i=0;str[i];i++)
	{
		arr[i] = str[i] - 48;
	}
	i = 0;
	for(k=0;k<j;k++)
	{
		i=l-1,pro = 1;
		while(i)
		{
			pro = pro * 10;
			i--;
		}
		sum = sum + arr[k]*pro;
		l--;		
	}
	return sum;
}


int my_atoi(unsigned char *str) {
    int result = 0;    // Store the final integer result
    int sign = 1;      // Default sign is positive
    

    // Check for optional sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Convert characters to integer until a non-digit character is encountered
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    // Apply the sign and return the result
    return result * sign;
}

int strcompare(unsigned char str1[],unsigned char str2[])
{
	int i=0,dif;
	for(i=0;str1[i] && str2[i];i++)
	{
		if(str1[i] != str2[i])
		{
			dif = str1[i] - str2[i];
			return dif;
		}
    }	
	return 0;
}

int strsubstr(unsigned char str1[],unsigned char sub[])
{
	int i=0,j,k,p;
	while(sub[i])
		i++;
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
			return 0;
		}
	}
	return 1;
}
