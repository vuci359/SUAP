#include "communication_data_handling.h"


char* generate_request(char *ID, char *network, int *network_type, char *interface, int *targetID, char *body){
    char json[REQUEST_STRING_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"ID\":");strcat(json, *ID);
        strcat(json, "\"network\":"); strcat(json, *network);
        strcat(json, "\"network_type\":"); strcat(json, *network_type);
        strcat(json, "\"interface\":"); strcat(json, *interface);
        strcat(json, "\"targetID\":"); strcat(json, *targetID);
        strcat(json, "\"body\":"); strcat(json, *body);
    strcat(json, "}");
    return json;
}

char* generate_request_body(int *request_type, int *device_type, int *logical_clock, int *device_id, char *data){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"request_type\":");strcat(json, *request_type);
        strcat(json, "\"device_type\":"); strcat(json, *device_type);
        strcat(json, "\"logical_clock\":"); strcat(json, *logical_clock);
        strcat(json, "\"device_id\":"); strcat(json, *device_id);
        strcat(json, "\"data\":"); strcat(json, *data);
    strcat(json, "}");
    return json;
}

char* generate_sensor_request(){
    return "null";
}

char* generate_actuator_request(int *new_state){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"old_state\":");strcat(json, "null");
        strcat(json, "\"new_state\":"); strcat(json, *new_state);
    strcat(json, "}");
    return json;
}

char* generate_user_request(char *message, bool *input_required){
    char json[REQUEST_BODY_SIZE] = "";
    strcat(json, "{");
        strcat(json, "\"message\":");strcat(json, *message);
        if(*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "true");
        }else if(!*input_required){
            strcat(json, "\"input_required\":"); strcat(json, "false");
        }
    strcat(json, "}");
    return json;
}