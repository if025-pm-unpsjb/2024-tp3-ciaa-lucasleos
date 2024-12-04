#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

void buttonTask(void *p);

int main(void) {
    boardConfig();

    xTaskCreate(buttonTask, "ButtonTask", configMINIMAL_STACK_SIZE * 2, NULL, configMAX_PRIORITIES - 1, NULL);

    vTaskStartScheduler();

    for (;;);
}

void buttonTask(void *p) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);

    bool ledState[4] = { false, false, false, false };
    bool lastButtonState[4] = { true, true, true, true };

    int buttons[4] = { TEC1, TEC2, TEC3, TEC4 };
    int leds[4] = { LEDB, LED1, LED2, LED3 };

    while (true) {
        for (int i = 0; i < 4; i++) {
            bool currentButtonState = gpioRead(buttons[i]);

            if (lastButtonState[i] && !currentButtonState) {
                ledState[i] = !ledState[i];
                gpioWrite(leds[i], ledState[i]);
            }

            lastButtonState[i] = currentButtonState;
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
