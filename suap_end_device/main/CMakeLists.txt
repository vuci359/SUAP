idf_component_register(SRCS
                        "spi_lcd_touch_example_main.c"
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

                       
idf_build_set_property(C_COMPILE_OPTIONS "-fcallgraph-info" APPEND) 
idf_build_set_property(CXX_COMPILE_OPTIONS "-fcallgraph-info" APPEND) 
idf_build_set_property(C_COMPILE_OPTIONS "-fdump-rtl-expand" APPEND) 
idf_build_set_property(CXX_COMPILE_OPTIONS "-fdump-rtl-expand" APPEND) 


littlefs_create_partition_image(littlefs ../littlefs FLASH_IN_PROJECT)

