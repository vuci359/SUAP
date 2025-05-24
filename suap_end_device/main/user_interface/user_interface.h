#ifndef __user_interface_h_ccc
#define __user_interface_h_ccc


#include "lvgl.h"
#include "esp_transport_ssl.h"
#include "esp_http_client.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <cJSON.h>
#include "networking/http_handling.h"

#include "global_variables.h"
#include "drivers/display.h"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define ZADNJA_VRIJEDONST "zadnja_vrijednost"
#define BROJAC "brojac"
#define PROSJEK "prosjek"


static char *bdy_data = {"*"};
static int zadnja_vrijednost = 0;
static int brojac = 0;
static double prosjek = 0;





void example_lvgl_demo_ui(lv_indev_t *encoder1, lv_indev_t *encoder2);

int add_button_without_slider(int index, char* label);




#endif