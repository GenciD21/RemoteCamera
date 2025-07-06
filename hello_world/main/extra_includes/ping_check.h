#ifndef PING_H
#define PING_H

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
#include "esp_log.h"

#include <string.h>
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_netif.h"



// void test_on_ping_success(esp_ping_handle_t hdl, void *args);
// void test_on_ping_timeout(esp_ping_handle_t hdl, void *args);
// void test_on_ping_end(esp_ping_handle_t hdl, void *args);
void initialize_ping(char *URL);

#endif
