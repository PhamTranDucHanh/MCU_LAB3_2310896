/*
 * lab3_fsm.c
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#include "lab3_fsm.h"
#define fsm1_timer 	4
#define fsm2_timer	5
#define fsm_dur		1000

typedef enum{
	INIT,
	AUTO_RED,
	AUTO_YELLOW,
	AUTO_GREEN,
	MAN_RED,
	MAN_GREEN,
	MAN_YELLOW
} traffic_state;

traffic_state currentState1 = INIT;
traffic_state currentState2 = INIT;

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

int count1 = 0;
int count2 = 0;
void lab3_fsm1(){
	switch (currentState1){
		case INIT:
			timerSet(fsm1_timer, fsm_dur);
			displayNone();

			currentState1 = AUTO_RED;
			break;
		case AUTO_RED:
			displayRed1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1++;
			}

			if (count1 == 5){
				count1 = 0;
				currentState1 = AUTO_GREEN;
			}
			break;
		case AUTO_GREEN:
			displayGreen1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1++;
			}

			if (count1 == 3){
				count1 = 0;
				currentState1 = AUTO_YELLOW;
			}
			break;
		case AUTO_YELLOW:
			displayYellow1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1++;
			}

			if (count1 == 2){
				count1 = 0;
				currentState1 = AUTO_RED;
			}
			break;
		case MAN_RED:

			break;
		case MAN_GREEN:

			break;
		case MAN_YELLOW:

			break;
		default:
			break;
	}
}

void lab3_fsm2(){
	switch (currentState2){
		case INIT:
			timerSet(fsm2_timer, fsm_dur);
			displayNone();

			currentState2 = AUTO_GREEN;
			break;
		case AUTO_RED:
			displayRed2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2++;
			}

			if (count2 == 5){
				count2 = 0;
				currentState2 = AUTO_GREEN;
			}
			break;
		case AUTO_GREEN:
			displayGreen2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2++;
			}

			if (count2 == 3){
				count2 = 0;
				currentState2 = AUTO_YELLOW;
			}
			break;
		case AUTO_YELLOW:
			displayYellow2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2++;
			}

			if (count2 == 2){
				count2 = 0;
				currentState2 = AUTO_RED;
			}
			break;
		case MAN_RED:

			break;
		case MAN_GREEN:

			break;
		case MAN_YELLOW:

			break;
		default:
			break;
	}
}
