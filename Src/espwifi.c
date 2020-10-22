/*
 * espwifi.c
 *
 *  Created on: 16 Oct 2020
 *      Author: idriss
 */

#include "espwifi.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

UART_HandleTypeDef huart;

/*
 * Basic AT Commands
 */
ESPWIFI_State ESP_RESET(UART_HandleTypeDef huart)
{
	char rBuff[3];
	HAL_USART_TransmitReceive(huart, (uint8_t*)"AT+RST",(uint8_t*)rBuff, 6, 200);
	if(!strcmp(rBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}

ESPWIFI_State ESP_UART_CONFIG(UART_HandleTypeDef huart,uint8_t BR, uint8_t DB, uint8_t SB,
		uint8_t p, uint8_t CF=0)
{
	char TxBuff[27]="AT+UART_CUR=";
	char RxBuff[3];
	char temp[14];
	sprintf(temp,"%i,%i,%i,%i,%i",BR,BD,SB,p,CF);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, (uint8_t*)TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}

ESPWIFI_State ESP_SLEEP_MODE(UART_HandleTypeDef huart, uint8_t sm)
{
	char TxBuff[11]="AT+SLEEP=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",sm);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}

char* ESP_REMAING_SPACE_RAM(UART_HandleTypeDef huart)
{
	char RxBuff[14];
	HAL_USART_TransmitReceive(huart, (uint8_t*)"AT+SYSRAM?", RxBuff, 11, 200);
	return RxBuff;
}


/*
 * Wi-Fi AT Commands
 */

ESPWIFI_State SET_WIFI_MODE_NOT_SAVED(UART_HandleTypeDef huart, uint8_t curm)
{
	if(curm<1 || curm>3)
		return ESPWIFI_ERROR;
	char TxBuff[11]="AT+CWMODE_CUR=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",curm);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}
ESPWIFI_State SET_WIFI_MODE_SAVED(UART_HandleTypeDef huart, uint8_t mode)
{
	if(mode<1 || mode>3)
			return ESPWIFI_ERROR;
	char TxBuff[11]="AT+CWMODE_DEF=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",curm);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}

ESP_CONNECTION CONNECT_TO_AP_NOT_SAVED(UART_HandleTypeDef huart, char* SSID, char* PASSWORD,
		char* BSSID=NULL, char* PCI_EN=NULL)
{
	char TxBuff[11]="AT+CWJAP_CUR=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%s,%s",SSID,PASSWORD,BSSID,PCI_EN);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return (ESP_OK,ESPWIFI_OK);
	else
	{
		uint8_t err = atoi(RxBuff[11]);
		switch(err)
		{
		case 1:
			return (ESP_CONNECTION_TIMEOUT, ESPWIFI_ERROR);
		case 2:
			return (ESP_WRONG_PASS, ESPWIFI_ERROR);
		case 3:
			return (ESP_AP_NOT_EXIST, ESPWIFI_ERROR);
		case 4:
			return (ESP_CONNECTION_FAILED, ESPWIFI_ERROR);
		default:
			return -1;
		}
	}
}
ESP_CONNECTION CONNECT_TO_AP_SAVED(UART_HandleTypeDef huart, char* SSID, char* PASSWORD,
		char* BSSID=NULL, char* PCI_EN=NULL)
{
	char TxBuff[11]="AT+CWJAP_DEF=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%s,%s",SSID,PASSWORD,BSSID,PCI_EN);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return (ESP_OK,ESPWIFI_OK);
	else
	{
		uint8_t err = atoi(RxBuff[11]);
		switch(err)
		{
		case 1:
			return (ESP_CONNECTION_TIMEOUT, ESPWIFI_ERROR);
		case 2:
			return (ESP_WRONG_PASS, ESPWIFI_ERROR);
		case 3:
			return (ESP_AP_NOT_EXIST, ESPWIFI_ERROR);
		case 4:
			return (ESP_CONNECTION_FAILED, ESPWIFI_ERROR);
		default:
			return -1;
		}
	}
}


ESPWIFI_State ESP_DISCONNECT(UART_HandleTypeDef huart)
{
	char RxBuff[3];
	HAL_USART_TransmitReceive(huart, (uint8_t*)"AT+CWQAP", RxBuff, 8, 200);
	if(!strcmp(RxBuff,"OK"))
		return ESPWIFI_OK;
	else
		return ESPWIFI_ERROR;
}

bool ESP_CONFIGURE_SOFTAP_NOT_SAVED(UART_HandleTypeDef huart,char* SSID, char* PASS,
		uint8_t channelID=8, uint8_t ecn=3)
{
	if(channelID>14 || channelID<1)
		return FALSE;
	if(ecn>4 || ecn<0)
		return FALSE;
	char TxBuff[11]="AT+CWJAP_CUR=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%i,%i",SSID,PASSWORD,channelID,ecn);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return TRUE;
	else
		return FALSE;
}
bool ESP_CONFIGURE_SOFTAP_SAVED(UART_HandleTypeDef huart,char* SSID, char* PASS,
		uint8_t channelID=8, uint8_t ecn=3)
{
	if(channelID>14 || channelID<1)
		return FALSE;
	if(ecn>4 || ecn<0)
		return FALSE;
	char TxBuff[11]="AT+CWJAP_DEF=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%i,%i",SSID,PASSWORD,channelID,ecn);
	HAL_USART_TransmitReceive(huart, TxBuff, RxBuff, sizeof(TxBuff), 200);
	if(!strcmp(RxBuff,"OK"))
		return TRUE;
	else
		return FALSE;
}

ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_NOT_SAVED(UART_HandleTypeDef* huart, ESP_AP mode, ESP_DHCP en)
{
	char TxBuff[11]="AT+CWDHCP_CUR=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i,%i\0",mode,en);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, Size, 200);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_SAVED(UART_HandleTypeDef* huart, ESP_AP mode, ESP_DHCP en)
{
	char TxBuff[11]="AT+CWDHCP_DEF=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i,%i\0",mode,en);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, sizeof(TxBuff), 200);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_AUTO_CONNECT(UART_HandleTypeDef* huart, AUTO_COONECT conf)
{
	char TxBuff[17]="AT+CWAUTOCONN=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i\0",conf);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, sizeof(TxBuff), 200);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_SMART_CONFIG(UART_HandleTypeDef* huart, ESP_SMR_CONFIG smart)
{
	char TxBuff[20]="AT+CWSTARTSMART=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i\0",smart);
	strcat(TxBuff,temp);
	HAL_USART_TransmitReceive(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, sizeof(TxBuff), 200);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_STOP_SMART_CONFIG(UART_HandleTypeDef* huart, ESP_SMR_CONFIG smart)
{
	char RxBuff[3];
	HAL_USART_TransmitReceive(huart, (uint8_t*)"AT+CWSTOPSMART=", (uint8_t*)RxBuff, 16, 200);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

/*
 * TCP-IP AT Commands
 */
