/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter

#include "lvgl.h"
#include "esp_transport_ssl.h"
#include "esp_http_client.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

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
void register_rest_function(const char *URL, const char *USERNAME, const char *PASSWORD, char *body_string)
{
    esp_http_client_config_t config_post = {
        .url = URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = client_event_http_handler,
        .auth_type = HTTP_AUTH_TYPE_BASIC,
        .username = USERNAME,
        .password = PASSWORD
        };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char  *post_data = body_string;
    
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    //tu se puniju headeri
    //esp_http_client_set_header(client, "Authorisation", "Basic aW50c3R2X3NtYXJ0cGFya2luZzpmZlozUElnM2tqc3JoVU1S");
    esp_http_client_set_header(client, "Content-Type", "application/json");
    //esp_http_client_set_header(client, "Cache-Control", "no-cache");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

//primjer get funkcije
//get_rest_function(CONFIG_DATA_URL"?res="CONFIG_ID_NAZIV"&maxPayloadsPerResource=1", "res="CONFIG_ID_NAZIV"&maxPayloadsPerResource=1", CONFIG_USER_NAME, CONFIG_USER_PASSWORD)

char* get_rest_function(const char *URL, const char *QUERY, const char *USERNAME, const char *PASSWORD)
{



    esp_http_client_config_t config_get = {
        .url = URL,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_http_handler,
        .auth_type = HTTP_AUTH_TYPE_BASIC,
        .username = USERNAME,
        .password = PASSWORD,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .user_data = body_data,
        .buffer_size = 2048,
       // .query = QUERY
    };

   // printf("%s\n", config_get.query);
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);

    esp_http_client_set_header(client, "Accept", "application/vnd.ericsson.m2m.output+json;version=1.0");

// GET
    esp_err_t err = esp_http_client_perform(client);
    

    if (err == ESP_OK) {
        printf( "HTTP GET Status = %d, content_length = %"PRId64"\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        printf("HTTP GET request failed: %s\n", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);

    return body_data;

}


//#include "lv_menu.h"

//#include "../../lv_examples.h"
//#if LV_USE_MENU && LV_USE_MSGBOX && LV_BUILD_EXAMPLES

static uint8_t button_counter = 0;
static lv_obj_t * count_label;

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

void example_lvgl_demo_ui(lv_disp_t *disp, lv_indev_t *encoder1i, lv_indev_t *encoder2i)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_group_t * g = lv_group_create();
    lv_group_t * g2 = lv_group_create();

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
  lv_obj_add_event_cb(btn1b, event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP POST
  lv_obj_align(btn1b, LV_ALIGN_CENTER, 50, -10);

  label = lv_label_create(btn1b);
  lv_label_set_text(label, "POST");
  lv_obj_center(label);

  lv_obj_t * btn2b = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn2b, event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP GET
  lv_obj_align(btn2b, LV_ALIGN_CENTER, 50, 40);

  label = lv_label_create(btn2b);
  lv_label_set_text(label, "GET");
  lv_obj_center(label);

  lv_group_add_obj(g2, btn1b);
  lv_group_add_obj(g2,btn2b);
  lv_indev_set_group(encoder2i, g2);
  
  }
