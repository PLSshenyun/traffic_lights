#include<reg52.h>
#include<intrins.h>

// LED(RGY)
sbit NS_RED = P0^0;  // North South 
sbit NS_GREEN = P0^1;
sbit NS_YELLOW = P0^2;
sbit EW_RED = P0^3;   // East  West
sbit EW_GREEN = P0^4;
sbit EW_YELLOW = P0^5;

// Button External Interrupt
sbit Btn1 = P3^2;  // RED LIGHT
sbit Btn2 = P3^3;  // YELLOW BLINK
unsigned int FLAG = 1;  // Enter the Main

sbit SEG1 = P1^0;
sbit SEG2 = P1^1;
#define DataPort P2 // DUAN define

// Digital Anode Code Value
unsigned char tab_disp[]= {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

//Init Time = 25
unsigned char N=25;
unsigned char step=1;
unsigned char i=0;
unsigned char f=0;


void delay(int t );
void deal(char m);
void display(char dat);

void LEDS_RED();
void LEDS_YELLOW();
void LEDS_DARK();
void InitINT();

void main()
{
	unsigned char i;

	//Init EA
	InitINT();
	LEDS_DARK();

	while(1)
	{
		// normal light mode
		if(FLAG == 1)
		{
			
			// Time count
			if(step == 1) // step1 program
			{
				if(f == 0) // if first to step 1, give the number
				{
					i = 25;
					f = 1;
				}
				else	// if not first to step 1, sub the i or change mode
				{
					if(i>1)
					{
						i--;
					}else
					{
						step = 2;
						f = 0;
					}
				}
			}
			if(step == 2)
			{
				if(f == 0) // if first to step 2, give the number
				{
					i = 5;
					f = 1;
				}
				else	// if not first to step 2, sub the i or change mode
				{
					if(i>1)
					{
						i--;
					}else
					{
						step = 3;
						f = 0;
					}
				}
			}
			if(step == 3)
			{
				if(f == 0)  // if first to step 3, give the number
				{
					i = 25;
					f = 1;
				}
				else	// if not first to step 3, sub the i or change mode
				{
					if(i>1)
					{
						i--;
					}else
					{
						step = 4;
						f = 0;
					}
				}
			}
			if(step == 4)
			{
				if(f == 0)  // if first to step 4, give the number
				{
					i = 5;
					f = 1;
				}
				else	// if not first to step 4, sub the i or change mode
				{
					if(i>1)
					{
						i--;
					}else
					{
						step = 1;
						f = 0;
					}
				}
			}
			
			if(step == 1) // NS_red and EW_green
			{
				NS_RED = 1;
				NS_GREEN = 0;
				NS_YELLOW = 0;
				EW_RED = 0;
				EW_GREEN = 1;
				EW_YELLOW = 0;
			}
			if(step == 2) // NS_red and EW_yellow
			{
				NS_RED = 1;
				NS_GREEN = 0;
				NS_YELLOW = 0;
				EW_RED = 0;
				EW_GREEN = 0;
				EW_YELLOW = 1;
			}
			if(step == 3) // NS_green and EW_red
			{
				NS_RED = 0;
				NS_GREEN = 1;
				NS_YELLOW = 0;
				EW_RED = 1;
				EW_GREEN = 0;
				EW_YELLOW = 0;
			}
			if(step == 4) // NS_green and EW_yellow
			{
				NS_RED = 0;
				NS_GREEN = 0;
				NS_YELLOW = 1;
				EW_RED = 1;
				EW_GREEN = 0;
				EW_YELLOW = 0;
			}
			
			deal(i); // show the number
		}
		
		// yellow light mod
		if(FLAG == 2)
		{
			LEDS_YELLOW();
			SEG1 = 0;
			SEG2 = 0;
			DataPort = 0XFF;
		}
		
		// red light mod
		if(FLAG == 3)
		{
			LEDS_RED();
			SEG1 = 0;
			SEG2 = 0;
			DataPort = 0XFF;
		}
	}
}


// Digital Tube Show
void display(char dat)
{
	SEG1 = 0;
	SEG2 = 0;	 // Dark the WEI
	DataPort = 0XFF;   // Dark the DUAN
	DataPort = tab_disp[dat/10];  //show the Anode Value
	if(dat/10)
		SEG1=1;
	delay(1000);

	DataPort=0XFF;
	SEG1=0;
	SEG2=0;	 // Dark the WEI
	P2=tab_disp[dat%10];   //show the Anode Value
	SEG2=1;
	delay(1000);
}
//Iterator Show 60 times
void deal(char m)
{
	int t=60;
	while(t--)
		display(m);
}




// Init the External Interrupt
void InitINT()
{
	IT0 = 1;    //set INT0 int type (1:Falling 0:Low level)
	EX0 = 1;    //enable INT0 interrupt
	IT1 = 1;    //set INT0 int type (1:Falling 0:Low level)
	EX1 = 1;    //enable INT0 interrupt
	EA = 1;     //open global interrupt switch}
}

// Enter the External Interrupt
void ExInt0() interrupt 0     
{
	if(FLAG == 1)
	{
		FLAG = 2;
	}
	else if(FLAG == 2)
	{
		FLAG = 1;
	}
}

void ExInt1() interrupt 2
{
	if(FLAG == 1)
	{
		FLAG = 3;
	}
	else if(FLAG == 3)
	{
		FLAG = 1;
	}
}



// DARK
void LEDS_DARK()
{
	NS_RED = 0;
	NS_GREEN = 0;
	NS_YELLOW = 0;
	EW_RED = 0;
	EW_GREEN = 0;
	EW_YELLOW = 0;
}
// RED
void LEDS_RED()
{
	NS_RED = 1;  // LIGHT
	NS_GREEN = 0;
	NS_YELLOW = 0;
	EW_RED = 1;  // LIGHT
	EW_GREEN = 0;
	EW_YELLOW = 0;
}
// YELLOW BLINK
void LEDS_YELLOW()
{
	NS_RED = 0;
	NS_GREEN = 0;
	NS_YELLOW = 1;  // LIGHT
	EW_RED = 0;
	EW_GREEN = 0;
	EW_YELLOW = 1;  // LIGHT
}




void delay(int t )
{
	while(t--);
}






