#include "main.h"
#define PRINT(...) sendstr(__VA_ARGS__)
MAC mac = {0x62, 0x26, 0x62, 0xAA, 0xBB, 0xCC};

void cpu_init()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_BKP, ENABLE);
  RST_CLK_HSEconfig (RST_CLK_HSE_ON); 
  while (RST_CLK_HSEstatus () != SUCCESS);
  RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul14); 
  RST_CLK_CPU_PLLcmd (ENABLE); 
  while (RST_CLK_CPU_PLLstatus () != SUCCESS);
  RST_CLK_CPUclkPrescaler (RST_CLK_CPUclkDIV1); 
  RST_CLK_CPU_PLLuse (ENABLE); 
  RST_CLK_CPUclkSelection (RST_CLK_CPUclkCPU_C3); 
}


void init(){
	MDR_RST_CLK->PER_CLOCK 	|= (1 << 6) | (1 << 23) | (1 << 24);
	MDR_PORTD->ANALOG 			|= (1<<7);
	MDR_PORTD->OE 					|= (1<<7);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR7_Pos);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR8_Pos);
	MDR_PORTD->RXTX 				|= (1<<8);
	cpu_init();
	SystemCoreClockUpdate();
	
	uart_init();
	eth_init(mac);
	MDR_PORTD->RXTX 				|= (1<<7);
	PRINT("INIT 0 FUCKS GIVEN");
}

int main(){
	init();
	
	
	volatile U16 counter 		= 0;
	volatile U16 data 				= 0;		
	U8 steps = 4;
	/*
	for(data = 0; data < steps; data++){
		for(int i = 0; i < 4; i++) {
			counter = 0;
			while (counter != 65535){
				counter++;
			}
		}
		MDR_PORTD->RXTX ^= 1<<7;
	}
	*/
	U16 datax;
	while(1){
		data = UART_ReceiveData(MDR_UART1);
		if(data != 0) UART_SendData(MDR_UART1, data);
		//datax = UART_ReceiveData(MDR_UART1);
		//UART_SendData(MDR_UART1, datax);
		//debug_eval(MDR_ETHERNET1);
	}
}

