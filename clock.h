void timer0_set_1ms(void)
{
	TH0=0xFC;
	TL0=0x18;
}
void timer0_mode_tmod_init(void)
{
	TMOD |= 0x01;
	timer0_set_1ms();
	ET0=1;
	EA=1;
	TR0=1;

}

