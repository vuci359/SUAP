#include "drivers/littlefs_data.h"


void initialise_device_from_config_file(char *filename){

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    if (example_lvgl_lock(-1)) {
        example_lvgl_demo_ui(encoder1i, encoder2i);
        // Release the mutex
        example_lvgl_unlock();
    }
}