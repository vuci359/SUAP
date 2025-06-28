/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "user_interface/user_interface.h"
// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter

//možda treba prebaciti u datoteku s programom
lv_obj_t * list1;
lv_obj_t * list2;
lv_obj_t * list3;

int slider_value = 0;

static lv_obj_t * slider_label;

static uint8_t button_counter = 0;
static lv_obj_t * count_label;
lv_obj_t * mbox1;

lv_group_t * g;
lv_group_t * g2;


static int extract_relevant_data(char *json){

    const cJSON *content_nodes = NULL;
    const cJSON *content_node = NULL;
    const cJSON *zadnja = NULL;
    const cJSON *br = NULL;
    const cJSON *pr = NULL;

    cJSON *json_data = cJSON_Parse(json);
    if(json_data == NULL){
        printf("JSON data IS NULL\n");
        return -1;
    }
    
        zadnja = cJSON_GetObjectItemCaseSensitive(json_data, ZADNJA_VRIJEDONST);
        if (!cJSON_IsNumber(zadnja)){
            return -5;
        }
        //provjera dali su poslani podaci za dobar uređaj
        br = cJSON_GetObjectItemCaseSensitive(json_data, BROJAC);
        if (!cJSON_IsNumber(br)){
            return -5;
        }
        pr = cJSON_GetObjectItemCaseSensitive(json_data, PROSJEK);
        if (!cJSON_IsNumber(pr)){
            return -5;
        }

        zadnja_vrijednost = zadnja->valueint;
        brojac = br->valueint;
        prosjek = pr->valuedouble;
        printf("%d %d %f\n", zadnja_vrijednost, brojac, prosjek);
    

    cJSON_Delete(json_data);
    return 0;
}

static void delete_msgbox(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    LV_UNUSED(label);
    LV_LOG_USER("Button %s clicked", lv_label_get_text(label));
    while(1){ //čekam mutex
      if (example_lvgl_lock(-1)) {

          lv_obj_del(mbox1);

      // Release the mutex
          example_lvgl_unlock();
      break;
      }
  }
}


static void event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
    button_counter++;
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);
  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }
}

static void get_event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT)
      LV_LOG_USER("Press and Hold Event");
     char *odgovor[25];
     char *povrat[50];

     get_rest_function(povrat, odgovor, API_GET_URL, NULL);
     extract_relevant_data(povrat);
    //  *odgovor = "ccc";
    sprintf(odgovor,"Vrijednost: %d\n Brojac: %d\n Prosjek: %f", zadnja_vrijednost, brojac, prosjek);
   //button_counter = button_counter + zadnja_vrijednost;
    lv_label_set_text_fmt(count_label, "Count: %d", button_counter);

    printf("%d %d %f\n", zadnja_vrijednost, brojac, prosjek);
    mbox1 = lv_msgbox_create(lv_layer_top(), "Primljeno", odgovor, NULL, true);
    lv_obj_add_event_cb(mbox1, delete_msgbox, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
      lv_group_add_obj(g2,mbox1);
      lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
      lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));


  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }

}

static void post_event_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED)
      LV_LOG_USER("Click Event");
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
     char podaci[100], odgovor[25];
     char str[10];
      sprintf(str, "%d", button_counter);
     post_rest_function(podaci, odgovor, API_POST_URL, str);

    mbox1 = lv_msgbox_create(lv_layer_top(), "Poslano", odgovor, NULL, true);
    lv_obj_add_event_cb(mbox1, delete_msgbox, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
      lv_group_add_obj(g2,mbox1);
      lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
      lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));


  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }
}



