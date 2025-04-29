#pragma once
#ifndef inc_tasks_h_ccc
#define inc_tasks_h_ccc
#include "networking/http_handling.h"

#include "global_variables.h"
#include "esp_wifi.h"
#include "esp_mac.h"

#include <cJSON.h>
#include "freertos/task.h"


extern const char *predef_tasks;



extern int register_end_device();
extern int get_neighbours();
void find_neighbours();

#endif