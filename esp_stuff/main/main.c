#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_eap_client.h"
#include "ping_check.h"
#include "wifi.h"
#include "http_requests.h"
#include "fotos.h"
#include <string.h>
#include "sdkconfig.h"



#define ESP_WIFI_IDENTITY      "gduraku@purdue.edu"
#define ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY
#define ESP_WIFI_USERNAME  CONFIG_ESP_WIFI_USERNAME

#define BLINK_GPIO 2

static EventGroupHandle_t s_wifi_event_group;

static esp_netif_t *sta_netif = NULL;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";

static int s_retry_num = 0;


//event handler f
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}


void wifi_init_sta(void)
{
    ESP_LOGI(TAG, "Initiating Wifi Protcol");
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_wifi_sta_enterprise_enable());


    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));


    sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .threshold.authmode = WIFI_AUTH_WPA2_ENTERPRISE

        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );

    ESP_ERROR_CHECK(esp_eap_client_set_identity((uint8_t *)ESP_WIFI_IDENTITY, strlen(ESP_WIFI_IDENTITY)));
    ESP_ERROR_CHECK(esp_eap_client_set_username((uint8_t *)ESP_WIFI_USERNAME, strlen(ESP_WIFI_USERNAME)));
    ESP_ERROR_CHECK(esp_eap_client_set_password((uint8_t *)ESP_WIFI_PASS, strlen(ESP_WIFI_PASS)));


    ESP_ERROR_CHECK( esp_wifi_start() );


}

//basic example

void app_main(void)
{
    initialize_internet_connection();
    esp_camera_set_psram_mode(1);
    initialize_configuration_http("http://48.217.67.239:5000/send_data/esp/Ulq24");
    while(1)
    {
     camera_fb_t *picture = take_picture();
    uint8_t * base_64_picture = printf_img_base64(picture);
    send_picture_data(base_64_picture);
    esp_camera_fb_return(picture);
    }

    

}
