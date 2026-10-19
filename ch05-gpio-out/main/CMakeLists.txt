idf_component_register(SRCS "gpio_blink_main.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES esp_driver_gpio)