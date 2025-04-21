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

#define BASE_URL "http://"SERVER_IP"/api"

#define API_GET_URL BASE_URL"/SuapApi"

#define API_POST_URL BASE_URL"/SuapApi"

#define PERIPHERAL_DEVICES "sfgsr" //dovršiti...

//dopisati par requestova

extern lv_obj_t * mbox1;
extern lv_obj_t * mbox2;

extern lv_group_t * g;
extern lv_group_t * g2;

int my_ID;
int neigbour_IDs[100];
int neigbour_count = 0;

typedef struct {
    int dev_type;
    int dev_id;
} attached_device;

typedef struct{
    int ID;
    char MAC[30];
    attached_device devices[5]; //pretpostavljam do 5 perifernih uređaja
} end_device;



#endif