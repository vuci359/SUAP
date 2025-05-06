#include "datagram_handling.h"


const char* generate_datagram(char *body, char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID){
    static char jsona[REQUEST_STRING_SIZE*3] = "";
  //  strcpy(jsona, "");
    strcpy(jsona, "{");
    char pom[5];

        strcat(jsona, "\"ID\":\"");strcat(jsona, ID);
        strcat(jsona, "\",\"network\":\""); strcat(jsona, network);
        sprintf(pom, "%d", *network_type);
        strcat(jsona, "\",\"network_type\":"); strcat(jsona, pom);
        strcat(jsona, ",\"interface\":\""); strcat(jsona, interface);
        sprintf(pom, "%d", *sourceID);
        strcat(jsona, "\",\"sourceID\":"); strcat(jsona, pom);
        sprintf(pom, "%d", *targetID);
        strcat(jsona, ",\"targetID\":"); strcat(jsona, pom);
        if(body != NULL){
            strcat(jsona, ",\"body\":"); strcat(jsona, body);
        }
        else{
            strcat(jsona, ",\"body\":"); strcat(jsona, "null");
        }
    strcat(jsona, "}");
    return jsona;
}

const char* generate_datagram_body(char *data, int request_type, int *logical_clock, int *device_id){
    static char jsonb[REQUEST_BODY_SIZE*2] = "";
  //  strcpy(jsonb, "");

    strcpy(jsonb, "{");
        char pom[10];
        sprintf(pom, "%d", request_type);

       //char *pom = "5555";
        strcat(jsonb, "\"request_type\":");strcat(jsonb, pom);
     //   sprintf(pom, "%d", device_type);
      //  strcat(jsonb, ",\"device_type\":"); strcat(jsonb, pom);
        sprintf(pom, "%d", *logical_clock);
        strcat(jsonb, ",\"logical_clock\":"); strcat(jsonb, pom);
        sprintf(pom, "%d", *device_id);
        strcat(jsonb, ",\"device_id\":"); strcat(jsonb, pom);
        if(data != NULL){
            strcat(jsonb, ",\"data\":"); strcat(jsonb, data);
        }
        else{
            strcat(jsonb, ",\"data\":"); strcat(jsonb, "null");
        }
    strcat(jsonb, "}");
    return jsonb;
}

const char* generate_sensor_datagram(int *measurement, char *unit){
    static char json[REQUEST_BODY_SIZE] = "";
    //printf("prob... %s", json);

    strcpy(json, "{\"type\":0, ");
        if(measurement == NULL){
            strcat(json, "\"measurement\":"); strcat(json, "null");
            //printf("probaa... %s", json);

        }else{
            char pom[5]; sprintf(pom ,"%d", *measurement);

            strcat(json, "\"measurement\":"); strcat(json, pom);
            //printf("probaaa... %s", json);

        }
        if(unit == NULL){
            strcat(json, ",\"unit\":"); strcat(json, "null");
            //printf("probaaaa... %s", json);


        }else {
            strcat(json, ",\"unit\":\""); strcat(json, unit); strcat(json, "\"");
            //printf("probaaaaa... %s", json);

        }
    strcat(json, "}");
  //  printf("proba... %s", json);
    return json;}

const char* generate_actuator_datagram(int *old_state, int *new_state){
    static char json[REQUEST_BODY_SIZE] = "";
    strcpy(json, "{\"type\":1, ");
    if(old_state == NULL){
            strcat(json, "\"old_state\":");strcat(json, "null");
        } else {
            strcat(json, "\"old_state\":");strcat(json, old_state);
        }
        if(new_state == NULL){
            strcat(json, "\"new_state\":");strcat(json, "null");
        } else {
            strcat(json, "\"new_state\":");strcat(json, new_state);
        }    strcat(json, "}");
    return json;
}

const char* generate_user_datagram(char *message, bool *input_required, int *user_input){
    static char json[REQUEST_BODY_SIZE] = "";
    strcpy(json, "{\"type\":2, ");
    strcat(json, "\"message\":");strcat(json, message);
        if(*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "true");
        }else if(!*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "false");
        }
        if(user_input == NULL){
            strcat(json, "\"user_input\":"); strcat(json, "null");

        }else {
            strcat(json, "\"user_input\":"); strcat(json, *user_input);
        }
    strcat(json, "}");
    return json;
}