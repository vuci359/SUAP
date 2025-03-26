#ifndef inc_encoder_h_ccc
#define inc_encoder_h_ccc


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_log.h"


#include "iot_button.h"
#include "driver/pulse_cnt.h"
#include "lvgl.h"




static const char *enc = "encoder";

#define ENCODER_MAX_WHATCHPIONT_COUNT 100

#define EXAMPLE_PCNT_HIGH_LIMIT 100
#define EXAMPLE_PCNT_LOW_LIMIT  -100

extern pcnt_unit_handle_t encoders[2];
extern QueueHandle_t queues[2];
extern int enc_pulse_old[2];
extern int enc_pulse_count[2];
extern button_handle_t gpio_btn[2];

void encoder1_read(lv_indev_drv_t * drv, lv_indev_data_t*data);
void encoder2_read(lv_indev_drv_t * drv, lv_indev_data_t*data);

static bool example_pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx){
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send event data to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
}

void encoder_init(void* callback, int index, int gpio_CLK, int gpio_DT, int gpio_BTN);


#endif