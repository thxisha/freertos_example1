
# Blink Example using FreeRTOS

This example was a way for me to practice blinking a LED by using FreeRTOS. The led_strip library and on-board FreeRTOS kernel were primarily used to complete this project. In addition, the project was based on Espressif's blink example project and the concepts in the following YouTube tutorial: https://youtu.be/yvxZHZI5mQs.

The biggest modification I made to the blink example project was adapt it to use a FreeRTOS task to control the blink behavior. I used a physical LED and wired it to the dev board for a second blinking LED.

<ins>What I learned/practiced</ins>
- Creating FreeRTOS tasks
- FreeRTOS delays (vTaskDelay())
- Working with ESP-IDF
- ESP-32 GPIO control
- Super loop vs. task-based execution


### Hardware Used

* ESP32-C6-DevKitC with on-board LED
* A USB-C cable for Power supply and programming
* Breadboard, 1k resistor, LED 

