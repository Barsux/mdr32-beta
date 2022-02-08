#ifndef UARTH
#define UARTH

#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK
#include "MDR32F9Qx_uart.h"    
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT
#include "string.h"
#include "stdio.h"
#include <stdarg.h>
#include <inttypes.h>

int sendstr(char * fmt, ...);
void usart_tx_byte(const char data);
#endif 
