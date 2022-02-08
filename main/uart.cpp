#include "uart.h"

bool init = false;
int uart_tx_init(){
	
	PORT_InitTypeDef uart_port;
	UART_InitTypeDef uart_init;
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	uart_port.PORT_Pin = PORT_Pin_3;
	uart_port.PORT_OE = PORT_OE_OUT;
	uart_port.PORT_PULL_UP = PORT_PULL_UP_OFF;
	uart_port.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	uart_port.PORT_PD_SHM = PORT_PD_SHM_OFF;
	
	uart_port.PORT_PD = PORT_PD_DRIVER;
	uart_port.PORT_GFEN = PORT_GFEN_OFF;
	uart_port.PORT_FUNC = PORT_FUNC_MAIN;
	uart_port.PORT_SPEED = PORT_SPEED_MAXFAST;
	uart_port.PORT_MODE = PORT_MODE_DIGITAL;
	
	PORT_Init(MDR_PORTC, &uart_port);
	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
	UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
	uart_init.UART_BaudRate = 115200;
	uart_init.UART_WordLength = UART_WordLength8b;
	uart_init.UART_StopBits = UART_StopBits1;
	uart_init.UART_Parity = UART_Parity_0;
	uart_init.UART_FIFOMode = UART_FIFO_OFF;
	uart_init.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE;
		
	UART_Init(MDR_UART1, &uart_init);
	UART_Cmd(MDR_UART1, ENABLE);
	init = true;	
	return 1;
}

int sendchar(int c)
{
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFF) == SET);
    UART_SendData(MDR_UART1, (uint8_t) c);
  
    return (c);
}

int sendstr(char * fmt, ...){
	char buff[2048];
  va_list va; va_start(va, fmt);
  int n = vsnprintf(buff, (int)sizeof(buff)-2, fmt, va); buff[sizeof(buff)-1]=0;
  va_end(va);
	if(!init) uart_tx_init();
	for(int i = 0; i < n; i++) sendchar(buff[i]);
	sendchar('\r');
	return n;
}

