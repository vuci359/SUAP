#include "datagram_handling.h"


void update_logical_clock(int *logical_clock){
    current_logical_clock = fmax(current_logical_clock, *logical_clock) + 1;
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
    int reply;

    err = parse_datagram(request, ID, network, &network_type, interface, &sourceID, &targetID, body);

    err = parse_datagram_body(body, &request_type, &device_type, &logical_clock, &device_id, data);
    update_logical_clock(&logical_clock);

//tu razbiti na metode za trančirati request i reply

    if(device_type == SENSOR){ //neznam zakaj tu crveno podcrtava...
       // err = parse_sensor_datagram(data, &measurement, unit); //treba mi za reply
       // err = display_sensor_data_to_user(sourceID, device_id, measurement, unit); //za prekapati reply
       err = read_data_from_sensor(device_id, &measurement, unit);
       //stvaranje odgovora
       generate_datagram(ID, network, &network_type, interface, sourceID, targetID, generate_datagram_body(REPLY, SENSOR, &current_logical_clock, &device_id, generate_sensor_datagram(&measurement, unit)));

    }else if(device_type == ACTUATOR){
        err = parse_actuator_datagram(data, &new_state);

        err = set_actuator_state(device_id, new_state);

    }else if(device_type == USER){
        err = parse_user_datagram(data, message, &input_required); //problem: povezivanje s GUI?
        err = display_message_to_user(message, input_required, reply);
        err = generate_user_reply(input_required,reply);

    }else{
        ESP_LOGE(pars, "Invalid device type");
        return;
    }

    err = generate_replydatagram();

}

int read_data_from_sensor(int *sensor_id, int *value, int unit){
    if(sensor_id == WLAN0){
        wifi_ap_record_t wifidata;
        if (esp_wifi_sta_get_ap_info(&wifidata)==0){
        printf("rssi:%d\r\n", wifidata.rssi);
        *value = wifidata.rssi;
        unit = "dBi";
        }
        else{
            return -1;
        }
    } else{
        ESP_LOGE(pars, "Specified sensor does not exist on current device");
        return -2;
    }
    return 0;
}