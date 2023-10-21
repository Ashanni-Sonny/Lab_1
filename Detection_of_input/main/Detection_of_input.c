/* 
	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)
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

void Led_toggle(){
    // Turn LED on/off may wait 1 second
	static int a = 0;
    if (a == 0) {
        a = 1;
    }
    else {
        a = 0;
    }
	
    gpio_set_level(GPIO_PIN, a);
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
}

static void echo_task(){
    // Configure parameters of an UART driver,
    // communication pins and install the driver
    uart_config_t uart_config = {
        .baud_rate = 74880,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
	char data_char;

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_0, data, BUF_SIZE, 5000/portTICK_RATE_MS);
		
		if(len >0){
			data_char = (char) *data;
			if (data_char == 'a'){
				Led_toggle();
				printf("led toggled\n");
			}
		
		}
		
        // Write data back to the UART
        uart_write_bytes(UART_NUM_0, (const char *) data, len);
		vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
	config();
	echo_task();
	
}

