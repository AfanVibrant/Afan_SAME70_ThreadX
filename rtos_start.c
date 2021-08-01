/****************************************************************************
 * Afan SAME70 Thread-X rtos_start.c
 * Author : Afan Vibrant (AfanVibrant@outlook.com)
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <atmel_start.h>
#include <peripheral_clk_config.h>

#include "rtos_config.h"

#include "rtos.h"
#include "rtos_start.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static CHAR *RTOSPoolAddr;
static TX_BYTE_POOL RTOSMboxMSGPool;

static const char *led_task_name = "led task";
static TX_THREAD led_thread_id;

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions Prototypes
 ****************************************************************************/

static void led_thread_entry(ULONG parameter);
static void user_application_run(void);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: led_thread_entry
 *
 * Description:
 *   Task for LED.
 *
 * Input Parameters:
 *   NONE.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void led_thread_entry(ULONG parameter)
{
  (void)parameter;

  while (1)
  {
    printf("led task run\r\n");
    rtos_task_delay_ms(500);
  }
}

/****************************************************************************
 * Name: user_application_run
 *
 * Description:
 *   Task for user application.
 *
 * Input Parameters:
 *   NONE.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void user_application_run(void)
{
  UINT ret;

  ret = tx_byte_allocate(&RTOSMboxMSGPool,
             (VOID **) &RTOSPoolAddr,
             (ULONG)LEDRUN_THREAD_STKSZ, TX_NO_WAIT);
  if (ret != TX_SUCCESS) {
    printf("%s task stack allocate fail\r\n", led_task_name);
    goto outs;
  }

  ret = tx_thread_create(&led_thread_id, (CHAR *)led_task_name,
                         led_thread_entry, (ULONG)NULL,
                         RTOSPoolAddr, (ULONG)LEDRUN_THREAD_STKSZ,
                         (rtos_thread_t)LEDRUN_THREAD_PRIO,
                         (rtos_thread_t)LEDRUN_THREAD_PRIO,
                         TX_NO_TIME_SLICE, TX_AUTO_START);

  if(TX_SUCCESS == ret) {

  }
  else {
    printf("%s task with priority-%d create fail\r\n", led_task_name, LEDRUN_THREAD_PRIO);
  }

outs:
  return;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tx_application_define
 *
 * Description:
 *   Define what the initial system looks like.
 *
 * Input Parameters:
 *   first_unused_memory:
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void  tx_application_define(void *first_unused_memory)
{
  /* Create a byte memory pool from which to allocate the thread stacks.  */

  tx_byte_pool_create(&RTOSMboxMSGPool, "RTOS Pool 0",
                      first_unused_memory,
                      TOTAL_THREAD_STKSZ + TOTAL_THREAD_MSGSZ);

  user_application_run();
}

/****************************************************************************
 * Name: ThreadX_V610X_start
 *
 * Description:
 *   Run Thread-X kernel.
 *
 * Input Parameters:
 *   first_unused_memory:
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void ThreadX_V610X_start(void)
{
  /* Enter the ThreadX kernel.  */
  tx_kernel_enter();
}
