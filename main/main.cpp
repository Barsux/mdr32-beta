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
	cpu_init();                                                       
	
	uart_init();
	
	MDR_PORTD->RXTX 				|= (0<<8) | (1<<7);
	eth_init(src);
	time_init();	
	PRINT("INIT");
}

int main(){
	init();
	U32 counter, TIMER_STAT, CYCLE_COUNTER;
	U16 ETH_STAT;
	U16 RXTX_STAT;
	U64 delays[20];
	memset(delays, 0, 20);
	for(counter = 0; counter < 20000; counter++){
		PRINT("Cycle starts");
		for(CYCLE_COUNTER = 0; CYCLE_COUNTER < 10000000; CYCLE_COUNTER++){
			TIMER_STAT = MDR_TIMER1->CNT;
		}
		create_packet(src, dst);
		PRINT("Cycle ends");
	}
}


