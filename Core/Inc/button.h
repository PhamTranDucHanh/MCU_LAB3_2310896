/*
 * button.h
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"

void buttonsInit(GPIO_TypeDef *PORT, uint16_t PIN);
//void buttonsFSMO ();
void buttonsFSM1 (int i);
void buttonsFSM2(int i);
void buttonsFSM3 (int i);
int buttonIsPressed (int index);
int buttonIsHold (int index);

#endif /* INC_BUTTON_H_ */
