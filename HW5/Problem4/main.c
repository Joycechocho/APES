#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"/*supplies GPIO_PIN_x*/
#include "inc/hw_memmap.h"/*supplies GPIO_PORTx_BASE*/

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

// Demo Task declarations
void vTask_1(void *pvParameters);
void vTask_2(void *pvParameters);
void vTask_3(void *pvParameters);

TaskHandle_t xTask3;
TimerHandle_t timing_2HZ;
TimerHandle_t timing_4HZ;
TickType_t tick_send;
TickType_t tick_receive;
xQueueHandle queue = 0;


void timerCallbackForT1(TimerHandle_t timing_2HZ)
{
    xTaskNotify(xTask3, LED_TOGGLE, eSetBits);
}

void timerCallbackForT2(TimerHandle_t timing_4HZ)
{
    tick_send = xTaskGetTickCount();
    xQueueSend(queue,&tick_send,10);
    xTaskNotify(xTask3, LOG_STRING, eSetBits);
}

/* Reference: https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/291574 */
void itoa(long unsigned int value, char* result, int base)
    {
      // check that the base if valid
      if (base < 2 || base > 36) { *result = '\0';}

      char* ptr = result, *ptr1 = result, tmp_char;
      int tmp_value;

      do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
      } while ( value );

      // Apply negative sign
      if (tmp_value < 0) *ptr++ = '-';
      *ptr-- = '\0';
      while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
      }

    }

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Initialize UART
    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);

    // Initialize Queue
    queue = xQueueCreate(10000, sizeof(TickType_t));

    UARTprintf("\nProject For Joyce Cho -- April 8th, 2018\n");

    // Create demo tasks
    xTaskCreate(vTask_1, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(vTask_2, (const portCHAR *)"UART",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(vTask_3, (const portCHAR *)"Task3",
                configMINIMAL_STACK_SIZE, NULL, 1, &xTask3);

    /* Start the scheduler. */
    vTaskStartScheduler();
    return 0;
}


void vTask_1(void *pvParameters)
{
    timing_2HZ = xTimerCreate("Timer1", (250 / portTICK_PERIOD_MS), pdTRUE, (void*) 0, timerCallbackForT1);
    xTimerStart(timing_2HZ, 0);
}


void vTask_2(void *pvParameters)
{
    timing_4HZ = xTimerCreate("Timer2", (500 / portTICK_PERIOD_MS), pdTRUE, (void*) 0, timerCallbackForT2);
    xTimerStart(timing_4HZ, 0);
}

void vTask_3(void *pvParameters)
{
    uint32_t notifyValue;
    uint32_t LED = GPIO_PIN_0;
    for (;;)
    {
        xTaskNotifyWait( pdFALSE, 0xFFFFFFFF, &notifyValue, portMAX_DELAY );
        if(notifyValue & LED_TOGGLE)
        {
            LED ^= (GPIO_PIN_0);
            LEDWrite(0x0F, LED);
        }
        if(notifyValue & LOG_STRING)
        {
            char data[100];
            xQueueReceive(queue, &tick_receive, 10);
            UARTprintf("\n\r Ticks: ");
            itoa(tick_send, data, 10);
            UARTprintf(data);
        }

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
