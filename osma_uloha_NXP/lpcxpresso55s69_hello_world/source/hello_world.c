/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static int counter = 0;
void CallbackIRQ(uint32_t callback)
{
	if(counter++ > 20)
	{
		counter = 0;
		PRINTF("CALLBACK ARRIVED (2s interval) \r\n");
	}
}
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    //BOARD_BootClockFROHF96M();
    BOARD_BootClockPLL100M();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    BOARD_InitPeripherals();

    PRINTF("hello world.\r\n");

    CTIMER_StartTimer(CTIMER1_PERIPHERAL);
    CTIMER_StartTimer(CTIMER2_PERIPHERAL);

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
