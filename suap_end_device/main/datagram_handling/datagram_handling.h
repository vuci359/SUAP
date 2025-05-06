#pragma once
#ifndef inc_datagram_handling_h
#define inc_datagram_handling_h

#include <string.h>
#include <cJSON.h>
#include "esp_err.h"
#include <math.h>
#include "esp_wifi.h"


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "networking/http_handling.h"
#include "global_variables.h"


//string sizes
#define REQUEST_STRING_SIZE 300
#define REQUEST_BODY_SIZE 200
#define REQUEST_DATA_SIZE 100

#define MESSAGE_SIZE 50


//parser

//target_id se vadi iz postgres baze na koordinatoru, a dobiva se nakon registracije
//device_id je indentifikator periferije spojene na uređajs
int parse_datagram(cJSON **json, char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID);
int parse_datagram_body(cJSON **json, int *request_type, int *logical_clock, int *device_id);
int parse_sensor_datagram(cJSON **json, int *measurement, char *unit);
int parse_actuator_datagram(cJSON **json, int *old_state,int *new_state);
int parse_user_datagram(cJSON **json, char *message, bool *input_required, int *user_input);

//generator
const char* generate_datagram(char *body, char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID);
const char* generate_datagram_body(char *data, int request_type, int *logical_clock, int *device_id);
const char* generate_sensor_datagram(int *measurement, char* unit);
const char* generate_actuator_datagram(int *oldstate, int *new_state);
const char* generate_user_datagram(char *message, bool *input_required, int *userinput);

//main_request_parser
int read_data_from_sensor(int *sensor_id, int *value, char *unit);
int set_actuator_state(int *actuator_id, int *new_state);
int get_actuator_state(int *actuator_id, int *new_state);

int display_message_to_user(char *message, bool *input_required, int *answer);

int parse_request(char *request);

#endif