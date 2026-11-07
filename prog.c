#include <stc89.h>
#include "sleep.h"
#include "hc595.h"
#include "clock.h"
#include "keyBoard4.h"

unsigned char buff[8]= {0,0,0,0,
						0,0,0,0
					   };
unsigned char moving[9]= {0,0,0,0,
						  0,0,0,0,0
						 };
unsigned char placed[8]= {0,0,0,0,
						  0,0,0,0
						 };
unsigned int rand_seed=4;
unsigned int generate_rand_seed(void){
	unsigned char __xdata *p;
	unsigned seed=0;
	p=0x80;
	unsigned char i;
	for(i=0;i<16;i++)
		seed=29*seed+(*p++);
	return seed;

}
unsigned char small_rand(void){
	rand_seed=rand_seed*25173+13849;
	return (unsigned char)((rand_seed>>8)%5);
		

}
unsigned line_is_empty(unsigned char j) {
	unsigned char i=0;
	i|=moving[8]|moving[1];
	i|=moving[2]|moving[3];
	i|=moving[4]|moving[5];
	i|=moving[6]|moving[7];
	i>>=j;
	if (i&1) {
		return 0;
	}
	else
	{	return 1;
	}
}
void try_move_left(void) {
	unsigned char i=0;
	for (i=1; i<9 ;i++) {
		if(moving[i]<<1&placed[i-1])
			return;
	}
	for (i=1; i<9; i++)
		moving[i]<<=1;

}
void try_move_right(void) {
	unsigned char i=0;
	for (i=1; i<9; i++) {
		if(moving[i]<<1&placed[i-1])
			return;
	}
	for (i=1; i<9; i++)
		moving[i]>>=1;

}

void try_clean_line(void){
unsigned char row;

    for (row = 7; row < 8; row--)   
    {
        if (placed[row] == 0xFF)
        {
            unsigned char r;

            for (r = row; r > 0; r--)
            {
                placed[r] = placed[r - 1];
            }
            placed[0] = 0x00;   
			row++;
        }
    }


}
void spawn_new_block(void){
	unsigned char type=small_rand();
	
	switch (type){
		case 0:
			moving[0]=0x38;
		case 1:
			moving[0]=0x18;
			break;
		case 2:
			moving[0]=0x10;
			moving[1]=0x10;
			break;
		case 3:
			moving[0]=0x08;
			break;
		case 4:
			moving[0]=0x18;
			moving[1]=0x10;
			break;
}
	
}
void merge(void) {
	unsigned char i=0;
	placed[0]|=moving[1];
	placed[1]|=moving[2];
	placed[2]|=moving[3];
	placed[3]|=moving[4];
	placed[4]|=moving[5];
	placed[5]|=moving[6];
	placed[6]|=moving[7];
	placed[7]|=moving[8];
	for (i=0; i<9; i++)
		moving[i]=0x00;

		spawn_new_block();
}
void moving_move_down(void) {
	if (moving[8]) {
		merge();//moving block reaches the bottom
		return;
	}
	
	unsigned char i=0;
	for (i=0; i<8; i++) {
		if (moving[i]&placed[i]){
			merge();
			return;
		}
	}
	//move down
	moving[8]=moving[7];
	moving[7]=moving[6];
	moving[6]=moving[5];
	moving[5]=moving[4];
	moving[4]=moving[3];
	moving[3]=moving[2];
	moving[2]=moving[1];
	moving[1]=moving[0];
	moving[0]=0x00;
	//move[0] is the top above the screen.And it won't show on the screen.


}
void main(void)

{
	rand_seed=generate_rand_seed();
	timer0_mode_tmod_init();
	hc595_init();
	unsigned char key;
	unsigned char i=0;
	spawn_new_block();
	//spawn new block, game starts here.
	while (1) {
		key=scanKey();

		switch (key) {
		case 1 :
			if (line_is_empty(0))
				try_move_right();
			break;
		case 2 :
			if (line_is_empty(7))
				try_move_left();
			break;
		case 3 :
			moving_move_down();
			break;

		case 4 :
			//for test only
			moving[0]=0x0C;
			break;

		}
		try_clean_line();
		for (i=0; i<=7; i++) {
			buff[i]=moving[i+1]|placed[i];

			}
		
		}
	}



void timer0_isr(void) __interrupt (1)
{
	static unsigned char row = 0;
	static unsigned int tick = 0;
	timer0_set_1ms();
	P0=0xFF;
	hc595_next_row(row);
	hc595_dump();
	P0=~(buff[row]);
	if (row == 7)
		row=0;
	else
		row++;
	P0=0xFF;
	if (tick == 512){
		moving_move_down(); 
		tick=0; }
	else
		tick++;
		

}
