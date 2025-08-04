#include "../inc/stm32f401rbt6.h"
#include "../inc/systemticktimer.h"


/**
 * @brief Sends the higher 4 bits of the data to the LCD
 * @details Loads the higher 4 bits of the data into GPIOB_ODR and sets the enable pin to write the data to the LCD.
 * @param data The data to be written to the LCD
 */
void LCD_WRITE_HIGHER_NIBBLE(unsigned char data)
{
	GPIOB_ODR &= 0XFFFFFFF0;	//Clearing the first 4 bits
	GPIOB_ODR |= data >> 4;		//load the 4-bit from data to GPIOB_ODR
	GPIOB_ODR |= 0X01 << 8;		//set the enable pin
	KM_mdelay(15);					//calling the delay function with 15msec delay
	GPIOB_ODR &= ~(0X01 << 8);	//clear the enable pin
}	

/**
 * @brief Sends the lower 4 bits of the data to the LCD
 * @details Loads the lower 4 bits of the data into GPIOB_ODR and sets the enable pin to write the data to the LCD.
 * @param data The data to be written to the LCD
 */
void LCD_WRITE_LOWER_NIBBLE(unsigned char data)
{
	GPIOB_ODR &= 0XFFFFFFF0;	//Clearing the first 4 bits
	GPIOB_ODR |= (data & 0X0F);	//load the 4-bit from data to GPIOB_ODR
	GPIOB_ODR |= 0X01 << 8;	//set the enable pin
	KM_mdelay(15);				//calling the delay function with 15msec delay
	GPIOB_ODR &= ~(0X01 << 8);	//clear the enable pin
}




/**
 * @brief Sends a command to the LCD.
 * @details This function clears the RS and R/W bits to select the command register and write operation, 
 *          then sends the higher and lower nibbles of the command data to the LCD.
 * @param data The command to be sent to the LCD.
 */
void LCD_WRITE_CMD(unsigned char data)
{
	GPIOB_ODR &= ~(0X01 << 4);		//clear the RS bit to select command register
	GPIOB_ODR &= ~(0X01 << 5);		//clear the R/W bit to select write operation
	LCD_WRITE_HIGHER_NIBBLE(data);	//calling the LCD_WRITE_HIGHER_NIBBLE function
	LCD_WRITE_LOWER_NIBBLE(data);		//calling the LCD_WRITE_LOWER_NIBBLE	function
}


/**
 * @brief Sends a data to the LCD.
 * @details This function sets the RS and R/W bits to select the data register and write operation, 
 *          then sends the higher and lower nibbles of the data to the LCD.
 * @param data The data to be sent to the LCD.
 */
void LCD_WRITE_DATA(unsigned char data)
{
	GPIOB_ODR |= 0X01 << 4;				//set the RS bit to select data register
	GPIOB_ODR &= ~(0X01 << 5);		//clear the R/W bit to select write operation
	LCD_WRITE_HIGHER_NIBBLE(data);	//calling the LCD_WRITE_HIGHER_NIBBLE function
	LCD_WRITE_LOWER_NIBBLE(data);		//calling the LCD_WRITE_LOWER_NIBBLE	function
}



/**
 * @brief Initializes the LCD in 4-bit mode.
 * @details This function enables the GPIOB clock, configures the necessary pins for 
 *          communication with the LCD, and sends initialization commands to set the 
 *          LCD in 4-bit mode. It also configures the display settings such as turning 
 *          the display on and disabling cursor blinking. Delays are used to ensure 
 *          proper timing for LCD operations.
 */
void KM_LCD_INIT(void)
{
	//RCC_AHB1ENR |= 0X01<<1;				//Enable the GPIOB clock
	GPIOB_MODER &= 0XFFFCF000;			//clearing the LCD connected pins
	GPIOB_MODER |= 0x00010555;			//configure the LCD connected pins to output mode to load 01 to each pin
	KM_mdelay(30);								//calling the delay function with 30 msec delay
	LCD_WRITE_CMD(0X33);						//calling the function  LCD_WRITE_CMD with 0x33 as argument to set the lcd to 4-bit mode
	KM_mdelay(10);								//calling the delay function with 10 msec delay
	LCD_WRITE_CMD(0x32);						//calling the function  LCD_WRITE_CMD with 0x32 as argument to set the lcd to 4-bit mode
	LCD_WRITE_CMD(0x0c);						//calling the function  LCD_WRITE_CMD with 0x0c as argument to set the lcd as dispplay on and cursor blinking off
	LCD_WRITE_CMD(0X01);						//calling the function  LCD_WRITE_CMD with 0x33 as argument to set the lcd screen clear
}


/**
 * @brief Writes a string to the LCD.
 * @details This function iterates through each character of the provided string and sends it to the LCD.
 *          If the current position is at the end of the first line (15th character), it moves the cursor
 *          to the beginning of the second line of the LCD. A delay is introduced to ensure proper timing.
 * @param str The string to be displayed on the LCD.
 */
void LCD_WRITE_STRING(unsigned char str[])
{
	int i=0;								
	while(str[i])							//print the string into lcd 
	{
		LCD_WRITE_DATA(str[i]);		//calling the LCD_WRITE_DATA fucntion to write data to the lcd
		if (i == 15){							//checking the i is 15
			LCD_WRITE_CMD(0xc0);		//calling the function  LCD_WRITE_CMD with 0xc0 as argument to set the lcd to start in second row
			KM_Delay_ms(10);					//calling the delay function with 10 msec delay
		}
		i++;									//increment the i value
	}
}


/**
 * @brief Converts an integer to a string.
 * @details This function takes an integer and its string representation is stored in the provided array.
 *          The string is stored in reverse order, so the least significant digit is stored at the first index of the array.
 * @param n The integer to be converted to a string.
 * @param str The character array to store the string representation of the integer.
 */
void myitoa(int n,unsigned char str[])
{
	int i=0,rem,arr[10],j,k;
	while(n)
	{
		rem = n %10;
		arr[i]= rem + 48;
		n = n /10;
		i++;
	}
	k=0;
	for(j=i-1;k<i;k++,j--)
	{
		str[k]=arr[j];
	}
	str[k]='\0';
}

void myitoat(int n,unsigned char str[])
{
	if(n == 0)
	{
		str[0] = '0';
		str[1] = '0';
		str[2] = 0;
	}
	if(n < 10)
	{
		str[0] = '0';
		str[1] = ((n % 10) + 48);
		str[2] = 0;
	}
	else
	{
		str[0] = ((n / 10) + 48);
		str[1] = ((n % 10) + 48);
		str[2] = 0;
	}
}


void strcopy(unsigned char str1[],unsigned char str2[])
{
	int j;
	for(j=0;str2[j];j++)
	{
		str1[j] = str2[j];
	}
	str2[j] = '\0';
}