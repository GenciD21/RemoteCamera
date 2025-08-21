#ifndef FOTOS_H
#define FOTOS_H
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "esp_camera.h"

#define BOARD_WROVER_KIT 1


    esp_err_t init_camera(uint32_t xclk_freq_hz, pixformat_t pixel_format, framesize_t frame_size, uint8_t fb_count, int sccb_sda_gpio_num, int sccb_port);
    camera_fb_t take_picture();

#endif