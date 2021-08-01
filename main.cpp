/*
 * Afan SAME70 ThreadX.cpp
 *
 * Created: 5/16/2021 9:56:11 PM
 * Author : A41459
 */ 


#include <atmel_start.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "rtos_start.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

int main(void)
{
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();

  /* Disable the watchdog */
  WDT->WDT_MR = WDT_MR_WDDIS;

  SCB_EnableICache();

  printf("Start running ThreadX on Microchip SAME70\r\n");
  ThreadX_V610X_start();
  
  /* Replace with your application code */
  while (1) 
  {
  }
}
