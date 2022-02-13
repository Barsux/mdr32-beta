#include "ethernet.h"
#define PRINT(...) sendstr(__VA_ARGS__)

int eth_init(MAC &srcMAC){
	//PRINT("ETH INIT");
	ETH_ClockDeInit();
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE); // Dma here now, idk.
	

	RST_CLK_HSE2config(RST_CLK_HSE2_ON);
	if(RST_CLK_HSE2status() == ERROR) return -1;
	
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);
	ETH_BRGInit(ETH_HCLKdiv1);
	ETH_ClockCMD(ETH_CLK1, ENABLE);
	ETH_DeInit(MDR_ETHERNET1);
	
	
	ETH_InitTypeDef  ETH_InitStruct;
	ETH_StructInit((ETH_InitTypeDef * ) &ETH_InitStruct);
	
	ETH_InitStruct.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
	ETH_InitStruct.ETH_Transmitter_RST = SET;
	ETH_InitStruct.ETH_Receiver_RST = SET;
	
	ETH_InitStruct.ETH_Buffer_Mode = ETH_BUFFER_MODE_LINEAR;
	ETH_InitStruct.ETH_Source_Addr_HASH_Filter = DISABLE;

	ETH_InitStruct.ETH_MAC_Address[2] = ((int)srcMAC[0]<<8)| (int)srcMAC[1];
	ETH_InitStruct.ETH_MAC_Address[1] = ((int)srcMAC[2]<<8)| (int)srcMAC[3];
	ETH_InitStruct.ETH_MAC_Address[0] = ((int)srcMAC[4]<<8)| (int)srcMAC[5];
	
	ETH_InitStruct.ETH_Dilimiter = 0x5DC;
	
	ETH_Init(MDR_ETHERNET1, (ETH_InitTypeDef *) &ETH_InitStruct);
	TCPLowLevelInit();
	ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
	ETH_Start(MDR_ETHERNET1);
	return 1;
}

void debug_eval(MDR_ETHERNET_TypeDef * ETHERNETx){
	uint32_t InputFrame[1514/4];
	uint16_t status_reg;
	ETH_StatusPacketReceptionTypeDef ETH_StatusPacketReceptionStruct;

	status_reg = ETH_GetMACITStatusRegister(ETHERNETx);

	if(ETHERNETx->ETH_R_Head != ETHERNETx->ETH_R_Tail){
	/*if(status_reg & ETH_MAC_IT_RF_OK ){*/
		ETH_StatusPacketReceptionStruct.Status = ETH_ReceivedFrame(ETHERNETx, InputFrame);
		PRINT("Accept packet with size=%u", ETHERNETx->ETH_R_Tail);
	}
}
