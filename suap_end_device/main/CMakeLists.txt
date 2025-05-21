idf_component_register(SRCS
                        "spi_lcd_touch_example_main.c"
                        "lvgl_demo_ui.c"
                        "drivers/encoder.c"
                        "drivers/display.c"
                        "drivers/littlefs_data.c"
                        "networking/http_handling.c"
                        "user_interface/user_interface.c"
                        "datagram_handling/datagram_parser.c"
                        "datagram_handling/datagram_generator.c"
                        "datagram_handling/main_request_parser.c"
                        "tasks/registration.c"
                        "tasks/request_getter.c"
                       INCLUDE_DIRS ".")

littlefs_create_partition_image(littlefs ../littlefs FLASH_IN_PROJECT)