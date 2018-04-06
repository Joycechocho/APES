#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


// Demo Task declarations
void vTask_1(void *pvParameters);
void vTask_2(void *pvParameters);
void demoSerialTask(void *pvParameters);

SemaphoreHandle_t xSem_1, xSem_2;

// Main function
int main(void)
{
    /* Create a binary semaphore */
    xSem_1 = xSemaphoreCreateBinary();
    xSem_2 = xSemaphoreCreateBinary();

    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Create demo tasks
    xTaskCreate(vTask_1, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(vTask_2, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(demoSerialTask, (const portCHAR *)"Serial",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /* Start the scheduler. */
    xSemaphoreGive(xSem_1);
    vTaskStartScheduler();
    return 0;
}


// Flash the LEDs on the launchpad
void vTask_1(void *pvParameters)
{
    for (;;)
    {
        /* Use the semaphore to wait for the event. */
        xSemaphoreTake(xSem_1, portMAX_DELAY);

        // Turn on LED 1
        LEDWrite(0x0F, 0x01);
        vTaskDelay(1000);

        /* Signal task2 to run */
        xSemaphoreGive(xSem_2);
    }
    /* Should the task implementation ever break out of the above loop, then the task must be deleted */
    vTaskDelete(NULL);
}

// Flash the LEDs on the launchpad
void vTask_2(void *pvParameters)
{
    for (;;)
    {
        /* Use the semaphore to wait for the event. */
        xSemaphoreTake(xSem_2, portMAX_DELAY);

        // Turn on LED 4
        LEDWrite(0x0F, 0x08);
        vTaskDelay(1000);

        xSemaphoreGive(xSem_1);
    }
    /* Should the task implementation ever break out of the above loop, then the task must be deleted */
    vTaskDelete(NULL);
}

// Write text over the Stellaris debug interface UART port
void demoSerialTask(void *pvParameters)
{
    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);


    for (;;)
    {
        UARTprintf("\r\nHello, world from FreeRTOS 9.0!");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
