#include "stm32f401rbt6.h"
#include "systemticktimer.h"

void LCD_WRITE_HIGHER_NIBBLE(unsigned char data);
void LCD_WRITE_LOWER_NIBBLE(unsigned char data);
void LCD_WRITE_CMD(unsigned char data);
void LCD_WRITE_DATA(unsigned char data);
void KM_LCD_INIT(void);
void LCD_WRITE_STRING(unsigned char str[]);
void myitoa(int,unsigned char * str);
void myitoat(int,unsigned char * str);
void strcopy(unsigned char str1[],unsigned char str2[]);
