#ifndef inc_http_handling_h_ccc
#define inc_http_handling0_h_ccc

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//#include <cjson/cJSON.h>
#include "esp_transport_ssl.h"

#include "esp_http_client.h"

static const char *TAGhttp = "HTTP";

#define MAX_HTTP_RECV_BUFFER 512

extern char responseBuffer[MAX_HTTP_RECV_BUFFER];
esp_err_t client_event_http_handler(esp_http_client_event_handle_t evt);

static void post_rest_function(char* odgovor, const char *URL, char* post_data)
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

    //char  *post_data[20];


    //sprintf(post_data,"%d", data);
    
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



#endif