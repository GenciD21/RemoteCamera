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
#include "esp_crt_bundle.h"
#include "lwip/inet.h"
#include "esp_eap_client.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_http_client.h"

#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "HTTP_JSON";
static char output_buffer[MAX_HTTP_OUTPUT_BUFFER]; 


esp_http_client_config_t config;
esp_http_client_handle_t client;


static esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    static char *read_buffer;
    static int output_len;       // Stores number of bytes read

    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        /*
         *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
         *  However, event handler can also be used in case chunked encoding is used.
         */
        if (!esp_http_client_is_chunked_response(evt->client)) {
            // If user_data buffer is configured, copy the response into the buffer
            if (evt->user_data) {
                memcpy(evt->user_data + output_len, evt->data, evt->data_len);
            } else {
                if (read_buffer == NULL) {
                    read_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
                    output_len = 0;
                    if (read_buffer == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                memcpy(read_buffer + output_len, evt->data, evt->data_len);
            }
            output_len += evt->data_len;
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        if (read_buffer != NULL) {
            // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
            // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
            free(read_buffer);
            read_buffer = NULL;
        }
        output_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != 0) {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        if (read_buffer != NULL) {
            free(read_buffer);
            read_buffer = NULL;
        }
        output_len = 0;
        break;
    default:
        break;
    }
    return ESP_OK;
}

void initialize_configuration_http(char *URL)
{
    
    esp_http_client_config_t config = {
        .url = URL,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .user_data = output_buffer,
        .event_handler = client_event_get_handler,
    };

    client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Accept", "application/json");
}



void send_picture_data(uint8_t * image_data)
{
    //Set correct method and header for content
      esp_http_client_set_method(client, HTTP_METHOD_POST);
      esp_http_client_set_header(client, "Content-Type", "multipart/form-data");



     //using doc commands to set up post request
      int content_length = strlen((char *)image_data);
      esp_err_t post_error = esp_http_client_open(client, content_length);

       const uint8_t * post_data = image_data;

      if (post_error == ESP_OK) 
       { //should write 
         int write_len = esp_http_client_write(client, (const char *)image_data, content_length);
         esp_http_client_perform(client);
       }
      else
       {
         ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(post_error));
       }

      ESP_ERROR_CHECK(esp_http_client_cleanup(client)); 
   
}

cJSON* fetch_json_from_server(esp_http_client_method_t request_type)
{
    printf("Gurt 2");

    int content_length = 0;
    cJSON * root = NULL;

    printf("Gurt 2.5");

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status");
        printf("Gurt 3");
        content_length = esp_http_client_get_content_length(client);
        printf("%d\n", content_length);
        if (content_length < MAX_HTTP_OUTPUT_BUFFER) {
            ESP_LOGI(TAG, "Received JSON: %s", output_buffer);
            printf("Gurt 3.5");
            root = cJSON_Parse(output_buffer);
            if (root == NULL) {
                ESP_LOGE(TAG, "JSON Parse Error");
            }
        }
        else {
            ESP_LOGW(TAG, "Response too large for buffer");
        }
} 
    else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    ESP_ERROR_CHECK(esp_http_client_cleanup(client)); 
    return root;
}

void send_data_through_api(__uint8_t base64_encoded, const char *url)
{
     esp_err_t err = esp_http_client_perform(client);

}