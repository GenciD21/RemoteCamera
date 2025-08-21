#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

#include "esp_camera.h"

#define BOARD_WROVER_KIT 1

static const char *TAGF = "example:take_picture";

//Pre determined pin out for WROOMs via github
#define CAM_PIN_PWDN 38
#define CAM_PIN_RESET -1  
#define CAM_PIN_VSYNC 6
#define CAM_PIN_HREF 7
#define CAM_PIN_PCLK 13
#define CAM_PIN_XCLK 15
#define CAM_PIN_SIOD 4
#define CAM_PIN_SIOC 5
#define CAM_PIN_D0 11
#define CAM_PIN_D1 9
#define CAM_PIN_D2 8
#define CAM_PIN_D3 10
#define CAM_PIN_D4 12
#define CAM_PIN_D5 18
#define CAM_PIN_D6 17
#define CAM_PIN_D7 16


//all information can be manipulated in the camera config. 
static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,    

    .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,      
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};



esp_err_t camera_initailize(void)
{
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAGF, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

 esp_err_t init_camera(uint32_t xclk_freq_hz, pixformat_t pixel_format, framesize_t frame_size, uint8_t fb_count, int sccb_sda_gpio_num, int sccb_port)
{
    framesize_t size_bak = frame_size;
    if (PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > frame_size) {
        frame_size = FRAMESIZE_HD;
    }

    //initialize the cameras
    esp_err_t ret = esp_camera_init(&camera_config);

    //Error checking 
    if (ESP_OK == ret && PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > size_bak) {
        sensor_t *s = esp_camera_sensor_get();
        s->set_framesize(s, size_bak);
    }

    return ret;
}



//  void printf_img_base64(const camera_fb_t *pic)
// {
//     uint8_t *outbuffer = NULL;
//     size_t outsize = 0;
//     if (PIXFORMAT_JPEG != pic->format) {
//         fmt2jpg(pic->buf, pic->width * pic->height * 2, pic->width, pic->height, pic->format, 50, &outbuffer, &outsize);
//     } else {
//         outbuffer = pic->buf;
//         outsize = pic->len;
//     }

//     uint8_t *base64_buf = calloc(1, outsize * 4);
//     if (NULL != base64_buf) {
//         size_t out_len = 0;
//         mbedtls_base64_encode(base64_buf, outsize * 4, &out_len, outbuffer, outsize);
//         printf("%s\n", base64_buf);
//         free(base64_buf);
//         if (PIXFORMAT_JPEG != pic->format) {
//             free(outbuffer);
//         }
//     } else {
//         ESP_LOGE(TAG, "malloc for base64 buffer failed");
//     }
// }


 camera_fb_t take_picture()
{
    camera_initailize();
    ESP_LOGI(TAGF, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();

    // use pic->buf to access the image
    ESP_LOGI(TAGF, "Picture taken! Its size was: %zu bytes", pic->len);
    esp_camera_fb_return(pic);

    return *pic;
}