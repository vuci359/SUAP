#ifndef _include_global_variables_h
#define _include_global_variables_h

#include "lvgl.h"
#include "driver/gpio.h"

#define EXAMPLE_PIN_NUM_BK_LIGHT       4
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

//varijable koje su kritične za razmjenu veđu funkcijama
extern lv_disp_t *disp; //samo spominjem da postoji negdje u memoriji

#define MAX_HTTP_RECV_BUFFER 512

#define SERVER_IP "10.200.2.194:5023"

#define API_GET_URL "http://"SERVER_IP"/api/SuapApi"

#define API_POST_URL "http://"SERVER_IP"/api/SuapApi"

extern lv_obj_t * mbox1;
extern lv_obj_t * mbox2;

extern lv_group_t * g;
extern lv_group_t * g2;


#endif