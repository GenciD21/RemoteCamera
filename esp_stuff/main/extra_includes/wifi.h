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
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_system.h"
#include "esp_netif.h"

#ifndef WIFI_H
#define WIFI_H



void initialize_internet_connection(void);


#endif //WIFI_H
