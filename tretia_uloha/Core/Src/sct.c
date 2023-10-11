/*
 * sct.c
 *
 *  Created on: Oct 11, 2023
 *      Author: xdovic03
 */
#include "main.h"
#include "sct.h"

// array with translation table
// first index chooses display (first, second or third)
// second index chooses requested digit (0,1,2,3,4,5,6,7,8,9)
static const uint32_t reg_values[3][10] = {
	{
	//PCDE--------GFAB @ DIS1
	0b0111000000000111 << 16,
	0b0100000000000001 << 16,
	0b0011000000001011 << 16,
	0b0110000000001011 << 16,
	0b0100000000001101 << 16,
	0b0110000000001110 << 16,
	0b0111000000001110 << 16,
	0b0100000000000011 << 16,
	0b0111000000001111 << 16,
	0b0110000000001111 << 16,
	},
	{
	//----PCDEGFAB---- @ DIS2
	0b0000011101110000 << 0,
	0b0000010000010000 << 0,
	0b0000001110110000 << 0,
	0b0000011010110000 << 0,
	0b0000010011010000 << 0,
	0b0000011011100000 << 0,
	0b0000011111100000 << 0,
	0b0000010000110000 << 0,
	0b0000011111110000 << 0,
	0b0000011011110000 << 0,
	},
	{
	//PCDE--------GFAB @ DIS3
	0b0111000000000111 << 0,
	0b0100000000000001 << 0,
	0b0011000000001011 << 0,
	0b0110000000001011 << 0,
	0b0100000000001101 << 0,
	0b0110000000001110 << 0,
	0b0111000000001110 << 0,
	0b0100000000000011 << 0,
	0b0111000000001111 << 0,
	0b0110000000001111 << 0,
	},
};

// init procedure for SCT LED driver
void sct_init(void){
	sct_led(0);
	// init NOE (negated output enable) and NLA(negated latch enable) to 0
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_RESET);
	// turns output on by setting NOA pin to 0
	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, GPIO_PIN_RESET);
}

// set shift register in SCT to required value
// each bit represents one LED
void sct_led(uint32_t value){
	for (int i=0; i<32; i++){
		uint32_t bit = ((value >> i) & 0b1);
		// first set correct bit value on SCT_SDI pin
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, bit);
		// then send 1ms pulse on SCT_CLK
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_RESET);
	}
	// then send pulse to NLA
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_RESET);

}

// sets 7seg display to display requested value
void sct_value(uint16_t value){
	// reg will hold bit values for all LEDs
	uint32_t reg = 0;
	// find digit for 1st display (hundreds)
	reg |= reg_values[0][value / 100 % 10];
	// find digit for 2nd display (tenths)
	reg |= reg_values[1][value / 10 % 10];
	// find digit for 3rd display (ones)
	reg |= reg_values[2][value / 1 % 10];

	//send calculated req value to sct_led to display it
	sct_led(reg);
}
