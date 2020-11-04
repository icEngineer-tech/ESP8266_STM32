/*
 * wifi.h
 *
 *  Created on: 16 Oct 2020
 *      Author: idriss
 */

#ifndef INC_WIFI_H_
#define INC_WIFI_H_
#include "espwifi.h"

bool WiFi_reset();
bool WiFi_Mode(unsigned, uint8_t);
bool WiFi_Configure_Hostname_Station(char*);
ESPWIFI_State Wifi_Send_Data(unsigned, const char*, unsigned short id=0);

#endif /* INC_WIFI_H_ */
