/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    LPC55S69_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC55S69_cm33_core0.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

const char* heslo = "veslo";
uint8_t heslova_funkcia(const char* input)
{
	int status = strcmp(heslo, input);
	return status;
}


uint8_t heslova_funkcia_safer(const char* input)
{
	uint32_t i;
	uint32_t DWT1, DWT2;
	DWT1 = DWT->CYCCNT;
	uint8_t status = strcmp(heslo, input);
	DWT2 = DWT->CYCCNT;
	while (DWT2-DWT1 < 2000)
	{
		i++;
		DWT2 = DWT->CYCCNT;
	}
	return status;
}

uint8_t pokusov = 0;
uint8_t heslova_funkcia_safer_2(const char* input)
{
	uint32_t i;
	uint32_t DWT1, DWT2;
	DWT1 = DWT->CYCCNT;
	uint8_t status = strcmp(heslo, input);
	DWT2 = DWT->CYCCNT;
	while (DWT2-DWT1 < 2000)
	{
		i++;
		DWT2 = DWT->CYCCNT;
	}
	return status;
}


/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;

    char input_pass[50];

    PRINTF("Starting main loop \r\n");
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;

		PRINTF("Enter password: ");
		SCANF("%49s", input_pass);
		PRINTF("\r\nPassword entered: %s \r\n", input_pass);

		uint8_t result;
		uint32_t DWT1, DWT2;
		DWT1 = DWT->CYCCNT;
		result = heslova_funkcia(input_pass);
		DWT2 = DWT->CYCCNT;
		PRINTF("\nCycles in function classic strcmp: %d\r\n", DWT2 - DWT1);

		DWT1 = DWT->CYCCNT;
		result = heslova_funkcia_safer(input_pass);
		DWT2 = DWT->CYCCNT;
		PRINTF("\nCycles in function safer: %d\r\n", DWT2 - DWT1);

		if (result == 0) {
			PRINTF("Password is correct \r\n");
		} else {
			PRINTF("Password is incorrect \r\n");
		}

        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
