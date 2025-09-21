/*
 * button_optimize.h
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#ifndef INC_BUTTON_OPTIMIZE_H_
#define INC_BUTTON_OPTIMIZE_H_
#include "main.h"

void buttonsInitO(GPIO_TypeDef *PORT, uint16_t PIN);
//void buttonsFSMO ();
void buttonsFSMO1 (int i);
void buttonsFSMO2(int i);
void buttonsFSMO3 (int i);
int buttonIsPressedO (int index);
int buttonIsHoldO (int index);

#endif /* INC_BUTTON_OPTIMIZE_H_ */
