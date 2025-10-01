/*
 * display.h
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"
#define seg_timer 	8
#define blink_timer 9
#define seg_dur		250
#define blink_dur	500

void displayNumbers(int num1, int num2);
void displayRed_Green();
void displayRed_Yellow();
void displayGreen_Red();
void displayYellow_Red();
void displayNone();
void displayAllRed();
void displayAllGreen();
void displayAllYellow();

#endif /* INC_DISPLAY_H_ */
