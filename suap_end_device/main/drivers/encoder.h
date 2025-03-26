#pragma once


#include "iot_button.h"
#include "driver/pulse_cnt.h"
#include "lvgl.h"

#ifndef encoder_h
#define encoder_h

static const char *encoder = "encoder";

pcnt_unit_handle_t encoders[2] = {NULL, NULL};
QueueHandle_t queues[2] = {NULL, NULL};
int enc_pulse_old[2] = {0, 0};
int enc_pulse_count[2] = {0, 0};
button_handle_t gpio_btn[2] = {NULL, NULL};

#endif