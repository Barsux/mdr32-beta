#include "main.h"

int main(){
	MAC mac = {0x62, 0x26, 0x62, 0xAA, 0xBB, 0xCC};
	char hell[] 						= "Hello, world!\r";
	MDR_RST_CLK->PER_CLOCK 	|= (1 << 6) | (1 << 23) | (1 << 24);
	MDR_PORTD->ANALOG 			|= (1<<7);
	MDR_PORTD->OE 					|= (1<<7);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR7_Pos);
	MDR_PORTD->RXTX 				|= (1<<7);
	volatile U16 counter 		= 0;
	volatile U8 data 				= 0;
	//uart_tx_init();
	eth_init(mac);
	
	char text[] = "HELL";
	
	while(1){
		debug_eval(MDR_ETHERNET1);
		DoNetworkStuff(MDR_ETHERNET1);
		if(counter == 65535){
			PRINT("BLINK");
			MDR_PORTD->RXTX ^= 1<<7;
			data++;
		}
		counter++;
	}
}

