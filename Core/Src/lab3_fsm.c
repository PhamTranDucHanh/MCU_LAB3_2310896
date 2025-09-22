/*
 * lab3_fsm.c
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#include "lab3_fsm.h"
#define fsm1_timer 		4
#define fsm2_timer		5
#define fsm_dur			500

#define mode_butt		0
#define change_butt 	1
#define set_butt		2

#define red_default		5
#define green_default	3
#define yellow_default	2

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

int GREEN_DUR = green_default;
int RED_DUR = red_default;
int YELLOW_DUR = yellow_default;

int tempGreen;
int tempYellow;
int tempRed;

int count1 = red_default;
int count2 = green_default;  //default value
void lab3_fsm1(){
	switch (currentState1){
		case INIT:
			timerSet(fsm1_timer, fsm_dur);
			displayNone();
			count1 = RED_DUR;

			currentState1 = AUTO_RED;
			break;
		case AUTO_RED:
			displayRed1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				currentState1 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState1 = INIT;
			}
			if (count1 <= 0){
				count1 = GREEN_DUR;
				currentState1 = AUTO_GREEN;
			}
			break;
		case AUTO_GREEN:
			displayGreen1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				currentState1 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState1 = INIT;
			}
			if (count1 <= 0){
				count1 = YELLOW_DUR;
				currentState1 = AUTO_YELLOW;
			}
			break;
		case AUTO_YELLOW:
			displayYellow1();
			if(timerFlag(fsm1_timer)){
				timerSet(fsm1_timer, fsm_dur);
				count1--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				currentState1 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState1 = INIT;
			}
			if (count1 == 0){
				count1 = RED_DUR;
				currentState1 = AUTO_RED;
			}
			break;
		case MAN_RED:
			displayAllRed();
			if (buttonIsPressedO(change_butt)){
				tempRed++;
			}
			if (buttonIsHoldO(change_butt)){
				tempRed--;
			}
			displayNumbers(tempRed, 2);
			if (buttonIsPressedO(set_butt)){
				RED_DUR = tempRed;
				displayNone();
				currentState1 = MAN_GREEN;			//Khi set xong chuyển luôn sang manual tiếp theo
			}

			if (buttonIsPressedO(mode_butt)) {
				displayNone();
				currentState1 = MAN_GREEN;
			}
			break;
		case MAN_GREEN:
			displayAllGreen();
			if (buttonIsPressedO(change_butt)){
				tempGreen++;
			}
			if (buttonIsHoldO(change_butt)){
				tempGreen--;
			}
			displayNumbers(tempGreen, 3);
			if (buttonIsPressedO(set_butt)){
				GREEN_DUR = tempGreen;
				displayNone();
				currentState1 = MAN_YELLOW;
			}

			if (buttonIsPressedO(mode_butt)) {
				displayNone();
				currentState1 = MAN_YELLOW;
			}
			break;
		case MAN_YELLOW:
			displayAllYellow();
			if (buttonIsPressedO(change_butt)){
				tempYellow++;
			}
			if (buttonIsHoldO(change_butt)){
				tempYellow--;
			}
			displayNumbers(tempYellow, 4);
			if (buttonIsPressedO(set_butt)){
				YELLOW_DUR = tempYellow;
				displayNone();
				currentState1 = INIT;
				currentState2 = INIT;		//cho cả hai
			}
			if (buttonIsPressedO(mode_butt)){
				displayNone();
				currentState1 = INIT;
				currentState2 = INIT;			//bug ở đâu đó quanh đây
			}
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
			count2 = GREEN_DUR;

			currentState2 = AUTO_GREEN;
			break;
		case AUTO_RED:
			displayRed2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				currentState2 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState2 = INIT;
			}
			if (count2 <= 0){
				count2 = GREEN_DUR;
				currentState2 = AUTO_GREEN;
			}
			break;
		case AUTO_GREEN:
			displayGreen2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				currentState2 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState2 = INIT;
			}
			if (count2 <= 0){
				count2 = YELLOW_DUR;
				currentState2 = AUTO_YELLOW;
			}
			break;
		case AUTO_YELLOW:
			displayYellow2();
			if(timerFlag(fsm2_timer)){
				timerSet(fsm2_timer, fsm_dur);
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressedO(mode_butt)){
				displayNone();
				currentState2 = MAN_RED;
			}
			if (buttonIsHoldO(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				currentState2 = INIT;
			}
			if (count2 <= 0){
				count2 = RED_DUR;
				currentState2 = AUTO_RED;
			}
			break;
		case MAN_RED:
			//everything is done by the first fsm
			break;
		case MAN_GREEN:

			break;
		case MAN_YELLOW:

			break;
		default:
			break;
	}
}
