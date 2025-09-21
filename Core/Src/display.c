/*
 * display.c
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#include "display.h"

#define seg_timer 	6
#define blink_timer 7
#define seg_dur		250
#define blink_dur	500

	  uint16_t segPins1[7] = {a1_Pin, b1_Pin, c1_Pin, d1_Pin, e1_Pin, f1_Pin, g1_Pin};
	  uint16_t segPins2[7] = {a2_Pin, b2_Pin, c2_Pin, d2_Pin, e2_Pin, f2_Pin, g2_Pin};
	  uint16_t enablePins1[2] = {EN1W1_Pin, EN2W1_Pin};
	  uint16_t enablePins2[2] = {EN1W2_Pin, EN2W2_Pin};
	  // Bảng mã cho từng số (0-9), 0 = sáng (common anode)
	  const uint8_t digitCode[10][7] = {
	    {0,0,0,0,0,0,1}, // 0
	    {1,0,0,1,1,1,1}, // 1
	    {0,0,1,0,0,1,0}, // 2
	    {0,0,0,0,1,1,0}, // 3
	    {1,0,0,1,1,0,0}, // 4
	    {0,1,0,0,1,0,0}, // 5
	    {0,1,0,0,0,0,0}, // 6
	    {0,0,0,1,1,1,1}, // 7
	    {0,0,0,0,0,0,0}, // 8
	    {0,0,0,0,1,0,0}  // 9
	  };

	  void display7SEGs(int num1, int num2) {
	    if (num1 > 9 || num1 < 0 || num2 < 0 || num2 > 9) return;
	    for (int i = 0; i < 7; i++) {
	      HAL_GPIO_WritePin(GPIOB, segPins1[i], digitCode[num1][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
	      HAL_GPIO_WritePin(GPIOB, segPins2[i], digitCode[num2][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
	    }
	  }
	  void enableSEGs(int idx)	{
	  	for (int i = 0; i < 2; i++){
	  		if(i == idx){
	  			HAL_GPIO_WritePin(GPIOA, enablePins1[i], GPIO_PIN_RESET);
	  			HAL_GPIO_WritePin(GPIOA, enablePins2[i], GPIO_PIN_RESET);
	  		}
	  		else {
	  			HAL_GPIO_WritePin(GPIOA, enablePins1[i], GPIO_PIN_SET);
	  			HAL_GPIO_WritePin(GPIOA, enablePins2[i], GPIO_PIN_SET);
	  		}
	  	}
	  }
int idx = 0;
void displayNumbers(int num1, int num2){
	if (timerFlag(seg_timer)){
		timerSet(seg_timer, seg_dur);

		switch (idx){
			case 0:
				display7SEGs(num1%10, num2%10);
				enableSEGs(idx);
				idx++;
				break;
			case 1:
				display7SEGs(num1/10, num2/10);
				enableSEGs(idx);

				idx = 0;
				break;
			default:
				break;
		}
	}
}

void displayRed1(){
	HAL_GPIO_WritePin(GPIOA, RED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_SET);
}

void displayYellow1(){
	HAL_GPIO_WritePin(GPIOA, RED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_SET);
}

void displayGreen1(){
	HAL_GPIO_WritePin(GPIOA, RED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
}

void displayRed2(){
	HAL_GPIO_WritePin(GPIOA, RED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_SET);
}

void displayYellow2(){
	HAL_GPIO_WritePin(GPIOA, RED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_SET);
}

void displayGreen2(){
	HAL_GPIO_WritePin(GPIOA, RED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
}

void displayNone(){
	HAL_GPIO_WritePin(GPIOA, RED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, RED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_SET);
}

void displayAllRed(){
	if(timerFlag(blink_timer)){
		timerSet(blink_timer, blink_dur);

		HAL_GPIO_TogglePin(GPIOA, RED1_Pin);
		HAL_GPIO_TogglePin(GPIOA, RED2_Pin);
	}
}

void displayAllGreen(){
	if(timerFlag(blink_timer)){
		timerSet(blink_timer, blink_dur);

		HAL_GPIO_TogglePin(GPIOA, GREEN1_Pin);
		HAL_GPIO_TogglePin(GPIOA, GREEN2_Pin);
	}
}

void displayAllYellow(){
	if(timerFlag(blink_timer)){
		timerSet(blink_timer, blink_dur);

		HAL_GPIO_TogglePin(GPIOA, YELLOW1_Pin);
		HAL_GPIO_TogglePin(GPIOA, YELLOW2_Pin);
	}
}
