/*
 * sct.h
 *
 *  Created on: Oct 11, 2023
 *      Author: xdovic03
 */

#ifndef INC_SCT_H_
#define INC_SCT_H_

void sct_init();

void sct_led(uint32_t value);

void sct_value(uint16_t value, uint8_t number_of_leds);

#endif /* INC_SCT_H_ */
