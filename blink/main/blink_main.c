/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  // Include for GPIO functions
#include "esp_system.h"

#define GPIO_PIN GPIO_NUM_2

void config(){
    gpio_config_t io_configuration; // Declare the gpio_config_t structure
    io_configuration.pin_bit_mask = (1ULL << GPIO_PIN);
    io_configuration.mode = GPIO_MODE_OUTPUT;
    io_configuration.intr_type = GPIO_INTR_DISABLE;
    io_configuration.pull_up_en = GPIO_PULLUP_DISABLE;
    io_configuration.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_configuration);
}

void blink(){
    // Turn LED off and wait 1 second
    printf("LED off\n");
    gpio_set_level(GPIO_PIN, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Turn LED on and wait 1 second
    printf("LED on\n");
    gpio_set_level(GPIO_PIN, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void app_main()
{
    config();
    while(1){
        blink();
    }
}