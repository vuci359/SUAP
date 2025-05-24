#include "drivers/littlefs_data.h"
#include <cJSON.h>

cJSON *req[MAX_REQUEST_COUNT];
int request_count = 0; //proj predefiniranih zahtjeva

void initialise_device_from_config_file(char *filename){

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

        if (ret != ESP_OK)
        {
                if (ret == ESP_FAIL)
                {
                        ESP_LOGE(TAG, "Failed to mount or format filesystem");
                }
                else if (ret == ESP_ERR_NOT_FOUND)
                {
                        ESP_LOGE(TAG, "Failed to find LittleFS partition");
                }
                else
                {
                        ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
                }
               // return;
        }

    ESP_LOGI(TAG, "Opening file");
    const cJSON *config;

    { //idem u novi scope zboj šparanja memorije...
        FILE *f = fopen(filename, "r");
        if (f == NULL){
            ESP_LOGE(TAG, "Failed to open file for reading");
                return;
        }
        char buffer[2000]; //2000 znakova za konfiguraciju...
        int i = 0, c; //c is the intermediate variable, i is the increment variable
        while ((c = fgetc(f)) != EOF && i<2000) {//Read contents until it reach the end of the file
            buffer[i] = c;
            i++;
        }
        printf("%s\n", buffer);
        //   fprintf(f, "LittleFS Rocks!\n");
        fclose(f);
         config = cJSON_Parse(buffer);
    }

    cJSON *request = NULL, *elem, *pom;
    char widget_type[10];
    char label[10];
    int group = 0; 

    if(config == NULL){
        ESP_LOGE(TAG, "root JSON je NULL");
        return;
    }
    int request_count = cJSON_GetArraySize(config);
    if(request_count > MAX_REQUEST_COUNT){
        request_count = MAX_REQUEST_COUNT; //ignoriram višak konfiguracije da nebi došlo do greške...
    }

    for(int i = 0; i < request_count; i++){
        elem = cJSON_GetArrayItem(config, i);
        pom = cJSON_GetObjectItem(elem, "widget_type");
        pom = cJSON_GetObjectItem(elem, "label");
        pom = cJSON_GetObjectItem(elem, "group");
        pom = cJSON_GetObjectItem(elem, "slider");
        pom = cJSON_GetObjectItem(elem, "method");
        pom = cJSON_GetObjectItem(elem, "request");
        req[i] = malloc(sizeof(pom)); //alociranje memorije
        memcpy(req[i], pom, sizeof(pom)); //spremanje duplikara requesta da se ne izgubi
       // strcpy(req[i], cJSON_Print(pom)); //zahtjev treba kopirati i stringizvlačiti kod slanja..
       // printf("%s\n", req[i]); //ne pije vodu...
    }

    //tu ide obrada JSON-a

    if (example_lvgl_lock(-1)) {
        example_lvgl_demo_ui(encoder1i, encoder2i);
        // Release the mutex
        example_lvgl_unlock();
    } //ne bi trebalo utjecati na kopiju requesta
    cJSON_Delete(config);
}