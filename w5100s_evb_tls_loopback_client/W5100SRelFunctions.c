#include "w5100s_evb.h"
#include "W5100SRelFunctions.h"
#include "stm32f10x_conf.h"

void W5100SInitialze(void)
{
	intr_kind temp;
	unsigned char W5100S_AdrSet[2][4] = {{2,2,2,2},{2,2,2,2}};
	/*
	 */
	//reg_wizchip_cs_cbfunc(csEnable, csDisable);
	//reg_wizchip_spi_cbfunc(spiReadByte,spiWriteByte);
	temp = IK_DEST_UNREACH;

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5100S_AdrSet) == -1)
	{
		printf("W5100S initialized fail.\r\n");
	}

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
	{
		printf("W5100S interrupt\r\n");
	}

	do{//check phy status.
		if(ctlwizchip(CW_GET_PHYLINK,(void*)&temp) == -1){
			printf("Unknown PHY link status.\r\n");
		}
	}while(temp == PHY_LINK_OFF);
}

uint8_t spiReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, 0xff);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(W5100S_SPI);
}

void spiWriteByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, byte);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(W5100S_SPI);
}

inline void csEnable(void)
{
	GPIO_ResetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void csDisable(void)
{
	GPIO_SetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void resetAssert(void)
{
	GPIO_ResetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

inline void resetDeassert(void)
{
	GPIO_SetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

void W5100SReset(void)
{
	int i,j,k;
	k=0;
	GPIO_ResetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
	GPIO_SetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
}
