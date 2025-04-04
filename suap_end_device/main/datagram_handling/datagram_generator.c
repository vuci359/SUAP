#include "datagram_handling.h"


char* generate_datagram(char *ID, char *network, int *network_type, char *interface, int *sourceID, int *targetID, char *body){
    char json[REQUEST_STRING_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"ID\":");strcat(json, ID);
        strcat(json, "\"network\":"); strcat(json, network);
        strcat(json, "\"network_type\":"); strcat(json, network_type);
        strcat(json, "\"interface\":"); strcat(json, interface);
        strcat(json, "\"sourceID\":"); strcat(json, sourceID);
        strcat(json, "\"targetID\":"); strcat(json, targetID);
        strcat(json, "\"body\":"); strcat(json, body);
    strcat(json, "}");
    return json;
}

char* generate_datagram_body(int *request_type, int *device_type, int *logical_clock, int *device_id, char *data){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"request_type\":");strcat(json, request_type);
        strcat(json, "\"device_type\":"); strcat(json, device_type);
        strcat(json, "\"logical_clock\":"); strcat(json, logical_clock);
        strcat(json, "\"device_id\":"); strcat(json, device_id);
        strcat(json, "\"data\":"); strcat(json, data);
    strcat(json, "}");
    return json;
}

char* generate_sensor_datagram(int *measurement, char *unit){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        if(measurement == NULL){
            strcat(json, "\"input_required\":"); strcat(json, "null");
        }else{
            strcat(json, "\"input_required\":"); strcat(json, *measurement);
        }
        if(unit == NULL){
            strcat(json, "\"user_input\":"); strcat(json, "null");

        }else {
            strcat(json, "\"user_input\":"); strcat(json, *unit);
        }
    strcat(json, "}");
    return json;}

char* generate_actuator_datagram(int *old_state, int *new_state){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
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

char* generate_user_datagram(char *message, bool *input_required, int *user_input){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"message\":");strcat(json, message);
        if(*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "true");
        }else if(!*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "false");
        }
        if(user_input == NULL){
            strcat(json, "\"user_input\":"); strcat(json, "nullsss");

        }else {
            strcat(json, "\"user_input\":"); strcat(json, user_input);
        }
    strcat(json, "}");
    return json;
}