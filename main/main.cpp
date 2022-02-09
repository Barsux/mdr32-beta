#include "main.h"
#define PRINT(...) sendstr(__VA_ARGS__)
MAC mac = {0x62, 0x26, 0x62, 0xAA, 0xBB, 0xCC};

void cpu_init()
{
	MDR_RST_CLK->HS_CONTROL 	|= RST_CLK_HS_CONTROL_HSE_ON;
	while(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY);
	MDR_RST_CLK->CPU_CLOCK 		|= 0x2<< RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos;
	MDR_RST_CLK->CPU_CLOCK 		|= 0x1<< RST_CLK_CPU_CLOCK_HCLK_SEL_Pos;
	
	MDR_RST_CLK->PLL_CONTROL 	|= (2) << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos;
	MDR_RST_CLK->PLL_CONTROL 	|= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
	while(!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY));
	
	MDR_EEPROM->CMD 					|= 7 << EEPROM_CMD_DELAY_Pos;	
	MDR_RST_CLK->CPU_CLOCK 		|= 1 << RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos;
}

void init(){
	cpu_init();
	uart_init();
	PRINT("==================INITS==================");
	PRINT("UART INIT SUCSCESSFUL");
	eth_init(mac);
	PRINT("ETH INIT SUCSCESSFUL");
	PRINT("=================/INITS==================");
	PRINT("%d", SystemCoreClock);
}

int main(){
	PRINT("MAIN WOID BEGIN");
	init();
	MDR_RST_CLK->PER_CLOCK 	|= (1 << 6) | (1 << 23) | (1 << 24);
	MDR_PORTD->ANALOG 			|= (1<<7);
	MDR_PORTD->OE 					|= (1<<7);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR7_Pos);
	MDR_PORTD->RXTX 				|= (1<<7);
	volatile U16 counter 		= 0;
	volatile U8 data 				= 0;		
	char text[] = "HELL";
	
	while(1){
		debug_eval(MDR_ETHERNET1);
		if(counter == 65535){
			PRINT("BLINK");
			MDR_PORTD->RXTX ^= 1<<7;
			data++;
		}
		counter++;
	}
}

