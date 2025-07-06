#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_event.h"
#include <string.h>
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_system.h"
#include "esp_netif.h"


#include "esp_http_client.h"

// Declare the config globally or pass it to functions as needed
esp_http_client_config_t config;
esp_http_client_handle_t client;

static void initialize_configuration_http(char *URL, )
{
    config = (esp_http_client_config_t){
        .url = URL,
        .method = HTTP_METHOD_GET,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .timeout_ms = 5000
    };

    client = esp_http_client_init(&config);
}

static void send_http_request()
{
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI("HTTP_CLIENT", "HTTPS Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE("HTTP_CLIENT", "HTTP GET request failed: %s", esp_err_to_name(err));
    }
}