#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"

#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_wpa2.h"

#include <string.h>
#include "esp_crt_bundle.h"

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"

#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "wifi.h"
#include "ping_check.h"
#include "http_requests.h"
#include "sdkconfig.h"


#define URL "http://48.217.67.239:5000/get_data/esp/Ulq24"
#define GET HTTP_METHOD_GET
#define POST HTTP_METHOD_POST

void app_main(void)

{
    int values[6]; //HSV Values

    char * object;  //Object Characteristic

    initialize_internet_connection();

    // ESP_LOGI("PINGING", "Pinging 8.8.8.8");

    // initialize_ping("8.8.8.8");

    ESP_LOGI("HTTP", "START LOGGING");

    printf("\nSending Request to %s\n", URL);

    initialize_configuration_http(URL, GET);

    printf("Gurt 1");

    cJSON * recieved_data = fetch_json_from_server(GET);
    printf("Gurt 3.75");
    cJSON * output = cJSON_GetArrayItem(recieved_data, 0);
    printf("Gurt 4");

    char *string = cJSON_Print(output);
    printf("%s", string);

    for(int i = 0; i < 6; i ++) {
      cJSON * data_point = cJSON_GetArrayItem(output, i);
      values[i] = data_point->valueint;
      printf("%d", values[i])
    }
    cJSON * string_point = cJSON_GetArrayItem(output, 7);
    object = string_point->valuestring;
    printf("\n%s", object);


    cJSON_Delete(recieved_data);
}
