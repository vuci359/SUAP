#ifndef _include_global_variables_h
#define _include_global_variables_h

#include "lvgl.h"
#include "driver/gpio.h"

//network_type
#define WiFi 0
#define ZigBee 1
#define LoRa 2

//request_type
#define REQUEST 0
#define REPLY 1
#define MESSAGE 2

//device type
#define IOT_SENZOR 0
#define ACTUATOR 1
#define USER 2


//senzori
#define WLAN0 0

//aktuatori
#define BACKLIGHT0 1

//user
#define USER0 2

#define EXAMPLE_PIN_NUM_BK_LIGHT       4
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

//varijable koje su kritične za razmjenu veđu funkcijama
extern lv_disp_t *disp; //samo spominjem da postoji negdje u memoriji

#define MAX_HTTP_RECV_BUFFER 1024

#define SERVER_IP "10.200.2.194:5023"

#define BASE_URL "http://"SERVER_IP"/api"

#define API_GET_URL BASE_URL"/SuapApi"

#define API_POST_URL BASE_URL"/SuapApi"

#define PERIPHERAL_DEVICES "[{\"device_type\": 0, \"device_id\": 0}, {\"device_type\": 1, \"device_id\": 1}, {\"device_type\": 2, \"device_id\": 2}]"
//dopisati par requestova

extern lv_obj_t * mbox1;
extern lv_obj_t * mbox2;

extern lv_group_t * g;
extern lv_group_t * g2;

extern int my_ID;
extern int neigbour_IDs[100];
extern int neigbour_count;

typedef struct {
    int device_type;
    int device_id;
} attached_device;

typedef struct{
    int ID;
    char *IP;
    char *MAC;
    attached_device *devices; //pretpostavljam do 5 perifernih uređaja
} end_device;

extern int my_ID;
extern int neighbour_count;
extern end_device neighbours[100];

extern int current_logical_clock;
static const char *pars = "main request parser"; //nek si kompajler napravi zasebnu kopiju za svaku funkciju da me ne maltretira...


#endif