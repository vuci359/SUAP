#include "datagram_handling.h"


void update_logical_clock(int *logical_clock){
    current_logical_clock = fmax(current_logical_clock, *logical_clock) + 1;
}


int parse_request(char *request){
    int err=0;

    char *reply;
    char ID[10], network[10], interface[10];
    int network_type, targetID, sourceID;
    char body[REQUEST_BODY_SIZE];

    int request_type, device_type, logical_clock, device_id;
    char data[REQUEST_DATA_SIZE];
    int measurement;
    char unit[10] = "";
    int old_state, new_state;
    char message[MESSAGE_SIZE];
    bool input_required;
    int answer;
    char pod[1000], odg[25];

    err = parse_datagram(request, ID, network, &network_type, interface, &sourceID, &targetID, body);

    err = parse_datagram_body(body, &request_type, &device_type, &logical_clock, &device_id, data);
    update_logical_clock(&logical_clock);

//tu razbiti na metode za trančirati request i reply

    if(device_type == SENSOR){ //neznam zakaj tu crveno podcrtava...
       // err = parse_sensor_datagram(data, &measurement, unit); //treba mi za reply
       // err = display_sensor_data_to_user(sourceID, device_id, measurement, unit); //za prekapati reply
       err = read_data_from_sensor(&device_id, &measurement, unit);
       //stvaranje odgovora
       reply = generate_datagram(ID, network, &network_type, interface, &sourceID, &targetID,
                                    generate_datagram_body(REPLY, SENSOR, &current_logical_clock, &device_id,
                                        generate_sensor_datagram(&measurement, unit)));
        ESP_LOGI(pars, "%s", odg);


    }else if(device_type == ACTUATOR){
        err = parse_actuator_datagram(data, &old_state, &new_state);
        err = get_actuator_state(&device_id, &old_state);
        err = set_actuator_state(&device_id, &new_state);
        reply = generate_datagram(ID, network, &network_type, interface, &sourceID, &targetID,
                                    generate_datagram_body(REPLY, SENSOR, &current_logical_clock, &device_id,
                                        generate_actuator_datagram(&old_state, &new_state)));


    }else if(device_type == USER){
        err = parse_user_datagram(data, message, &input_required, &answer); //problem: povezivanje s GUI?
        err = display_message_to_user(message, &input_required, &answer);
        err = generate_user_datagram(NULL, &input_required, &answer);
        reply = generate_datagram(ID, network, &network_type, interface, &sourceID, &targetID,
                                    generate_datagram_body(REPLY, SENSOR, &current_logical_clock, &device_id,
                                        generate_user_datagram(NULL, &input_required, &answer)));
                                    

    }else{
        ESP_LOGE(pars, "Invalid device type");
        reply = "";
        return -1;
    }

    post_rest_function(pod, odg, API_POST_URL, reply);

    return err;

}

int read_data_from_sensor(int *sensor_id, int *value, char *unit){
    if(sensor_id == WLAN0){
        wifi_ap_record_t wifidata;
        if (esp_wifi_sta_get_ap_info(&wifidata)==0){
        printf("rssi:%d\r\n", wifidata.rssi);
        *value = wifidata.rssi;
        strcat(unit, "dBi");
        }
        else{
            unit = NULL;
            return -1;
        }
    } else{
        unit = NULL;
        ESP_LOGE(pars, "Specified sensor does not exist on current device");
        return -2;
    }
    return 0;
}

int set_actuator_state(int *actuator_id, int *new_state){
    ESP_LOGI(pars, "Turn on LCD backlight");
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, *new_state);
    return 0;
}
int get_actuator_state(int *actuator_id, int *new_state){
    ESP_LOGI(pars, "Turn on LCD backlight");
    *new_state = gpio_get_level(EXAMPLE_PIN_NUM_BK_LIGHT);
    return 0;
}

static void mevent_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    LV_UNUSED(label);
    LV_LOG_USER("Button %s clicked", lv_label_get_text(label));
    lv_obj_del(mbox2);
}

int display_message_to_user(char *message, bool *input_required, int *answer){
    mbox2 = lv_msgbox_create(lv_layer_top(), "MESSAGE", message, answer, true);
    lv_obj_add_event_cb(mbox2, mevent_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox2);
    lv_group_add_obj(g2,mbox2);
    lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox2));
    lv_group_focus_obj(lv_msgbox_get_close_btn(mbox2));
    
    
    return 0;
}