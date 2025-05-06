#include "datagram_handling.h"

lv_obj_t * mbox2;


void update_logical_clock(int *logical_clock){
    current_logical_clock = fmax(current_logical_clock, *logical_clock) + 1;
}


int parse_request(char *request){
    int err=0;

    char reply[1000];
    char ID[10], network[10], interface[10];
    int network_type, targetID, sourceID;
    const cJSON *body = NULL;

    int request_type, device_type, logical_clock, device_id;
    const cJSON *data = NULL;
    int measurement;
    char unit[10] = "";
    int old_state, new_state;
    char message[MESSAGE_SIZE];
    bool input_required;
    int answer;
    char pod[1000], odg[25];

    const cJSON *main_request = cJSON_Parse(request);
    if(main_request == NULL){
        ESP_LOGE(pars, "JSON je NULL");
        return -1;
    }

    err = parse_datagram(&main_request, ID, network, &network_type, interface, &sourceID, &targetID);

 //   printf("%ld\n", (long int) body);
 //   cJSON *pom = cJSON_GetObjectItem(body, "device_id");
    printf("%s\n", ID);

    printf("error %d\n", err);

    if(err != 0){
        if(main_request != NULL){
         //   cJSON_Delete(main_request);
            main_request = NULL;
        }
        return err;
    }
 //   if(main_request != NULL) {
  //      cJSON_Delete(main_request);
 //   }

    main_request = cJSON_Parse(request); //trikovi zaneznam kaj...
    body = cJSON_GetObjectItem(main_request, "body");

    if(main_request == NULL){
        ESP_LOGE(pars, "JSONn je NULL");
        return -1;
    }

//
    err = parse_datagram_body(&body, &request_type, &device_type, &logical_clock, &device_id);
    update_logical_clock(&logical_clock);
    printf("error %d\n", request_type);
   // err=1;

    if(err != 0){
        if(main_request != NULL){
            cJSON_Delete(main_request);
            main_request = NULL;
        }
        return err;
    }


//tu razbiti na metode za trančirati request i reply

    if(device_type == IOT_SENZOR){ //neznam zakaj tu crveno podcrtava...
       // err = parse_sensor_datagram(data, &measurement, unit); //treba mi za reply
       // err = display_sensor_data_to_user(sourceID, device_id, measurement, unit); //za prekapati reply
       err = read_data_from_sensor(&device_id, &measurement, unit);
       if(err != 0){
        if(main_request != NULL){
            cJSON_Delete(main_request);
            main_request = NULL;
        }
        return err; //kasnije složiti slanje greške...
       }
    //   printf("%d %s\n", measurement, unit);

   // char* fff = generate_sensor_datagram(&measurement, unit);
    //   if(fff == NULL){
    //    printf("NULL pointer!!!!!!");
    //   }
    //   else{
     //   printf("%s\n", fff);
     //  }
       //stvaranje odgovora
       strcpy(reply, "");
       char *dta = generate_sensor_datagram(&measurement, unit); //IOT_SENSOR
     //  printf("ttt %s bbb\n",dta);

       char *bdy = generate_datagram_body(dta, REPLY, &current_logical_clock, &device_id);
     //  printf("ttt %s bbb\n",bdy);

       strcpy(reply, generate_datagram(bdy, ID, network, &network_type, interface, &targetID, &sourceID));
       //free(dta);
       //free(bdy);

        printf("ttt %s bbb\n",reply);
       // ESP_LOGI(pars, "%s", odg);


    }else if(device_type == ACTUATOR){
        data = cJSON_GetObjectItem(body, "data");
        err = parse_actuator_datagram(&data, &old_state, &new_state);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
        err = get_actuator_state(&device_id, &old_state);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
        err = set_actuator_state(&device_id, &new_state);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
           char *dta = generate_actuator_datagram(&old_state, &new_state); //ACTUATOR
           char *bdy = generate_datagram_body(dta, REPLY, &current_logical_clock, &device_id);
           strcpy(reply, generate_datagram(bdy, ID, network, &network_type, interface, &targetID, &sourceID));
         //  free(dta);
          // free(bdy);


    }else if(device_type == USER){
        data = cJSON_GetObjectItem(body, "data");
        err = parse_user_datagram(&data, message, &input_required, &answer); //problem: povezivanje s GUI?
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
        err = display_message_to_user(message, &input_required, &answer);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
        err = generate_user_datagram(NULL, &input_required, &answer);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
           strcpy(reply, "");

           char *dta = generate_user_datagram(&message, &input_required, &answer); //USER
           char *bdy = generate_datagram_body(dta, REPLY, &current_logical_clock, &device_id);
           strcpy(reply, generate_datagram(bdy, ID, network, &network_type, interface, &targetID, &sourceID));
          // free(dta);
          // free(bdy);
                                    

    }else{
        ESP_LOGE(pars, "Invalid device type");
        strcpy(reply, "");
        return -1;
    }

    post_rest_function(pod, odg, API_POST_URL, reply);

    if(main_request != NULL){
        //cJSON_Delete(main_request);
        main_request = NULL;
    }

    return err;

}

int read_data_from_sensor(int *sensor_id, int *value, char *unit){
    if(*sensor_id == WLAN0){
        wifi_ap_record_t wifidata;
        if (esp_wifi_sta_get_ap_info(&wifidata)==0){
        printf("rssi: %d\r\n", wifidata.rssi);
        *value = wifidata.rssi;
        strcat(unit, "dBi");
        }
        else{
            unit = NULL;
            return -1;
        }
    } else{
        unit = NULL;
        ESP_LOGE(pars, "Specified sensor does not exist on current device %d", *sensor_id);
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