#include <stdio.h>

/*include header files for board*/
#include "w5100s_evb.h" 
#include "stm32f10x_conf.h"

/*include header files for W5100S*/
#include "wizchip_conf.h"
#include "W5100SRelFunctions.h"

/*include header files for third party libraries*/
#include "SSLInterface.h"

//#include MBEDTLS_CONFIG_FILE

/*DEFINITION*/
#define ETH_MAX_BUF_SIZE	2048
#define SERVER_PORT			443
#define SOCK_SMTP			0

/*Default network informations*/
wiz_NetInfo defaultNetInfo = { .mac = {0x00,0x08,0xdc,0xff,0xee,0xdd},
							.ip = {192,168,128,128},
							.sn = {255,255,255,0},
							.gw = {192,168,128,1},
							.dns = {168, 126, 63, 1},
							.dhcp = NETINFO_STATIC};

unsigned char gServer_IP[4] = {192,168,128,244};
unsigned char ethBuf0[ETH_MAX_BUF_SIZE];

extern volatile unsigned long globalTimer;
void delay100ms(uint32_t ttd);
void print_network_information(void);

void delay100ms(uint32_t ttd)
{
	unsigned long tempTime = globalTimer + ttd;

	while(tempTime > globalTimer){}
}

void print_network_information(void)
{
	wizchip_getnetinfo(&defaultNetInfo);
	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",defaultNetInfo.mac[0],defaultNetInfo.mac[1],defaultNetInfo.mac[2],defaultNetInfo.mac[3],defaultNetInfo.mac[4],defaultNetInfo.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",defaultNetInfo.ip[0],defaultNetInfo.ip[1],defaultNetInfo.ip[2],defaultNetInfo.ip[3]);
	printf("SM Mask	   : %d.%d.%d.%d\n\r",defaultNetInfo.sn[0],defaultNetInfo.sn[1],defaultNetInfo.sn[2],defaultNetInfo.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",defaultNetInfo.gw[0],defaultNetInfo.gw[1],defaultNetInfo.gw[2],defaultNetInfo.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",defaultNetInfo.dns[0],defaultNetInfo.dns[1],defaultNetInfo.dns[2],defaultNetInfo.dns[3]);
}

int main(void)
{
	int len, rc, server_fd = 0;

	wiz_tls_context tlsContext;

	/*  Hardware initialization  */

	gpioInitialize();
	usartInitialize();
	timerInitialize();
	FSMCInitialize();

	/*  reset W5100S  */
	resetAssert();
	delay100ms(2);
	resetDeassert();
	delay100ms(2);

	/*  initialize W5100S  */
	W5100SInitialze();
	wizchip_setnetinfo(&defaultNetInfo);
	print_network_information();

	/*  initialize ssl context  */
	wiz_tls_init(&tlsContext,&server_fd);

	/*  Connect to the ssl server  */
	wiz_tls_connect(&tlsContext, SERVER_PORT, gServer_IP);

	while(1){

		/* Read data from SSL channel  */
		len = wiz_tls_read(&tlsContext, ethBuf0, ETH_MAX_BUF_SIZE);

		if(len > 0)
		{
			printf("%s : %d \r\n",ethBuf0, len);
			/*  Write data to the SSL channel  */
			wiz_tls_write(&tlsContext, ethBuf0, len);
		}
	}

	/*  Send close notify  */
	if(wiz_tls_close_notify(&tlsContext) == 0)
		printf("SSL closed\r\n");

	/*  Free tls context  */
	wiz_tls_deinit(&tlsContext);

	while(1)
    {

    }
}
