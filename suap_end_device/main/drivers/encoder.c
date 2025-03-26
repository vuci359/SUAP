#include "encoder.h"

pcnt_unit_handle_t encoders[2] = {NULL, NULL};
QueueHandle_t queues[2] = {NULL, NULL};
int enc_pulse_old[2] = {0, 0};
int enc_pulse_count[2] = {0, 0};
button_handle_t gpio_btn[2] = {NULL, NULL};

void encoder1_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
    pcnt_unit_get_count(encoders[0], &enc_pulse_count[0]);
  data->enc_diff =  (enc_pulse_count[0] - enc_pulse_old[0])/2;
  if(iot_button_get_event(gpio_btn[0]) == BUTTON_PRESS_DOWN) data->state = LV_INDEV_STATE_PRESSED;
  else data->state = LV_INDEV_STATE_RELEASED;
   // printf("krepal1\n");
   enc_pulse_old[0] = enc_pulse_count[0];
}
void encoder2_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
    pcnt_unit_get_count(encoders[1], &enc_pulse_count[1]);
  data->enc_diff =  (enc_pulse_count[1] - enc_pulse_old[1])/2;
    if(iot_button_get_event(gpio_btn[1]) == BUTTON_PRESS_DOWN) data->state = LV_INDEV_STATE_PRESSED;
  else data->state = LV_INDEV_STATE_RELEASED;
 //   printf("krepal2\n");
    enc_pulse_old[1] = enc_pulse_count[1];

}

void encoder_init(void* callback, int index, int gpio_CLK, int gpio_DT, int gpio_BTN){
    ESP_LOGI(enc, "install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
        .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &encoders[index]));

    ESP_LOGI(enc, "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(encoders[index], &filter_config));

    ESP_LOGI(enc, "install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = gpio_CLK,
        .level_gpio_num = gpio_DT,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(encoders[index], &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = gpio_DT,
        .level_gpio_num = gpio_CLK,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(encoders[index], &chan_b_config, &pcnt_chan_b));

    ESP_LOGI(enc, "set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_LOGI(enc, "add watch points and register callbacks");
    int watch_points[] = {EXAMPLE_PCNT_LOW_LIMIT, -50, 0, 50, EXAMPLE_PCNT_HIGH_LIMIT};
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(encoders[index], watch_points[i]));
    }
    pcnt_event_callbacks_t cbs = {
        .on_reach = example_pcnt_on_reach,
    };
    queues[index] = xQueueCreate(10, sizeof(int));
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(encoders[index], &cbs, queues[index]));

    ESP_LOGI(enc, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(encoders[index]));
    ESP_LOGI(enc, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(encoders[index]));
    ESP_LOGI(enc, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(encoders[index]));

#if CONFIG_EXAMPLE_WAKE_UP_LIGHT_SLEEP
    // EC11 channel output high level in normal state, so we set "low level" to wake up the chip
    ESP_ERROR_CHECK(gpio_wakeup_enable(gpio_CLK, GPIO_INTR_LOW_LEVEL));
    ESP_ERROR_CHECK(esp_sleep_enable_gpio_wakeup());
    ESP_ERROR_CHECK(esp_light_sleep_start());
#endif

    // create gpio button
button_config_t gpio_btn_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
    .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
    .gpio_button_config = {
        .gpio_num = gpio_BTN,
        .active_level = 0,
    },
};
gpio_btn[index] = iot_button_create(&gpio_btn_cfg);
if(NULL == gpio_btn[index]) {
    ESP_LOGE(enc, "Button create failed");
}
}