void example_lvgl_demo_ui(lv_indev_t *encoder1i, lv_indev_t *encoder2i)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    g = lv_group_create();
    g2 = lv_group_create();

    /*Create a menu object*/
    lv_obj_t * menu = lv_menu_create(scr);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * label;


    /*Create a sub page*/
    lv_obj_t * sub_page = lv_menu_page_create(menu, NULL);

    list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1, lv_pct(50), lv_pct(100));
    lv_obj_set_style_pad_row(list1, 5, 0);
    lv_obj_align(list1, LV_ALIGN_CENTER, -60, 20);

    list2 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list2, lv_pct(50), lv_pct(100));
    lv_obj_set_style_pad_row(list2, 5, 0);
    lv_obj_align(list2, LV_ALIGN_CENTER, 15, 20);

    list3 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list3, lv_pct(50), lv_pct(100));
    lv_obj_set_style_pad_row(list3, 5, 0);
    lv_obj_align(list3, LV_ALIGN_CENTER, 85, 20);



    lv_obj_t * btn1 = lv_btn_create(list1);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_width(btn1, 55);



    label = lv_label_create(btn1);
     //   lv_obj_set_style_text_font(label, LV_FONT_MONTSERRAT_40, 0);

    lv_label_set_text(label, "Button");
    lv_obj_center(label);


    count_label = lv_label_create(lv_scr_act());
    lv_obj_align(count_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text(count_label, "Counts: 0");

    lv_obj_t * btn2 = lv_btn_create(list1);
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    // lv_obj_align(btn2, LV_ALIGN_CENTER, -50, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);
    lv_obj_set_width(btn2, 55);


    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
    lv_group_add_obj(g, btn1);
    lv_group_add_obj(g,btn2);
    lv_indev_set_group(encoder1i, g);


    lv_obj_t * btn1b = lv_btn_create(list2);
    lv_obj_set_width(btn1b, 55);
    lv_obj_add_event_cb(btn1b, post_event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP POST

    label = lv_label_create(btn1b);
    lv_label_set_text(label, "POST");
    lv_obj_center(label);

    lv_obj_t * btn2b = lv_btn_create(list2);
    lv_obj_set_width(btn2b, 55);
    lv_obj_add_event_cb(btn2b, get_event_handler, LV_EVENT_ALL, NULL); //promjeniti u handler za HTTP GET
    //lv_obj_align(btn2b, LV_ALIGN_CENTER, 50, 40);

    label = lv_label_create(btn2b);
    lv_label_set_text(label, "GET");
    lv_obj_center(label);

    lv_group_add_obj(g2, btn1b);
    lv_group_add_obj(g2,btn2b);
    lv_indev_set_group(encoder2i, g2);

  }

static void user_event_cb(lv_event_t *e){
   char *odgovor[25];
    char *povrat[50];
        lv_obj_t *btn = lv_event_get_target(e);
        int request_id = lv_obj_get_child_id(lv_obj_get_parent(btn));


        cJSON *pom = cJSON_GetObjectItem(postavke[request_id].req, "body"); //dovršiti prepravljanje novog stanjas
        pom = cJSON_GetObjectItem(pom, "data");
        pom = cJSON_GetObjectItem(pom, "new_state");

        cJSON_SetNumberValue(pom , slider_value);

        if(postavke[request_id].has_slider){
          if(strcmp(postavke[request_id].method, "POST") == 0){
            post_rest_function(povrat, odgovor, BASE_URL"/Datagram/PushMessage", cJSON_Print(postavke[request_id].req));

          }
          else if(strcmp(postavke[request_id].method, "GET") == 0){
            get_rest_function(povrat, odgovor, BASE_URL"/Datagram/PushMessage", NULL);
          }
        }           printf("Zahtjev ID %d\n", request_id);
                //    printf("Zahtjev %s\n", cJSON_Print(postavke[request_id].req));

        mbox1 = lv_msgbox_create(lv_layer_top(), "Primljeno", odgovor, NULL, true);
        lv_obj_add_event_cb(mbox1, delete_msgbox, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(mbox1);
        lv_group_add_obj(g2,mbox1);
        lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
        lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));
        mbox1 = lv_obj_get_parent(btn);
        if (example_lvgl_lock(-1)) {

          lv_obj_del(mbox1);

        // Release the mutex
          example_lvgl_unlock();
        }

}


