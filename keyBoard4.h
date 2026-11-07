#define KEY1_VALUE 1
#define KEY2_VALUE 2
#define KEY3_VALUE 3
#define KEY4_VALUE 4
#define NONE_PRESS 0
__sbit __at (0xB1) KEY1;
__sbit __at (0xB0) KEY2;
__sbit __at (0xB2) KEY3;
__sbit __at (0xB3) KEY4;

int scanKey(void){
	static char keyUp =1;
	char keyValue = 0;
	if(KEY1 == 0)
		keyValue = KEY1_VALUE; 
	else if (KEY2 == 0)
		keyValue = KEY2_VALUE; 
	else if (KEY3 == 0)
		keyValue = KEY3_VALUE; 
	else if (KEY4 == 0)
		keyValue = KEY4_VALUE; 
	if (keyUp && keyValue ){
		keyUp=0;
		return keyValue;
	} else if (keyValue == 0)
		{
			keyUp = 1;
		
		}
	return NONE_PRESS;
}
