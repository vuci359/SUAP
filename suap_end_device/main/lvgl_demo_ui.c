/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter

#include "lvgl.h"
//#include "esp_transport_ssl.h"
#include "esp_http_client.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define SERVER_IP "192.168.88.252:5023"

#define API_GET_URL "http://" SERVER_IP "/api/SuapApi"

#define API_POST_URL "http://"SERVER_IP"/api/SuapApi"

char *body_data = {"*"};

esp_err_t client_event_http_handler(esp_http_client_event_handle_t evt)

{
    switch (evt->event_id)
    {
   case HTTP_EVENT_ERROR:
            printf("HTTP_EVENT_ERROR\n");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            printf("HTTP_EVENT_ON_CONNECTED\n");
            break;
        case HTTP_EVENT_HEADER_SENT:
            printf("HTTP_EVENT_HEADER_SENT\n");
            break;
        case HTTP_EVENT_ON_HEADER:
            printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s\n", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            printf("HTTP_EVENT_ON_DATA, len=%d\n", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                printf("\t%.*s\n", evt->data_len, (char*)evt->data);
                body_data = evt->data;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            printf("HTTP_EVENT_ON_FINISH\n");
            break;
        case HTTP_EVENT_DISCONNECTED:
            printf("HTTP_EVENT_DISCONNECTED\n");
            break;
    default:
        break;
    }
    return ESP_OK;
}

//primjer post funkcije
//register_rest_function(CONFIG_REGISTRATION_URL_2, CONFIG_USER_NAME, CONFIG_USER_PASSWORD, "[\""CONFIG_ID_NAZIV"\"]");
void register_rest_function(char* odgovor, const char *URL, int broj)
{
    esp_http_client_config_t config_post = {
        .url = URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .user_data = body_data,
        .event_handler = client_event_http_handler,
        };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char  *post_data[20];


    sprintf(post_data,"%d", broj);
    
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    //tu se puniju headeri
    //esp_http_client_set_header(client, "Authorisation", "Basic aW50c3R2X3NtYXJ0cGFya2luZzpmZlozUElnM2tqc3JoVU1S");
    esp_http_client_set_header(client, "Content-Type", "application/json");
    //esp_http_client_set_header(client, "Cache-Control", "no-cache");
    esp_err_t err = esp_http_client_perform(client);

    //odgovor = "AABBCC";

    if (err == ESP_OK) {
        printf("HTTP POST Status = %d, content_length = %"PRId64"\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        sprintf(odgovor,"Status = %d",
                esp_http_client_get_status_code(client));
        
    } else {
        printf("HTTP POST request failed: %s\n", esp_err_to_name(err));
        sprintf(odgovor,"FAIL");
    }

    esp_http_client_cleanup(client);
   // return status;
}

//primjer get funkcije
//get_rest_function(CONFIG_DATA_URL"?res="CONFIG_ID_NAZIV"&maxPayloadsPerResource=1", "res="CONFIG_ID_NAZIV"&maxPayloadsPerResource=1", CONFIG_USER_NAME, CONFIG_USER_PASSWORD)

char* get_rest_function(char* odgovor,const char *URL, const char *QUERY)
{



    esp_http_client_config_t config_get = {
        .url = URL,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_http_handler,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .user_data = body_data,
        .buffer_size = 2048,
       // .query = QUERY
    };

   // printf("%s\n", config_get.query);
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);

    esp_http_client_set_header(client, "Accept", "application/json");

// GET
    esp_err_t err = esp_http_client_perform(client);
    

    if (err == ESP_OK) {
        printf("HTTP GET Status = %d, content_length = %"PRId64"\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        sprintf(odgovor,"Status = %d",
                esp_http_client_get_status_code(client));
        
    } else {
        printf("HTTP GET request failed: %s\n", esp_err_to_name(err));
        sprintf(odgovor,"FAIL");
    }
    esp_http_client_cleanup(client);

    return body_data;

}


static uint8_t button_counter = 0;
static lv_obj_t * count_label;
lv_obj_t * mbox1;
lv_group_t * g;
lv_group_t * g2;

static void mevent_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    LV_UNUSED(label);
    LV_LOG_USER("Button %s clicked", lv_label_get_text(label));
    lv_obj_del(mbox1);
}


static void event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
    button_counter++;
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);
  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }
}

static void get_event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

   if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
     get_rest_function(API_GET_URL,"000");
    button_counter = 335;
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);
  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }

}

static void post_event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
     char *odgovor[25];
     register_rest_function(odgovor, API_POST_URL, button_counter);
     printf("gecrk\n");
    //  *odgovor = "ccc";
     printf("%s\n",*odgovor);
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);

       // static const char * btns[] ={"Apply", "Close", ""};
   // printf(odgovor);
  // odgovor = "ccccc";
    mbox1 = lv_msgbox_create(lv_layer_top(), "Poslano", odgovor, NULL, true);
    lv_obj_add_event_cb(mbox1, mevent_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
      lv_group_add_obj(g2,mbox1);
      lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
      lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));


  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }
}



void example_lvgl_demo_ui(lv_disp_t *disp, lv_indev_t *encoder1i, lv_indev_t *encoder2i)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    g = lv_group_create();
    g2 = lv_group_create();

    /*Create a menu object*/
    lv_obj_t * menu = lv_menu_create(scr);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * label;


    /*Create a sub page*/
    lv_obj_t * sub_page = lv_menu_page_create(menu, NULL);

  lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, -50, -10);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "Button");
  lv_obj_center(label);

  count_label = lv_label_create(lv_scr_act());
  lv_obj_align(count_label, LV_ALIGN_CENTER, 0, -60);
  lv_label_set_text(count_label, "Counts: 0");

  lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn2, LV_ALIGN_CENTER, -50, 40);
  lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_height(btn2, LV_SIZE_CONTENT);

  label = lv_label_create(btn2);
  lv_label_set_text(label, "Toggle");
  lv_obj_center(label);
  lv_group_add_obj(g, btn1);
  lv_group_add_obj(g,btn2);
  lv_indev_set_group(encoder1i, g);


  lv_obj_t * btn1b = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn1b, post_event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP POST
  lv_obj_align(btn1b, LV_ALIGN_CENTER, 50, -10);

  label = lv_label_create(btn1b);
  lv_label_set_text(label, "POST");
  lv_obj_center(label);

  lv_obj_t * btn2b = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn2b, get_event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP GET
  lv_obj_align(btn2b, LV_ALIGN_CENTER, 50, 40);

  label = lv_label_create(btn2b);
  lv_label_set_text(label, "GET");
  lv_obj_center(label);

  lv_group_add_obj(g2, btn1b);
  lv_group_add_obj(g2,btn2b);
  lv_indev_set_group(encoder2i, g2);
  
  }
