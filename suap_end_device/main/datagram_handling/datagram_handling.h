#pragma once
#ifndef inc_datagram_handling_h
#define inc_datagram_handling_h

#include <string.h>
#include <cJSON.h>
#include "esp_err.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"

//string sizes
#define REQUEST_STRING_SIZE 300
#define REQUEST_BODY_SIZE 100
#define MESSAGE_SIZE 50

//network_type
#define WiFi 0
#define ZigBee 1
#define LoRa 2

//request_type
#define REQUEST 0
#define REPLY 1
#define MESSAGE 2

//device type
#define SENSOR 0
#define ACTUATOR 1
#define USER 2

//parser

//target_id se vadi iz postgres baze na koordinatoru, a dobiva se nakon registracije
//device_id je indentifikator periferije spojene na uređajs
const char *pars = "main request parser";
int parse_datagram(char *json, char *ID, char *network, int *network_type, char *interface, int *targetID, char *body);
int parse_datagram_body(char *json, int *request_type, int *device_type, int *logical_clock, int *device_id, char *data);
int parse_sensor_datagram(char *json);
int parse_actuator_datagram(char *json, int *new_state);
int parse_user_datagram(char *json, char *message, bool *input_required);


//generator
char* generate_datagram(char *ID, char *network, int *network_type, char *interface, int *targetID, char *body);
char* generate_datagram_body(int *request_type, int *device_type, int *logical_clock, int *device_id, char *data);
char* generate_sensor_datagram();
char* generate_actuator_datagram(int *oldstate, int *new_state);
char* generate_user_datagram(char *message, bool *input_required, int *userinput);




#endif