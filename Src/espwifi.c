/*
 * espwifi.c
 *
 *  Created on: 16 Oct 2020
 *      Author: idriss
 */

/*
 * includes
 */
#include "espwifi.h"




/*
 * Basic AT Commands
 */
ESPWIFI_State ESP_RESET(UART_HandleTypeDef* huart)
{
	char RxBuff[3];
	HAL_UART_Transmit(huart, (uint8_t*)"AT+RST", 6, 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizef(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_UART_CONFIG(UART_HandleTypeDef* huart,uint8_t BR, uint8_t DB, uint8_t SB,
		uint8_t p, uint8_t CF=0)
{
	char TxBuff[27]="AT+UART_CUR=";
	char RxBuff[3];
	char temp[14];
	sprintf(temp,"%i,%i,%i,%i,%i",BR,BD,SB,p,CF);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_SLEEP_MODE(UART_HandleTypeDef* huart, uint8_t sm)
{
	char TxBuff[11]="AT+SLEEP=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",sm);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}

char* ESP_REMAING_SPACE_RAM(UART_HandleTypeDef* huart)
{
	char RxBuff[14];
	HAL_USART_TransmitReceive(huart, (uint8_t*)"AT+SYSRAM?", 11, 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return RxBuff;
}


/*
 * Wi-Fi AT Commands
 */

ESPWIFI_State SET_WIFI_MODE_NOT_SAVED(UART_HandleTypeDef* huart, uint8_t* curm)
{
	if(*curm<1 || *curm>3)
		return ESPWIFI_ERROR;
	char TxBuff[20]="AT+CWMODE_CUR=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",*curm);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}
ESPWIFI_State SET_WIFI_MODE_SAVED(UART_HandleTypeDef* huart, uint8_t* mode)
{
	if(*mode<1 || *mode>3)
			return ESPWIFI_ERROR;
	char TxBuff[18]="AT+CWMODE_DEF=";
	char RxBuff[3];
	char temp[2];
	sprintf(temp,"%i",*mode);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff,  sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}

ESP_CONNECTION CONNECT_TO_AP_NOT_SAVED(UART_HandleTypeDef* huart, char* SSID, char* PASSWORD,
		char* BSSID=NULL, char* PCI_EN=NULL)
{
	char TxBuff[11]="AT+CWJAP_CUR=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%s,%s",SSID,PASSWORD,BSSID,PCI_EN);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
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
ESP_CONNECTION CONNECT_TO_AP_SAVED(UART_HandleTypeDef* huart, char* SSID, char* PASSWORD,
		char* BSSID=NULL, char* PCI_EN=NULL)
{
	char TxBuff[11]="AT+CWJAP_DEF=";
	char RxBuff[3];
	char temp[80];
	sprintf(temp,"%s,%s,%s,%s",SSID,PASSWORD,BSSID,PCI_EN);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, (uint8_t*)RxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
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


ESPWIFI_State ESP_DISCONNECT(UART_HandleTypeDef* huart)
{
	char RxBuff[3];
	HAL_UART_Transmit(huart, (uint8_t*)"AT+CWQAP", 8, 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart,  (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))?ESPWIFI_OK:ESPWIFI_ERROR;
}

bool ESP_CONFIGURE_SOFTAP_NOT_SAVED(UART_HandleTypeDef* huart,char* SSID, char* PASS,
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
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? TRUE:FALSE;
}
bool ESP_CONFIGURE_SOFTAP_SAVED(UART_HandleTypeDef* huart,char* SSID, char* PASS,
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
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? TRUE:FALSE;
}

ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_NOT_SAVED(UART_HandleTypeDef* huart, ESP_AP mode, ESP_DHCP en)
{
	char TxBuff[20]="AT+CWDHCP_CUR=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i,%i",mode,en);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart,(uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_ENABLE_DISABLE_DHCP_SAVED(UART_HandleTypeDef* huart, ESP_AP mode, ESP_DHCP en)
{
	char TxBuff[20]="AT+CWDHCP_DEF=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i,%i",mode,en);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_AUTO_CONNECT(UART_HandleTypeDef* huart, AUTO_COONECT conf)
{
	char TxBuff[17]="AT+CWAUTOCONN=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i",conf);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_SMART_CONFIG(UART_HandleTypeDef* huart, ESP_SMR_CONFIG smart)
{
	char TxBuff[20]="AT+CWSTARTSMART=";
	char RxBuff[3];
	char temp[3];
	sprintf(temp,"%i",smart);
	strcat(TxBuff,temp);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_STOP_SMART_CONFIG(UART_HandleTypeDef* huart, ESP_SMR_CONFIG smart)
{
	char RxBuff[3];
	HAL_UART_Transmit(huart, (uint8_t*)"AT+CWSTOPSMART=", 16, 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}

ESPWIFI_State ESP_CONFIGURE_NAME_HOST_STATION(UART_HandleTypeDef* huart, char* hostname)
{
	const char* Command="AT+CWHOSTNAME=";
	char RxBuff[3];
	char* TxBuff = (char*)malloc(sizeof(char)*(strlen(hostname)+strlen(Command)));
	strcpy(TxBuff,Command);
	strcat(TxBuff,hostname);
	HAL_UART_Transmit(huart, (uint8_t*)TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(100);
	HAL_UART_Receive(huart,(uint8_t*)RxBuff, sizeof(RxBuff), 100);
	free(TxBuff);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}
/*
 * TCP-IP AT Commands
 */

/*
 * the best call of this function to use the sizeof data as length
 */
ESPWIFI_State ESP_SEND_DATA(UART_HandleTypeDef* huart,unsigned length, const char* data)
		//unsigned short id=0)
{
	const char* TxBuff="AT+CIPSEND=";
	//if(lentgh>2048)
	//	return ESPWIFI_ERROR;


	char* temp;
	sprintf(temp,"%i",lenght);
	strcat(TxBuff,temp);
	free(temp);
	/*else
	{
		char temp[4];
		sprintf(temp,"%i,%i",id, lenght);
		strcat(TxBuff,temp);
	}*/
	char RxBuff[8];
	HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(50);
	HAL_UART_Transmit(huart, (uint8_t*)data, sizeof(data), 200);
	HAL_Delay(100);
	if(len<strlen(data)*sizeof(char))
		return ESPWIFI_BUSY;
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_FAIL;
}

ESPWIFI_State Get_The_Local_IP_ADDR(UART_HandleTypeDef* huart)
{
	const char* TxBuff="AT+CIFSR";
	char RxBuff[100];
	HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(50);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	HAL_Delay(100);
}

/*
 * params: mode=0 --> single connection
 * 		   mode=1 --> multiple connection
 * return: ESP_OK if the query has been executed successfully
 */
__weak ESPWIFI_State En_Dis_Multiple_Conn(UART_HandleTypeDef* huart, bool mode)
{
	const char* TxBuff= mode? "AT+CIPMUX=1":"AT+CIPMUX=0";
	char RxBuff[3];
	HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(50);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	HAL_Delay(100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}
ESPWIFI_State Create_Del_TCP_Server(UART_HandleTypeDef* huart, bool mode, unsigned short port=333)
{
	const char* TxBuff= mode? "AT+CIPSERVER=1":"AT+CIPSERVER=0";
	char* temp;
	char RxBuff[3];
	sprintf(temp,"%i",port);
	strcat(TxBuff, temp);
	free(temp);
	HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
	HAL_Delay(50);
	HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
	HAL_Delay(100);
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}
/*
 * you should call this function before calling Create_Del_TCP_Server() function
 */
ESPWIFI_State Set_Max_Conn_Allowed_by_Server(UART_HandleTypeDef* huart, bool mode,\
		unsigned short num=1, unsigned short port=333)
{
	if(En_Dis_Multiple_Conn(huart,mode) != ESP_OK)
		return ESP_CONNECTION_FAILED;
	if(num<1 || num>5)
#error please verify the number of connection
		return ESP_CONNECTION_FAILED;
	if(mode==FALSE)
	{
		const char* TxBuff= "AT+CIPSERVERMAXCONN=1";
		char RxBuff[3];
		HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
		HAL_Delay(50);
		HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
		HAL_Delay(100);
	}
	else
	{
		const char* TxBuff= "AT+CIPSERVERMAXCONN=";
		char RxBuff[3];
		sprintf(temp,"%i",num);
		strcat(TxBuff,temp);
		free(temp);
		HAL_UART_Transmit(huart, TxBuff, sizeof(TxBuff), 100);
		HAL_Delay(50);
		HAL_UART_Receive(huart, (uint8_t*)RxBuff, sizeof(RxBuff), 100);
		HAL_Delay(100);
	}
	return (!strcmp(RxBuff,"OK"))? ESPWIFI_OK:ESPWIFI_ERROR;
}
