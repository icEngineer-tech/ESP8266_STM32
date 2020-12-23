/*
 * espwifi.h
 *
 *  Created on: 16 Oct 2020
 *      Author: idriss
 */

#ifndef INC_ESPWIFI_H_
#define INC_ESPWIFI_H_
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	ESPWIFI_OK,
	ESPWIFI_ERROR,
	ESPWIFI_BUSY,
	ESPWIFI_FAIL
}ESPWIFI_State;

typedef enum
{
	ESPWIFI_4800_BR = 4800,
	ESPWIFI_9600_BR = 9600,
	ESPWIFI_14400_BR = 14400,
	ESPWIFI_19200_BR = 19200,
	ESPWIFI_38400_BR = 38400,
	ESPWIFI_57600_BR = 57600,
	ESPWIFI_115200_BR = 115200,
	ESPWIFI_230400_BR = 230400,
	ESPWIFI_460800_BR = 460800

}ESPWIFI_BAUDRATE;

typedef enum
{
	ESPWIFI_5bit = 5,
	ESPWIFI_6bit,
	ESPWIFI_7bit,
	ESPWIFI_8bit
}ESPWIFI_DATABIT;

typedef enum
{
	ESPWIFI_1bit = 1,
	ESPWIFI_2bit,
	ESPWIFI_3bit
}ESPWIFI_STOPB;

typedef enum
{
	ESPWIFI_NONE,
	ESPWIFI_ODD,
	ESPWIFI_EVEN
}ESPWIFI_PARITY;

typedef enum
{
	ESP_OK,
	ESP_CONNECTION_TIMEOUT,
	ESP_WRONG_PASS,
	ESP_AP_NOT_EXIST,
	ESP_CONNECTION_FAILED
}ESPWIFI_ERRORC_t;
typedef struct
{
	ESPWIFI_ERRORC_t error;
	ESPWIFI_State state;
}ESP_CONNECTION;

typedef enum
{
	FALSE,
	TRUE
} bool;

typedef enum
{
	ESP_DISABLE_DHCP,
	ESP_ENABLE_DHCP
}ESP_DHCP;

typedef enum
{
	ESP_SoftAP,
	ESP_Station,
	ESP_SoftAP_Station
}ESP_AP;

typedef enum
{
	ESP_DISABLE_AUTO_CONNECT,
	ESP_ENABLE_AUTO_CONNECT
}AUTO_COONECT;

typedef enum
{
	ESP_TOUCH=1,
	ESP_AIRKISS,
	ESP_BOTH
}ESP_SMR_CONFIG;

/*
 * Basic AT Commands
 */
ESPWIFI_State ESP_RESET(UART_HandleTypeDef*);
ESPWIFI_State ESP_UART_CONFIG(UART_HandleTypeDef* huart,uint8_t BR, uint8_t DB, uint8_t SB,
		uint8_t p, uint8_t CF=0);
ESPWIFI_State ESP_SLEEP_MODE(UART_HandleTypeDef*, uint8_t);
char* ESP_REMAING_SPACE_RAM(UART_HandleTypeDef*);
/*
 * Wi-Fi AT Commands
 */
ESPWIFI_State SET_WIFI_MODE_NOT_SAVED(UART_HandleTypeDef*, uint8_t*);
ESPWIFI_State SET_WIFI_MODE_SAVED(UART_HandleTypeDef*, uint8_t*);
ESP_CONNECTION CONNECT_TO_AP_NOT_SAVED(UART_HandleTypeDef*, char*, char*,
		char* BSSID=NULL, char* PCI_EN=NULL);
ESP_CONNECTION CONNECT_TO_AP_SAVED(UART_HandleTypeDef*, char*, char*,
		char* BSSID=NULL, char* PCI_EN=NULL);

ESPWIFI_State ESP_DISCONNECT(UART_HandleTypeDef*);
bool ESP_CONFIGURE_SOFTAP_NOT_SAVED(UART_HandleTypeDef*,char*, char*,
		uint8_t channelID=8, uint8_t ecn=3);
ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_NOT_SAVED(UART_HandleTypeDef*, ESP_AP, ESP_DHCP);
ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_SAVED(UART_HandleTypeDef*, ESP_AP, ESP_DHCP);
ESPWIFI_State ESP_AUTO_CONNECT(UART_HandleTypeDef*, AUTO_COONECT);
ESPWIFI_State ESP_SMART_CONFIG(UART_HandleTypeDef*, ESP_SMR_CONFIG);
ESPWIFI_State ESP_STOP_SMART_CONFIG(UART_HandleTypeDef*, ESP_SMR_CONFIG);
ESPWIFI_State ESP_CONFIGURE_NAME_HOST_STATION(UART_HandleTypeDef*, char*);
/*
 * TCP-IP AT Commands
 */
ESPWIFI_State ESP_SEND_DATA(UART_HandleTypeDef* ,unsigned, const char*);
		//unsigned short id=0);
ESPWIFI_State Get_The_Local_IP_ADDR(UART_HandleTypeDef*);
__weak ESPWIFI_State En_Dis_Multiple_Conn(UART_HandleTypeDef*, bool);
ESPWIFI_State Create_Del_TCP_Server(UART_HandleTypeDef*, bool, unsigned short port=333);
ESPWIFI_State Set_Max_Conn_Allowed_by_Server(UART_HandleTypeDef* , bool,
		unsigned short num=1, unsigned short port=333);
#endif /* INC_ESPWIFI_H_ */
