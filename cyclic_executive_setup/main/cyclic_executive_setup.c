/* Demo table-based cyclic task dispatcher for Linux with
multiple task slots */
#include <stdio.h>
#include <ctype.h>
#include <sys/times.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define SLOTX 4
#define CYCLEX 5
#define SLOT_T 5000 // 5 sec slot time
#define configUSE_16_BIT_TICKS  0

static const char* TAG = "cyclic_executive_setup";

int tps, cycle = 0, slot = 0;
TickType_t now, then;
struct tms n;

void one()
{ 
    ESP_LOGI(TAG, "task 1 running");
    vTaskDelay(1000 / portTICK_RATE_MS);
}
void two()
{
    ESP_LOGI(TAG, "task 2 running");
    vTaskDelay(2000 / portTICK_RATE_MS);
}
void three()
{
    ESP_LOGI(TAG, "task 3 running");
    vTaskDelay(3000 / portTICK_RATE_MS);
}
void four()
{
    ESP_LOGI(TAG, "task 4 running");
    vTaskDelay(4000 / portTICK_RATE_MS);
}
void five()
{
    ESP_LOGI(TAG, "task 5 running");
    vTaskDelay(5000 / portTICK_RATE_MS);
}
void burn()
{
    TickType_t bstart = xTaskGetTickCount();
    ESP_LOGI(TAG, "bstart = %2.2d\n\n", bstart);
    while (((now = xTaskGetTickCount()) - then) < SLOT_T * tps / 1000)
    {
        /* burn time here */
    }
    ESP_LOGI(TAG, "burn time = %2.2dms\n\n", ((xTaskGetTickCount() - bstart) * 1000 / tps));
    then = now;
    cycle = CYCLEX;
}

void (*ttable[SLOTX][CYCLEX])() = {
    {one, two, burn, burn, burn},
    {one, three, burn, burn, burn},
    {one, four, burn, burn, burn},
    {burn, burn, burn, burn, burn}};

void app_main()
{
    //configTICK_TYPE_WIDTH_IN_BITS() = TICK_TYPE_WIDTH_32_BITS;
    tps = portTICK_RATE_MS*10;
    ESP_LOGI(TAG, "clock ticks/sec = %d\n\n", tps);
    while (1)
    {
        for (slot = 0; slot < SLOTX; slot++)
            for (cycle = 0; cycle < CYCLEX; cycle++)
                (*ttable[slot][cycle])(); // dispatch next task
        // from table
    }
}
