#pragma once
#ifndef inc_tasks_h_ccc
#define inc_tasks_h_ccc
#include "networking/http_handling.h"

#include "global_variables.h"
#include "esp_wifi.h"
#include "esp_mac.h"

#include <cJSON.h>


extern const char *predef_tasks;



int register_end_device();
int get_neigbours();

#endif