#include "datagram_handling.h"

int parse_datagram(cJSON **json, char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID){
    cJSON *pom = NULL;

    //main_request = cJSON_Parse(json);
    if(*json == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItem(*json, "ID");
        if (!cJSON_IsString(pom)){
           ESP_LOGE(pars, "ID nije string");
           return -2;
        }
        strcpy(ID, pom->valuestring);
        //printf("%s %s\n", ID, pom->valuestring);


        pom = cJSON_GetObjectItem(*json, "network");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "ID mreže nije string");
            return -3;
        }
        strcpy(network, pom->valuestring);


        pom = cJSON_GetObjectItem(*json, "network_type");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Tip mreže nije broj");
            return -4;
        }
        *network_type = pom->valueint;


        pom = cJSON_GetObjectItem(*json, "interface");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Mrežno sučelje nije string");
            return -5;
        }
        strcpy(interface, pom->valuestring);

        pom = cJSON_GetObjectItem(*json, "sourceID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator izvora nije broj");
            return -6;
        }
        *sourceID = pom->valueint;

        pom = cJSON_GetObjectItem(*json, "targetID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Indentifikator odredišta nije broj");
            return -6;
        }
        *targetID = pom->valueint;

       // pom = cJSON_GetObjectItem(*json, "body");

        //body = pom;
      //  memcpy(*body, pom, sizeof (*pom)); /* copy contents of b0 into b1 */
      //  printf("%ld\n", (long int) cJSON_GetObjectItem(*json, "body"));

    
 //   if(pom != NULL) {
 //       cJSON_Delete(pom);
  //  }
    if(*json != NULL) {
        cJSON_Delete(*json);
        *json = NULL;
    }

    return 0;
}


int parse_datagram_body(cJSON **json, int *request_type, int *device_type, int *logical_clock, int *device_id){
//    cJSON *main_request = json;
    cJSON *pom = NULL;

    //main_request = cJSON_Parse(json);
    if(*json == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
       // printf("%ld\n", (long int) *json);
        pom = cJSON_GetObjectItem(*json, "request_type");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "Vrsta zahtjeva nije broj");
           return -2;
        }
        *request_type = pom->valueint;

        pom = cJSON_GetObjectItem(*json, "device_type");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Tip periferije nije broj");
        }
        *device_type = pom->valueint;


        pom = cJSON_GetObjectItem(*json, "logical_clock");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Logički sats nije broj");
            return -4;
        }
        *logical_clock = pom->valueint;


        pom = cJSON_GetObjectItem(*json, "device_id");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "ID periferije nije broj");
            return -5;
        }
        *device_id = pom->valueint;
        pom = cJSON_GetObjectItem(*json, "body");

        cJSON *pom2 = cJSON_GetObjectItem(*json, "device_id");
        if (!cJSON_IsNumber(pom2)){
            ESP_LOGE(pars, "TIP periferije nije broj");
            return -6;
        }
        *device_type = pom->valueint;



      //  *data = cJSON_GetObjectItem(*json, "data");


    cJSON_Delete(pom);

    return 0;
}

int parse_sensor_datagram(cJSON **json, int* measurement, char* unit){
   // const cJSON *main_request = json;
    cJSON *pom = NULL;

   //main_request = cJSON_Parse(json);
    if(*json == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItem(*json, "measurement");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "iznos mjerenja nije broj");
           return -2;
        }
        *measurement = pom->valueint;

        pom = cJSON_GetObjectItem(*json, "unit");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(pars, "Mjerna jedinica nije string");
            return -3;
        }
        *unit = pom->valueint;
        cJSON_Delete(pom);
    return 0;
}
int parse_actuator_datagram(cJSON **json, int *old_state, int *new_state){
    //const cJSON *main_request = json;
    cJSON *pom = NULL;

    //main_request = cJSON_Parse(json);
    if(*json == NULL){
        ESP_LOGE(pars, "Novo stanje je NULL!!!");
        return -1;
    }

    pom = cJSON_GetObjectItem(*json, "old_state");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "Novo stanje nije broj");
           return -2;
        }
    *old_state = pom->valuestring;

    pom = cJSON_GetObjectItem(*json, "new_state");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(pars, "Novo stanje nije broj");
           return -2;
        }
    *new_state = pom->valuestring;
    
    cJSON_Delete(pom);

    return 0;
}

int parse_user_datagram(cJSON **json, char *message, bool *input_required, int *user_input){
   // const cJSON *main_request = json;
    cJSON *pom = NULL;

    //main_request = cJSON_Parse(json);
    if(*json == NULL){
        ESP_LOGE(pars, "Novo stanje je NULL!!!");
        return -1;
    }
    pom = cJSON_GetObjectItem(*json, "message");
        if (!cJSON_IsString(pom)){
           ESP_LOGE(pars, "poruka nije string");
           return -2;
        }
    *message = pom->valuestring;
    pom = cJSON_GetObjectItem(*json, "input_required");
    if (cJSON_IsTrue(pom)) {
        *input_required = true;
    }else if (cJSON_IsFalse(pom)) {
        *input_required = false;
    }else{
        ESP_LOGE(pars, "zahtjev za ulazom nije bool");
           return -3;
    }
    pom = cJSON_GetObjectItem(*json, "user_input");
        if (!cJSON_IsNumber(pom) && !cJSON_IsNull(pom)){
           ESP_LOGE(pars, "poruka nije string");
           return -2;
        }
    *user_input = pom->valueint;
    if(cJSON_IsNull(pom)){
        *user_input = NULL;
    }
   
    cJSON_Delete(pom);

    return 0;
}