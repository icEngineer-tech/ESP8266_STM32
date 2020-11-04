/*
 * wifi.c
 *
 *  Created on: 16 Oct 2020
 *      Author: idriss
 */
/*
 * to use this driver please follow the following steps:
 * 1/ to activate the SoftAP Mode:
 *
 */
#include "wifi.h"
#define huart huart3

uint8_t mode;

bool WiFi_reset()
{
	if(ESP_RESET(huart))
		return TRUE;
	else
		return FALSE;
}

bool WiFi_Mode(unsigned type, uint8_t mode)
{
	if (type>1 || type<0)
	{
#error not matching type
		return FALSE;
	}
	switch(type)
	{
	case 0:
		SET_WIFI_MODE_NOT_SAVED(UART_HandleTypeDef* huart, &mode);
		break;
	case 1:
		SET_WIFI_MODE_SAVED(UART_HandleTypeDef* huart, &mode);
		break;
	}
	return TRUE;
}
bool WiFi_Configure_Hostname_Station(char* hostname)
{
	bool res = TRUE;
	mode==1?ESP_CONFIGURE_NAME_HOST_STATION(huart ,hostname):res=FALSE;
	return res;
}

ESPWIFI_State Wifi_Send_Data(unsigned length, const char* data, unsigned short id=0)
{
	return ESP_SEND_DATA(huart, length, data, id);
}
