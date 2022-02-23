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
	MDR_ETHERNET1->PHY_Status |= 0 << 1;
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
	status_reg = ETHERNETx->ETH_IFR;

	if(ETHERNETx->ETH_R_Head != ETHERNETx->ETH_R_Tail){
	/*if(status_reg & ETH_MAC_IT_RF_OK ){*/
		ETH_StatusPacketReceptionStruct.Status = ETH_ReceivedFrame(ETHERNETx, InputFrame);
		PRINT("Accept packet with size=%u", ETHERNETx->ETH_R_Tail);
	}
}

void debug_send(MDR_ETHERNET_TypeDef * ETHERNETx, MAC src, MAC dst){
	uint8_t size;
	uint8_t packet[size + 5]; //4 Bytes before/ 1Byte therefore
	memset(packet, 0, size+6);
	packet[0] = size;
	struct ethheader *eth = (struct ethheader *)packet[4];
	for(int i = 0; i < 6; i++){
		eth->h_source[i] = src[i];
		eth->h_dest[i] = dst[i];
	}
	eth->h_proto = 0x0800;
	packet[size-1] = size - sizeof(ethheader);
	PRINT("SENDING");
	ETH_SendFrame(ETHERNETx, (uint32_t *) packet, (uint32_t) packet[0]);
	PRINT("SENDED");
}

int	SendPacket(void* buffer, int size, MAC macsrc, MAC macdst)
{
	uint16_t i;
	uint32_t tmp, head, tail;
	uint32_t *src, *dst;
	uint16_t space[2];

	tail = MDR_ETHERNET1->ETH_R_Tail;
	head = MDR_ETHERNET1->ETH_R_Head;

	if(head>tail)
	{
		space[0]=head-tail;
		space[1]=0;
	} else
	{
		space[0]=0x2000-tail;
		space[1]=head-0x1000;
	}

	if(size>(space[0]+space[1]-8))	return 0;	

	tmp=size;
	src=(U32*)buffer;
	dst=(U32*)(0x38000000+tail);
	
	*dst++ =tmp;
	space[0]-=4;
	if((U32)dst>0x1FFC)	dst=(U32*)0x38001000;

	tmp=(size+3)/4;

	if(size<=space[0])
	{
		for(i=0; i<tmp; i++)
			*dst++ = *src++;
	}
	else
	{
		tmp-=space[0]/4;
		for(i=0;i<(space[0]/4);i++)
			*dst++ = *src++;
		dst=(uint32_t*)0x38001000;
		for(i=0;i<tmp;i++)
			*dst++ = *src++;
	}
	if((U32)dst>0x1FFC)	dst=(U32*)0x38001000;
	tmp=0;
	*dst++ =tmp;
	if((U32)dst>0x1FFC)	dst=(U32*)0x38001000;

	MDR_ETHERNET1->ETH_X_Tail=(U32)dst;
	return	size;
}

void create_packet(MAC src, MAC dst){
	U8 packet[1000];
	memset(packet, 0, 1000);
	struct ethheader *eth = (struct ethheader *)packet;
	memcpy(eth->h_source, src, 6);
	memcpy(eth->h_dest, dst, 6);
	eth->h_proto = 0x0800;
	SendPacket(packet, 1000, src, dst);
}
