#include "main.h"
#define PRINT(...) sendstr(__VA_ARGS__)
MAC src = {0x62, 0x26, 0x62, 0xAA, 0xBB, 0xCC};
MAC dst = {0x70, 0x85, 0xC2, 0xC8, 0xBF, 0x25};

void cpu_init()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_BKP, ENABLE);
  RST_CLK_HSEconfig (RST_CLK_HSE_ON); 
  while (RST_CLK_HSEstatus () != SUCCESS);
  RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul9); 
  RST_CLK_CPU_PLLcmd (ENABLE); 
  while (RST_CLK_CPU_PLLstatus () != SUCCESS);
  RST_CLK_CPUclkPrescaler (RST_CLK_CPUclkDIV1); 
  RST_CLK_CPU_PLLuse (ENABLE); 
  RST_CLK_CPUclkSelection (RST_CLK_CPUclkCPU_C3); 
	SystemCoreClockUpdate();
}


void init(){
	MDR_RST_CLK->PER_CLOCK 	|= (1 << 6) | (1 << 23) | (1 << 24);
	MDR_PORTD->ANALOG 			|= (1<<7);
	MDR_PORTD->OE 					|= (1<<7);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR7_Pos);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR8_Pos);
	MDR_PORTD->RXTX 				|= (1<<8);
	cpu_init();
	
	uart_init();
	eth_init(src);
	MDR_PORTD->RXTX 				|= (1<<7);
	PRINT("INIT 0 FUCKS GIVEN");
}

int main(){
	init();
	
	
	volatile U16 counter 		= 0;
	volatile U16 data 				= 0;		
	U8 steps = 4;
	U16 datax;
	while(1){
		if((MDR_ETHERNET1->PHY_Status&0x02)==0x00)	MDR_PORTB->SETTX=1<<15;	
		else									MDR_PORTB->CLRTX=1<<15;

		if((MDR_ETHERNET1->PHY_Status&0x08)==0x00)	MDR_PORTB->SETTX=1<<14;
		else									MDR_PORTB->CLRTX=1<<14;
		if(counter == 65535) {
			MDR_PORTD->RXTX ^= (1<<7);
			//debug_send(MDR_ETHERNET1, src, dst);
			create_packet(src, dst);
			PRINT("SENDED");
		}
		counter++;
	}
}

