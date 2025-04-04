#include "datagram_handling.h"


void update_logical_clock(logical_clock){
    current_logical_clock = fmax(current_logical_clock, logical_clock) + 1;
}


parse_request(char *request){
    int err=0;

    char *ID, *network, *interface;
    int network_type, targetID, sourceID;
    char *body;

    int request_type, device_type, logical_clock, device_id;
    char *data;
    int measurement;
    char *unit;
    int old_state, new_state;
    char *message;
    bool input_required;

    err = parse_datagram(request, ID, network, &network_type, interface, &sourceID, &targetID, body);
    err = parse_datagram_body(body, &request_type, &device_type, &logical_clock, &device_id, data);
    if(device_type == SENSOR){
        err = parse_sensor_datagram(data, &measurement, unit);

    }else if(device_type == ACTUATOR){
        err = parse_actuator_datagram(data, &new_state);

    }else if(device_type == USER){
        parse_user_datagram(data, message, &input_required);
    }else{
        ESP_LOGE(pars, "Invalid device type");
        return;
    }


    update_logical_clock();

}