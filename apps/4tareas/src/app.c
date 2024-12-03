/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"

/* Tasks periods. */
#define TASK1_PERIOD 	4000
#define TASK2_PERIOD 	6000
#define TASK3_PERIOD 	9000
#define TASK4_PERIOD 	11000

/* Tasks WCETs. */
#define TASK1_WCET		900
#define TASK2_WCET		1900
#define TASK3_WCET		900
#define TASK4_WCET		1900

/*
 * Busy wait the specified number of ticks.
 */
static void vBusyWait(TickType_t ticks);

/*
 * Periodic task.
 */
static void prvTask(void *pvParameters);

void task(void *p);

SemaphoreHandle_t s;

struct xTaskStruct {
	TickType_t wcet;
	TickType_t period;
};

typedef struct xTaskStruct xTask;

xTask task1 = { TASK1_WCET, TASK1_PERIOD };
xTask task2 = { TASK2_WCET, TASK2_PERIOD };
xTask task3 = { TASK3_WCET, TASK3_PERIOD };
xTask task4 = { TASK4_WCET, TASK4_PERIOD };


int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        // Configurar LEDs como salida
        gpioConfig(GPIO0, GPIO_OUTPUT); // LED1
        gpioConfig(GPIO1, GPIO_OUTPUT); // LED2
        gpioConfig(GPIO2, GPIO_OUTPUT); // LED3
        gpioConfig(GPIO3, GPIO_OUTPUT); // LED4

        pwmConfig(0, PWM_ENABLE);

        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);

        pwmWrite(PWM7, 10);

    	s = xSemaphoreCreateMutex();

    	/* Creates the periodic tasks. */
    	xTaskCreate(prvTask, "T1", configMINIMAL_STACK_SIZE + 50, (void*) &task1,
    	configMAX_PRIORITIES - 1, NULL);
    	xTaskCreate(prvTask, "T2", configMINIMAL_STACK_SIZE + 50, (void*) &task2,
    	configMAX_PRIORITIES - 2, NULL);
    	xTaskCreate(prvTask, "T3", configMINIMAL_STACK_SIZE + 50, (void*) &task3,
    	configMAX_PRIORITIES - 3, NULL);
    	xTaskCreate(prvTask, "T4", configMINIMAL_STACK_SIZE + 50, (void*) &task4,
    	configMAX_PRIORITIES - 4, NULL);

        vTaskStartScheduler();
        for(;;);
}

void task(void *p)
{
		TickType_t xPeriod = 10;
		int dutyCicle = 0;
        while( TRUE ) {
                pwmWrite(PWM7, dutyCicle);
                vTaskDelay( xPeriod );
                dutyCicle = ( dutyCicle + 1 ) % 255;
        }
}

static void vBusyWait(TickType_t ticks) {
	TickType_t elapsedTicks = 0;
	TickType_t currentTick = 0;
	while (elapsedTicks < ticks) {
		currentTick = xTaskGetTickCount();
		while (currentTick == xTaskGetTickCount()) {
			__asm__("nop");
		}
		elapsedTicks++;
	}
}


void prvTask(void *pvParameters) {
    xTask *task = (xTask*) pvParameters;
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();

    // Asignar LED en función de la tarea
    gpioMap_t led;
    if (task == &task1) {
        led = GPIO0;
    } else if (task == &task2) {
        led = GPIO1;
    } else if (task == &task3) {
        led = GPIO2;
    } else if (task == &task4) {
        led = GPIO3;
    }

    for (;;) {
        gpioWrite(led, ON); // Encender LED cuando la tarea está activa
        vBusyWait(task->wcet);
        gpioWrite(led, OFF); // Apagar LED cuando la tarea termina

        vTaskDelayUntil(&pxPreviousWakeTime, task->period);
    }

    vTaskDelete(NULL);
}
