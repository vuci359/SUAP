#include "datagram_handling.h"

int pom_answr = NULL;
bool answered = false;
//lv_obj_t * mbox2;


void update_logical_clock(int *logical_clock){
    current_logical_clock = fmax(current_logical_clock, *logical_clock) + 1;
}


int parse_request(char *request){
    int err=0;

    char reply[1000];
    char ID[10], network[10], interface[10];
    int network_type, targetID, sourceID;
    const cJSON *body = NULL;

    int request_type, device_type = -1, logical_clock, device_id;
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
    body = cJSON_GetObjectItem(main_request, "body");

    data = cJSON_GetObjectItem(body, "data");

    cJSON *pom = cJSON_GetObjectItem(data, "type");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(pars, "Tip periferije nije broj");
            err = -2;
        }
        else device_type = pom->valueint;

    err = parse_datagram(&main_request, ID, network, &network_type, interface, &sourceID, &targetID);

    err = parse_datagram_body(&body, &request_type, &logical_clock, &device_id);
    update_logical_clock(&logical_clock);
   // printf("error %d\n", request_type);
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

       if(request_type == REQUEST){
       err = read_data_from_sensor(&device_id, &measurement, unit);
       if(err != 0){
        if(main_request != NULL){
            cJSON_Delete(main_request);
            main_request = NULL;
        }
        return err; //kasnije složiti slanje greške...
       }

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
    }else if(request_type == REPLY){
        char* data_from_sensor[60];
        input_required = false;
        data = cJSON_GetObjectItem(body, "data");
        err = parse_sensor_datagram(&data, &measurement, unit);
        sprintf(data_from_sensor,"Data from device %d, sensor %d :\n %d %s",sourceID, device_id, measurement, unit);
        //printf("Data from device %d, sensor %d :\n %d %s",sourceID, device_id, measurement, unit);
        err = display_message_to_user(data_from_sensor, &input_required, &answer);
    }
    else{
        ESP_LOGE(pars,"SENSOR; nepoznata vrsta zahtjeva");
    }


    }else if(device_type == ACTUATOR){
        if(request_type == REQUEST){
        answered = false;
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
        err = get_actuator_state(&device_id, &new_state); //kontrolno čitanje

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

           printf("ttt %s bbb\n",reply);

         //  free(dta);
          // free(bdy);
        }else if(request_type == REPLY){
            char* data_from_actuator[100];
            input_required  = false;
            data = cJSON_GetObjectItem(body, "data");
            err = parse_actuator_datagram(&data, &old_state, &new_state);
            sprintf(data_from_actuator,"Data from device %d, actuator %d :\n old state: %d\n new state: %d",sourceID, device_id, old_state, new_state);
            err = display_message_to_user(data_from_actuator, &input_required, &answer);
        }
        else{
            ESP_LOGE(pars,"SENSOR; nepoznata vrsta zahtjeva");
        }


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
           if(request_type == REPLY){
                input_required = false;
           }
        err = display_message_to_user(message, &input_required, &answer);
        if(err != 0){
            if(main_request != NULL){
                cJSON_Delete(main_request);
                main_request = NULL;
            }
            return err; //kasnije složiti slanje greške...
           }
        if(pom_answr != -1){
            answer = pom_answr;
            printf("answer: %d\n", answer);
        } else{
            answer = NULL;
        }

        err = generate_user_datagram(message, &input_required, &answer);

           strcpy(reply, "");

           char *dta = generate_user_datagram(&message, &input_required, &answer); //USER
           char *bdy = generate_datagram_body(dta, REPLY, &current_logical_clock, &device_id);
           strcpy(reply, generate_datagram(bdy, ID, network, &network_type, interface, &targetID, &sourceID));

           printf("ttt %s bbb\n",reply);

                                    

    }else{
        ESP_LOGE(pars, "Invalid device type");
        strcpy(reply, "");
        return -1;
    }


    if(request_type == REQUEST) {
        post_rest_function(pod, odg, BASE_URL"/Datagram/PushMessage/", reply);
    }
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
    ESP_LOGI(pars, "Turn LCD backlight %d", *new_state);
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, *new_state);
    return 0;
}
int get_actuator_state(int *actuator_id, int *old_state){
    *old_state = gpio_get_level(EXAMPLE_PIN_NUM_BK_LIGHT);
    ESP_LOGI(pars, "Check LCD backlight %d", *old_state);

    return 0;
}

static void mevent_cb(lv_event_t * e)
{
    //lv_obj_t * btn = lv_event_get_target(e);
    //lv_obj_t * label = lv_obj_get_child(btn, 0);
    //LV_UNUSED(label);

    while(1){ //čekam mutex
        if (example_lvgl_lock(-1)) {
            pom_answr = -1;

           // printf("Button %d clicked\n", lv_btnmatrix_get_selected_btn(lv_msgbox_get_btns(mbox2)));
          //  printf("Button text: %s\n",lv_btnmatrix_get_btn_text(lv_msgbox_get_btns(mbox2), lv_btnmatrix_get_selected_btn(lv_msgbox_get_btns(mbox2))));
            //  const int *x = e->user_data;
            //pom_answr = atoi(lv_btnmatrix_get_btn_text(lv_msgbox_get_btns(lv_event_get_target(e)), lv_btnmatrix_get_selected_btn(lv_msgbox_get_btns(lv_event_get_target(e))))); //derefen+renciram x i spremem u pom_answr
            if(lv_msgbox_get_btns(mbox2) != NULL){
                pom_answr = lv_btnmatrix_get_selected_btn(lv_msgbox_get_btns(mbox2));
            }
            answered = true;
            lv_obj_del(mbox2);

        // Release the mutex
            example_lvgl_unlock();
        break;
        }
    }
}

int display_message_to_user(char *message, bool *input_required, int *answer){
    pom_answr = *answer;
        static const char *btn_ids[4] = {"0","1","2","3"};

   // printf("radim msgbox");
    while(1){ //čekam mutex
        if (example_lvgl_lock(-1)) {

            if(*input_required){
                mbox2 = lv_msgbox_create(lv_layer_top(), "MESSAGE", message, btn_ids, true);

            } else {
                mbox2 = lv_msgbox_create(lv_layer_top(), "MESSAGE", message, NULL, true);        
            }

            lv_obj_add_event_cb(mbox2, mevent_cb, LV_EVENT_VALUE_CHANGED, -1);
            lv_obj_center(mbox2);

            lv_group_add_obj(g2,mbox2);
            lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox2));
            if(*input_required){
                lv_group_add_obj(g2,lv_msgbox_get_btns(mbox2));
                lv_btnmatrix_set_btn_ctrl(lv_msgbox_get_btns(mbox2), 0, LV_BTNMATRIX_CTRL_CLICK_TRIG);
                lv_btnmatrix_set_one_checked(lv_msgbox_get_btns(mbox2),true);//namjerno označavam prvi gumb da me ne maltretira da treba nekaj kloknuti...
                lv_group_focus_obj(lv_msgbox_get_btns(mbox2));
            }
            else{
                lv_group_focus_obj(lv_msgbox_get_close_btn(mbox2));
            }
            // Release the mutex
            example_lvgl_unlock();
            break;
        }
    }
 //   printf("msgbox napravljen");
    while(*input_required && !answered){
        vTaskDelay(1000/portTICK_PERIOD_MS); //čekam odgovor, nije vezano za ovu funkciju, samo je mjesto praktično...
    }
    return 0;
}