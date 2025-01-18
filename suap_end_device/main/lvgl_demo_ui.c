/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter

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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_HTTP_RECV_BUFFER 512

#define SERVER_IP "192.168.88.252:5023"

#define API_GET_URL "http://"SERVER_IP"/api/SuapApi"

#define API_POST_URL "http://"SERVER_IP"/api/SuapApi"

#define ZADNJA_VRIJEDONST "zadnja_vrijednost"
#define BROJAC "brojac"
#define PROSJEK "prosjek"

char *TAG = "SUAP";

char responseBuffer[MAX_HTTP_RECV_BUFFER];

char *body_data = {"*"};
static int zadnja_vrijednost = 0;
static int brojac = 0;
static double prosjek = 0;

static int extract_relevant_data(char *json){

    const cJSON *content_nodes = NULL;
    const cJSON *content_node = NULL;
    const cJSON *zadnja = NULL;
    const cJSON *br = NULL;
    const cJSON *pr = NULL;

    cJSON *json_data = cJSON_Parse(json);
    if(json_data == NULL){
        printf("JSON data IS NULL\n");
        return -1;
    }
    
        zadnja = cJSON_GetObjectItemCaseSensitive(json_data, ZADNJA_VRIJEDONST);
        if (!cJSON_IsNumber(zadnja)){
            return -5;
        }
        //provjera dali su poslani podaci za dobar uređaj
        br = cJSON_GetObjectItemCaseSensitive(json_data, BROJAC);
        if (!cJSON_IsNumber(br)){
            return -5;
        }
        pr = cJSON_GetObjectItemCaseSensitive(json_data, PROSJEK);
        if (!cJSON_IsNumber(pr)){
            return -5;
        }

        zadnja_vrijednost = zadnja->valueint;
        brojac = br->valueint;
        prosjek = pr->valuedouble;
        printf("%d %d %f\n", zadnja_vrijednost, brojac, prosjek);
    

    cJSON_Delete(json_data);
    return 0;
}

esp_err_t client_event_http_handler(esp_http_client_event_handle_t evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // Clean the buffer in case of a new request
            if (output_len == 0 && evt->user_data) {
                // we are just starting to copy the output data into the use
                memset(evt->user_data, 0, MAX_HTTP_RECV_BUFFER);
            }
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (true/*!esp_http_client_is_chunked_response(evt->client)*/) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    // The last byte in evt->user_data is kept for the NULL character in case of out-of-bound access.
                    copy_len = MIN(evt->data_len, (MAX_HTTP_RECV_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    int content_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        // We initialize output_buffer with 0 because it is used by strlen() and similar functions therefore should be null terminated.
                        output_buffer = (char *) calloc(content_len + 1, sizeof(char));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (content_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }else
            {
                ESP_LOGI(TAG, "Data chunk: %s; len = %d", (char *) evt->data, evt->data_len);   // HERE!
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}

static void http_download_chunk(void)
{
    esp_http_client_config_t config = {
        .url = API_GET_URL,
        .event_handler = client_event_http_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP chunk encoding Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}
//primjer post funkcije
//register_rest_function(CONFIG_REGISTRATION_URL_2, CONFIG_USER_NAME, CONFIG_USER_PASSWORD, "[\""CONFIG_ID_NAZIV"\"]");
static void register_rest_function(char* odgovor, const char *URL, int broj)
{
    esp_http_client_config_t config_post = {
        .url = URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        //.user_data = body_data,
        .event_handler = client_event_http_handler,
        .buffer_size = MAX_HTTP_RECV_BUFFER
        };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char  *post_data[20];


    sprintf(post_data,"%d", broj);
    
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    //tu se puniju headeri
    //esp_http_client_set_header(client, "Authorisation", "Basic aW50c3R2X3NtYXJ0cGFya2luZzpmZlozUElnM2tqc3JoVU1S");
    esp_http_client_set_header(client, "Content-Type", "application/json; charset=utf-8");
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

static void get_rest_function(char * return_data, char* odgovor, const char *URL, const char *QUERY)
{
  esp_http_client_config_t config_get = {
        .url = URL,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_http_handler,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .user_data = responseBuffer,
        //.disable_auto_redirect = true,
        .buffer_size = MAX_HTTP_RECV_BUFFER

      //  .buffer_size = 2048,
       // .query = QUERY
    };

   // printf("%s\n", config_get.query);
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);

    esp_http_client_set_header(client, "Accept", "application/json");

// GET
    esp_err_t err = esp_http_client_perform(client);
   // esp_err_t err2 = esp_http_client_read(client, body_data, MAX_HTTP_RECV_BUFFER);


    if (err == ESP_OK) {
        printf("HTTP GET Status = %d, content_length = %"PRId64"\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        sprintf(odgovor,"Status = %d",
                esp_http_client_get_status_code(client));
        
    } else {
        printf("HTTP GET request failed: %s\n", esp_err_to_name(err));
        sprintf(odgovor,"FAIL");
    }   // printf("%s\n", return_data);

    esp_http_client_cleanup(client);
        printf("%s\n", responseBuffer);
    strcpy(return_data, responseBuffer); 
   // return_data = body_data;
   // printf("%s\n", return_data);
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
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT)
      LV_LOG_USER("Press and Hold Event");
     char *odgovor[25];
     char *povrat[50];

     get_rest_function(povrat, odgovor, API_GET_URL, NULL);
     extract_relevant_data(povrat);
    //  *odgovor = "ccc";
    sprintf(odgovor,"Vrijednost: %d\n Brojac: %d\n Prosjek: %f", zadnja_vrijednost, brojac, prosjek);
   //button_counter = button_counter + zadnja_vrijednost;
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);

    printf("%d %d %f\n", zadnja_vrijednost, brojac, prosjek);
    mbox1 = lv_msgbox_create(lv_layer_top(), "Primljeno", odgovor, NULL, true);
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
