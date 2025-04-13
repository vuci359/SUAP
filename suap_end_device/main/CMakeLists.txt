idf_component_register(SRCS "spi_lcd_touch_example_main.c" "lvgl_demo_ui.c"
                         "drivers/encoder.c" "drivers/display.c"
                         "networking/http_handling.c"
                         "datagram_handling/datagram_parser.c" "datagram_handling/datagram_generator.c"
                         "datagram_handling/main_request_parser.c"
                       INCLUDE_DIRS ".")
