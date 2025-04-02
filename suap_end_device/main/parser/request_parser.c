#include "request_parser.h"

static int extract_relevant_data(char *json, char *ID, char *network, int *network_type, char *interface, int *targetID, int *request_type, char *body){
    const cJSON *main_request = NULL;
    const cJSON *pom = NULL;

    main_request = cJSON_Parse(json);
    if(main_request == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItemCaseSensitive(main_request, "ID");
        if (!cJSON_IsString(pom)){
           ESP_LOGE(pars, "ID nije string");
           return -2;
        }
        *ID = pom->valuestring;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "network");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "ID mreže nije string");
            return -3;
        }
        *network = pom->valuestring;


        pom = cJSON_GetObjectItemCaseSensitive(main_request, "network_type");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Tip mreže nije broj");
            return -4;
        }
        *network_type = pom->valueint;


        pom = cJSON_GetObjectItemCaseSensitive(main_request, "interface");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Mrežno sučelje nije string");
            return -5;
        }
        *interface = pom->valuestring;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "targetID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator odredišta nije broj");
            return -6;
        }
        *targetID = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "request_type");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator odredišta nije broj");
            return -7;
        }
        *request_type = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "request_body");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Sadržaj zahtjeva nije string");
            return -8;
        }
        *body = pom->valuestring;


    cJSON_Delete(main_request);
    return 0;
}
