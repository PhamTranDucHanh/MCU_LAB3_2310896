/*
 * button.h
 *
 *  Created on: Sep 17, 2025
 *      Author: HANH
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

void buttonsInit(GPIO_TypeDef *PORT, uint16_t PIN);
void buttonsFSM ();
int buttonIsPressed (int index);
int buttonIsHold (int index);

#endif /* INC_BUTTON_H_ */
