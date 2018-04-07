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

int count = 0;
int LED1_status = 0;
int LED4_status = 0;
void timerCallback(TimerHandle_t timing)
{
  LED4_status =!  LED4_status;
  count++;
  if(count == 2){
      LED1_status =! LED1_status;
      count = 0;
  }
}

void timerInitialize()
{
    TimerHandle_t timing;
    //  Timer should be having a period of 25ms or 0.25s
    timing = xTimerCreate("Timer", (250 / portTICK_PERIOD_MS), pdTRUE, (void *)2, timerCallback);
    xTimerStart(timing, 0);
}

// Main function
int main(void)
{

    timerInitialize();

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

    /* Start the scheduler. */
    vTaskStartScheduler();
    return 0;
}


// Flash the LEDs on the launchpad
void vTask_1(void *pvParameters)
{
    for (;;)
    {
        if(LED1_status == 0){
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0x00); //turn off led 1
        }else{
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0x02); //turn on led 1
        }
    }
}

// Flash the LEDs on the launchpad
void vTask_2(void *pvParameters)
{
    for (;;)
    {
        if(LED4_status == 0){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00); //turn off led 4
        }else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);//turn on led 4
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