static void slider_event_cb(lv_event_t * e){
    
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    slider_value = lv_slider_get_value(slider);        

}
static void config_event_handler_with_slider(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);    
  
  
    if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
    {
      if ( code == LV_EVENT_CLICKED){
        LV_LOG_USER("Click Event");
        lv_obj_t * btn = lv_event_get_target(e);
      int request_id = lv_obj_get_child_id(btn);

    mbox1 = lv_msgbox_create(lv_layer_top(), "Ulaz", NULL, NULL, true);
    lv_obj_add_event_cb(mbox1, delete_msgbox, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
    lv_group_add_obj(g2,mbox1);
    lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
    lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));


       /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(mbox1);
        lv_group_add_obj(g2, slider);

    lv_obj_center(slider);
    lv_obj_set_width(slider, 150);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_slider_set_range(slider, postavke[request_id].min_value, postavke[request_id].max_value);
    slider_label = lv_label_create(mbox1);
    lv_label_set_text(slider_label, "0");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 0, 0);


  lv_obj_t * btn_cnf = lv_btn_create(mbox1);
  lv_obj_t * btn_label;
  btn_label = lv_label_create(btn_cnf);
  lv_label_set_text(btn_label, "OK");
  lv_obj_center(btn_label);
  lv_group_add_obj(g2, btn_cnf);
  lv_obj_add_event_cb(btn_cnf, user_event_cb, LV_EVENT_CLICKED, NULL);


  //lv_obj_add_event_cb(btn_cnf, user_event_cb, LV_EVENT_ALL, NULL);
    
      
        printf("Stisnut je gumb slider indexom %d; metoda %s.\n", request_id, postavke[request_id].method); //korisno za izračunati eventualni pomak
        //u verziji sa sliderom dodati msgbox i prikazati slider
    }
      else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
        LV_LOG_USER("Press and Hold Event");
    }
    else if(code == LV_EVENT_VALUE_CHANGED)
    {
      LV_LOG_USER("Toggle Event");
    }
  }

static void config_event_handler_without_slider(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
    


  if( (code == LV_EVENT_CLICKED) || (code ==  LV_EVENT_LONG_PRESSED_REPEAT) )
  {
    if ( code == LV_EVENT_CLICKED){
      LV_LOG_USER("Click Event");
      lv_obj_t * btn = lv_event_get_target(e);
    int request_id = lv_obj_get_child_id(btn);


    char *odgovor[25];
    char *povrat[50];

    if(!postavke[request_id].has_slider){
      if(strcmp(postavke[request_id].method, "POST") == 0){
        printf("%s\n", cJSON_Print(postavke[request_id].req));
        post_rest_function(povrat, odgovor, BASE_URL"/Datagram/PushMessage", cJSON_Print(postavke[request_id].req));
      }
      else if(strcmp(postavke[request_id].method, "GET") == 0){
        get_rest_function(povrat, odgovor, BASE_URL"/Datagram/PushMessage", NULL);
      }
    }
    mbox1 = lv_msgbox_create(lv_layer_top(), "Primljeno", odgovor, NULL, true);
    lv_obj_add_event_cb(mbox1, delete_msgbox, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
    lv_group_add_obj(g2,mbox1);
    lv_group_add_obj(g2,lv_msgbox_get_close_btn(mbox1));
    lv_group_focus_obj(lv_msgbox_get_close_btn(mbox1));

      printf("Stisnut je gumb s indexom %d; metoda %s.\n", request_id, postavke[request_id].method); //korisno za izračunati eventualni pomak
      //u verziji sa sliderom dodati msgbox i prikazati slider
  }
    else if( code == LV_EVENT_LONG_PRESSED_REPEAT )
      LV_LOG_USER("Press and Hold Event");
  }
  else if(code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggle Event");
  }
}

int add_button_without_slider(int group, char *label){
  lv_obj_t * btn_cnf = lv_btn_create(list3);
  lv_obj_t * btn_label;
  btn_label = lv_label_create(btn_cnf);
  lv_label_set_text(btn_label, label);
  lv_obj_center(btn_label);
  lv_obj_add_event_cb(btn_cnf, config_event_handler_without_slider, LV_EVENT_ALL, NULL);
  if(group == 1){
      lv_group_add_obj(g, btn_cnf);
  } else if(group == 2){
    lv_group_add_obj(g2, btn_cnf);
}
return 0;
}

int add_button_with_slider(int group, char *label){
  lv_obj_t * btn_cnf = lv_btn_create(list3);
  lv_obj_t * btn_label;
  btn_label = lv_label_create(btn_cnf);
  lv_label_set_text(btn_label, label);
  lv_obj_center(btn_label);
  lv_obj_add_event_cb(btn_cnf, config_event_handler_with_slider, LV_EVENT_ALL, NULL);
  if(group == 1){
      lv_group_add_obj(g, btn_cnf);
  } else if(group == 2){
    lv_group_add_obj(g2, btn_cnf);
}
return 0;
}