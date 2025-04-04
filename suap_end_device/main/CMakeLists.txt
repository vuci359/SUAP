idf_component_register(SRCS "spi_lcd_touch_example_main.c" "lvgl_demo_ui.c"
                         "drivers/encoder.c" "drivers/display.c"
                         "networking/http_handling.c"
                         "communication_data_handling/request_parser.c" "communication_data_handling/request_generator.c"
                       INCLUDE_DIRS ".")
