#include "main.h"
#define PRINT(...) sendstr(__VA_ARGS__)
MAC src = {0x62, 0x26, 0x62, 0xAA, 0xBB, 0xCC};
MAC dst = {0x70, 0x85, 0xC2, 0xC8, 0xBF, 0x25};
float time_clock_coeff;


volatile uint32_t HIGHUTC = 0;

uint64_t get_utc(){
	return (((uint64_t)HIGHUTC << 32) | ((uint64_t)MDR_TIMER1->CNT)) * time_clock_coeff;
}

void set_utc(uint64_t utc){
	utc /= time_clock_coeff;
	HIGHUTC = utc >> 32;
	MDR_TIMER1->CNT = utc;
}

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
	time_clock_coeff = (float)1000000000/SystemCoreClock;
	
	
}

extern "C" void TIMER1_IRQHandler(void)
{
	if (TIMER_GetITStatus(MDR_TIMER1, TIMER_STATUS_CNT_ARR) == SET)
  {
		MDR_TIMER1->STATUS = ~TIMER_STATUS_CNT_ARR;
		HIGHUTC++;
  }
}

void timer_init(){
	TIMER_CntInitTypeDef timer_struct;
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1 );
	timer_struct.TIMER_ARR_UpdateMode 	= TIMER_ARR_Update_Immediately;
	timer_struct.TIMER_BRK_Polarity 		= TIMER_BRKPolarity_NonInverted;
	timer_struct.TIMER_CounterDirection = TIMER_CntDir_Up;
	timer_struct.TIMER_CounterMode 			= TIMER_CntMode_ClkFixedDir;
	timer_struct.TIMER_ETR_FilterConf 	= TIMER_Filter_1FF_at_TIMER_CLK;
	timer_struct.TIMER_ETR_Polarity 		= TIMER_ETRPolarity_NonInverted;
	timer_struct.TIMER_ETR_Prescaler 		= TIMER_ETR_Prescaler_None;
	timer_struct.TIMER_EventSource 			= TIMER_EvSrc_None;
	timer_struct.TIMER_FilterSampling 	= TIMER_FDTS_TIMER_CLK_div_1;
	timer_struct.TIMER_IniCounter 			= 0;
	timer_struct.TIMER_Period 					= 0xFFFFFFFF;
	timer_struct.TIMER_Prescaler 				= 0;
	
	TIMER_CntInit(MDR_TIMER1, &timer_struct);
	
	NVIC_EnableIRQ(TIMER1_IRQn);
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
	
	TIMER_Cmd(MDR_TIMER1, ENABLE);
}

void init(){
	MDR_RST_CLK->PER_CLOCK 	|= (1 << 6) | (1 << 23) | (1 << 24);
	MDR_PORTD->ANALOG 			|= (1<<7);
	MDR_PORTD->OE 					|= (1<<7);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR7_Pos);
	MDR_PORTD->PWR 					|= (0x1 << PORT_PWR8_Pos);
	cpu_init();                                                       
	
	uart_init();
	PRINT("%f", time_clock_coeff);
	
	MDR_PORTD->RXTX 				|= (0<<8) | (1<<7);
	//eth_init(src);
	timer_init();
}

int main(){
	init();
	uint64_t time = 1645644836000000;
	set_utc(time);
	uint32_t counter;
	uint32_t data1 = 0;
	PRINT("%llu", get_utc());
	for(int i = 0; i < 10; i++){
	for(counter = 1; counter < 100000000; counter++){
		data1 = MDR_TIMER1->CNT;
	}}
	PRINT("%llu", get_utc());
	
	

}

