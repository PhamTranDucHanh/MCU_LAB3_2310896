/*
 * lab3_fsm.c
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#include "lab3_fsm.h"
#define fsm_timer 		6
#define time_out_timer	7
#define fsm_dur			1000
#define time_out_dur	6000

#define mode_butt		0
#define change_butt 	1
#define set_butt		2

#define red_default		5
#define green_default	3
#define yellow_default	2

typedef enum{
	INIT,
	RED_GREEN,
	RED_YELLOW,
	GREEN_RED,
	YELLOW_RED,
	MAN_RED,
	MAN_GREEN,
	MAN_YELLOW
} traffic_state;

traffic_state fsm_current_state = INIT;

int GREEN_DUR = green_default;
int RED_DUR = red_default;
int YELLOW_DUR = yellow_default;

int tempGreen;
int tempYellow;
int tempRed;

int count1 = red_default;
int count2 = green_default;  //default value
void lab3_fsm(){
	switch (fsm_current_state){
		case INIT:
			timerSet(fsm_timer, fsm_dur);
			displayNone();
			count1 = RED_DUR;
			count2 = GREEN_DUR;

			fsm_current_state = RED_GREEN;
			break;
		case RED_GREEN:
			displayRed_Green();
			if(timerFlag(fsm_timer)){
				timerSet(fsm_timer, fsm_dur);
				count1--;
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressed(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				fsm_current_state = MAN_RED;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				fsm_current_state = INIT;
			}
			if (count2 <= 0){
				count2 = YELLOW_DUR;
				fsm_current_state = RED_YELLOW;
			}
			break;
		case RED_YELLOW:
			displayRed_Yellow();
			if(timerFlag(fsm_timer)){
				timerSet(fsm_timer, fsm_dur);
				count1--;
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressed(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				fsm_current_state = MAN_RED;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				fsm_current_state = INIT;
			}
			if (count1 <= 0 || count2 <= 0){
				count1 = GREEN_DUR;
				count2 = RED_DUR;
				fsm_current_state = GREEN_RED;
			}
			break;
		case GREEN_RED:
			displayGreen_Red();
			if(timerFlag(fsm_timer)){
				timerSet(fsm_timer, fsm_dur);
				count1--;
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressed(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				fsm_current_state = MAN_RED;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				fsm_current_state = INIT;
			}
			if (count1 <= 0){
				count1 = YELLOW_DUR;
				fsm_current_state = YELLOW_RED;
			}
			break;
		case YELLOW_RED:
			displayYellow_Red();
			if(timerFlag(fsm_timer)){
				timerSet(fsm_timer, fsm_dur);
				count1--;
				count2--;
			}
			displayNumbers(count1, count2);

			if (buttonIsPressed(mode_butt)){
				displayNone();
				tempRed = RED_DUR;
				tempGreen = GREEN_DUR;
				tempYellow = YELLOW_DUR;
				fsm_current_state = MAN_RED;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(set_butt)){
				GREEN_DUR = green_default;
				RED_DUR = red_default;
				YELLOW_DUR = yellow_default;
				fsm_current_state = INIT;
			}
			if (count1 <= 0 || count2 <= 0){
				count1 = RED_DUR;
				count2 = GREEN_DUR;
				fsm_current_state = RED_GREEN;
			}
			break;
		case MAN_RED:
			displayAllRed();
			if (buttonIsPressed(change_butt)){
				tempRed++;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(change_butt)){
				tempRed--;
				timerSet(time_out_timer, time_out_dur);
			}
			displayNumbers(tempRed, 2);

			if (buttonIsPressed(set_butt)){
				RED_DUR = tempRed;
				displayNone();
				fsm_current_state = MAN_GREEN;			//Khi set xong chuyển luôn sang manual tiếp theo
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsPressed(mode_butt)) {
				displayNone();
				fsm_current_state = MAN_GREEN;
				timerSet(time_out_timer, time_out_dur);
			}
			if (timerFlag(time_out_timer)){
				fsm_current_state = INIT;
			}
			break;
		case MAN_GREEN:
			displayAllGreen();
			if (buttonIsPressed(change_butt)){
				tempGreen++;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(change_butt)){
				tempGreen--;
				timerSet(time_out_timer, time_out_dur);
			}
			displayNumbers(tempGreen, 3);

			if (buttonIsPressed(set_butt)){
				GREEN_DUR = tempGreen;
				displayNone();
				fsm_current_state = MAN_YELLOW;
				timerSet(time_out_timer, time_out_dur);
			}

			if (buttonIsPressed(mode_butt)) {
				displayNone();
				fsm_current_state = MAN_YELLOW;
			}
			if (timerFlag(time_out_timer)){
				fsm_current_state = INIT;
			}
			break;
		case MAN_YELLOW:
			displayAllYellow();
			if (buttonIsPressed(change_butt)){
				tempYellow++;
				timerSet(time_out_timer, time_out_dur);
			}
			if (buttonIsHold(change_butt)){
				tempYellow--;
				timerSet(time_out_timer, time_out_dur);
			}
			displayNumbers(tempYellow, 4);

			if (buttonIsPressed(set_butt)){
				YELLOW_DUR = tempYellow;
				displayNone();
				fsm_current_state = INIT;
			}
			if (buttonIsPressed(mode_butt)){
				displayNone();
				fsm_current_state = INIT;
			}
			if (timerFlag(time_out_timer)){
				fsm_current_state = INIT;
			}
			break;
		default:
			break;
	}
}

