#pragma once
#ifndef inc_request_parser_h
#define inc_request_parser_h

#include <string.h>
#include <cJSON.h>
#include "esp_err.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"


//network_type
#define WiFi 0
#define ZigBee 1
#define LoRa 2

//request_type
#define REQUEST 0
#define REPLY 1
#define MESSAGE 2

//target_id se vadi iz postgres baze na koordinatoru, a dobiva se nakon registracije
const char *pars = "main request parser";
static int extract_relevant_data(char *json, char *ID, char *network, int *network_type, char *interface, int *targetID, int *request_type, char *body);


#endif