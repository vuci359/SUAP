#include "datagram_handling.h"

int parse_datagram(char *json, char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID, char *body){
    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

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

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "sourceID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator izvora nije broj");
            return -6;
        }
        *sourceID = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "targetID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator odredišta nije broj");
            return -6;
        }
        *targetID = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "request_body");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Sadržaj zahtjeva nije string");
            return -7;
        }
        *body = pom->valuestring;


    cJSON_Delete(main_request);
    cJSON_Delete(pom);

    return 0;
}


int parse_datagram_body(char *json, int *request_type, int *device_type, int *logical_clock, int *device_id, char *data){
    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

    main_request = cJSON_Parse(json);
    if(main_request == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItemCaseSensitive(main_request, "request_type");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "Vrsta zahtjeva nije broj");
           return -2;
        }
        *request_type = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "device_type");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "ID mreže nije string");
            return -3;
        }
        *device_type = pom->valuestring;


        pom = cJSON_GetObjectItemCaseSensitive(main_request, "logical_clock");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Logički sats nije broj");
            return -4;
        }
        *logical_clock = pom->valueint;


        pom = cJSON_GetObjectItemCaseSensitive(main_request, "device_id");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "ID periferije nije broj");
            return -5;
        }
        *device_id = pom->valuestring;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "data");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Podaci nisu string");
            return -6;
        }
        *data = pom->valueint;


    cJSON_Delete(main_request);
    cJSON_Delete(pom);

    return 0;
}

int parse_sensor_datagram(char *json, int* measurement, char* unit){
    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

    main_request = cJSON_Parse(json);
    if(main_request == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItemCaseSensitive(main_request, "measurement");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "iznos mjerenja nije broj");
           return -2;
        }
        *measurement = pom->valueint;

        pom = cJSON_GetObjectItemCaseSensitive(main_request, "unit");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Mjerna jedinica nije string");
            return -3;
        }
        *unit = pom->valueint;
    cJSON_Delete(main_request);
    return -1;
}
int parse_actuator_datagram(char *json, int *new_state){
    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

    main_request = cJSON_Parse(json);
    if(main_request == NULL){
        ESP_LOGE(pars, "Novo stanje je NULL!!!");
        return -1;
    }
    pom = cJSON_GetObjectItemCaseSensitive(main_request, "new_state");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "Novo stanje nije broj");
           return -2;
        }
        *new_state = pom->valuestring;
    
    cJSON_Delete(main_request);
    cJSON_Delete(pom);

    return 0;
}

int parse_user_datagram(char *json, char *message, bool *input_required, int *user_input){
    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

    main_request = cJSON_Parse(json);
    if(main_request == NULL){
        ESP_LOGE(pars, "Novo stanje je NULL!!!");
        return -1;
    }
    pom = cJSON_GetObjectItemCaseSensitive(main_request, "message");
        if (!cJSON_IsString(pom)){
           ESP_LOGE(pars, "poruka nije string");
           return -2;
        }
    *message = pom->valuestring;
    pom = cJSON_GetObjectItemCaseSensitive(main_request, "input_required");
    if (cJSON_IsTrue(pom)) {
        *input_required = true;
    }else if (cJSON_IsFalse(pom)) {
        *input_required = false;
    }else{
        ESP_LOGE(pars, "zahtjev za ulazom nije bool");
           return -3;
    }
    pom = cJSON_GetObjectItemCaseSensitive(main_request, "user_input");
        if (!cJSON_IsNumber(pom) && !cJSON_IsNull(pom)){
           ESP_LOGE(pars, "poruka nije string");
           return -2;
        }
    *user_input = pom->valueint;
    if(cJSON_IsNull(pom)){
        *user_input = NULL;
    }
   
    cJSON_Delete(main_request);
    cJSON_Delete(pom);

    return 0;
}