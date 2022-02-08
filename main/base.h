#ifndef BASEH
#define BASEH
#include "MDR1986VE1T.h"                // Keil::Device:Startup
#include "MDR32F9Qx_dma.h"              // Keil::Drivers:DMA
#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK
#include "MDR32F9Qx_uart.h"             // Keil::Drivers:UART
#include "MDR32F9Qx_eth.h"              // Keil::Drivers:Ethernet
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT
#include "MDR32F9Qx_timer.h"            // Keil::Drivers:TIMER

//#include "MDR32F9Qx_eeprom.h"           // Keil::Drivers:EEPROM

#include "inttypes.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "uart.h"
#include "time.h"

#define PRINT(...) sendstr(__VA_ARGS__); 

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;
typedef unsigned int U4;
typedef unsigned short U2;
typedef int64_t I64;
typedef int32_t I32;
typedef int16_t I16;
typedef int8_t I8;
typedef int I4;
typedef short I2;
typedef U64 UTC;
typedef U8 MAC[6];
typedef U32 IP4;
typedef char * STR;
typedef unsigned char UC;


#endif

