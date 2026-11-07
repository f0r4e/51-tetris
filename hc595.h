/*A driver for hc595*/
__sbit __at (0xB6) shiftRegisterClock;
__sbit __at (0xB5) registerClockLatch;
__sbit __at (0xB4) serialDataInput;

void hc595_write(unsigned char data)
{	
	int j;
	for (j=0;j<8;j++)
	{
		serialDataInput = data & 0x01;
		data >>=1;
		shiftRegisterClock = 0;
		NOP();
		shiftRegisterClock = 1;
		NOP();
	}
	}

void hc595_dump(void)
{
	registerClockLatch=0;
	NOP();
	registerClockLatch=1;
	NOP();
	registerClockLatch=0;
}

void hc595_init(void)
{

        int j;
        for (j=0;j<8;j++)
        {
                serialDataInput = 0;
                shiftRegisterClock = 0;
                NOP();
                shiftRegisterClock = 1;
                NOP();
        }
        }
void hc595_next_row(unsigned char row)
{
        serialDataInput = (row==0)?1:0;
        shiftRegisterClock = 0;
        NOP();
        shiftRegisterClock = 1;
	NOP();
        shiftRegisterClock = 0;
}
