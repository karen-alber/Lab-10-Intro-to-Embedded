#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"

void Timer_Delay();
void switch_interrupt(void);

int main()
{
   //Clock Portf
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
  
  //Timer0 init
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){};
  TimerDisable(TIMER0_BASE, TIMER_A);
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_16_BIT | TIMER_TAMR_TAMR_PERIOD));
  TimerEnable(TIMER0_BASE, TIMER_A);
  
  //interrupt portf
  //IntDisable(INT_TIMER0A);
  GPIOIntDisable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
  GPIOIntRegister (GPIO_PORTF_BASE, switch_interrupt);
  GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
  //IntEnable(INT_TIMER0A);
  GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
  
  while(1)
  {
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    GPIO_PORTF_DATA_R ^= 0x02;
    Timer_Delay(250);
    
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
  }
}

void Timer_Delay(int time)
{
    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerLoadSet(TIMER0_BASE, TIMER_A, (16000*time-1));
    TIMER0_ICR_R = 0x01;
    while((TIMER0_RIS_R & 0x01) == 0){};
    
}

void switch_interrupt(void)
{
  GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_PIN_2);
  while((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) & 0x10) == 0){};
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
}

