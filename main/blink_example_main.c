/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

#define LED_2       2

static uint8_t s_led_state = 0;

#ifdef CONFIG_BLINK_LED_STRIP

static led_strip_handle_t led_strip;

TaskHandle_t led_task_handle = NULL;

static void blink_led(void *task_param)
{
    ESP_LOGI(TAG, "LED1 blinked!");
    while(1) {
        /* If the addressable LED is enabled */
        if (s_led_state) {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            led_strip_set_pixel(led_strip, 0, 16, 16, 16);
            /* Refresh the strip to send data */
            led_strip_refresh(led_strip);
            s_led_state = 0;
            vTaskDelay(1000 / portTICK_PERIOD_MS); // block for 1 sec
        } else {
            /* Set all LED off to clear all pixels */
            led_strip_clear(led_strip);
            s_led_state = 1;
            vTaskDelay(1000 / portTICK_PERIOD_MS); // block for 1 sec
        }
    }
}

static void blink_led2(void *task_param)
{
    while(1) {
        ESP_LOGI(TAG, "LED2 blinked!");
        /* If the addressable LED is enabled */
        if (s_led_state) {
            gpio_set_level(GPIO_NUM_2, 1);
            s_led_state = 0;
            vTaskDelay(2000 / portTICK_PERIOD_MS); // block for 2 sec
        } else {
            /* Set all LED off to clear all pixels */
            gpio_set_level(GPIO_NUM_2, 0);
            s_led_state = 1;
            vTaskDelay(2000 / portTICK_PERIOD_MS); // block for 2 sec
        }
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
#endif
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

static void configure_led2(void) {
    /*Physical LED configuration*/
    ESP_LOGI(TAG, "Example configured to blink addressable LED2!");

    gpio_config_t led2_config = {
        .pin_bit_mask = (1ULL << LED_2),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led2_config);
}
#endif

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();
    configure_led2();
    s_led_state = 0;
    xTaskCreate(
        blink_led, // Task function
        "LED Task", // Task name 
        4096,       // Stack size
        NULL,       // Task parameter
        1,          // Task priority
        &led_task_handle // Task handle
    );

    xTaskCreate(
        blink_led2,
        "LED2 Task",
        4096,
        NULL,
        2,
        NULL
    );
}
