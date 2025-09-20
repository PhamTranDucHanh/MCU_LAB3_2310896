/*
 * button_optimize.c
 *
 *  Created on: Sep 20, 2025
 *      Author: HANH
 */

#include "button_optimize.h"

#define NO_OF_BUTTONS	3

#define DEBOUNCE_TIMER	0
#define HOLD1_TIMER		1
#define HOLD2_TIMER		2
#define HOLD3_TIMER		3

#define DEBOUNCE_DUR	10
#define HOLD_DUR		1000
#define Press			GPIO_PIN_RESET
#define Release			GPIO_PIN_SET


typedef enum{
	INIT,
	RELEASE,
	PRESS,
	PREHOLD,
	HOLD,
	POSTHOLD
} states;

states currentState [NO_OF_BUTTONS];

struct ButtonStruct
{
	GPIO_TypeDef *PORT;
	uint16_t PIN;

};

//int hold_timer[NO_OF_BUTTONS] = {HOLD1_TIMER, HOLD2_TIMER, HOLD3_TIMER};
struct ButtonStruct ButtonArray[NO_OF_BUTTONS];
GPIO_PinState validPress[NO_OF_BUTTONS];
GPIO_PinState filterPress1[NO_OF_BUTTONS];
GPIO_PinState filterPress2[NO_OF_BUTTONS];

void buttonsInitO(GPIO_TypeDef *PORT, uint16_t PIN){
	static int count = 0;
	if (count >= NO_OF_BUTTONS) return;
	ButtonArray[count].PORT = PORT;
	ButtonArray[count].PIN = PIN;
	currentState[count] = INIT;
	validPress[count] = Release;
	filterPress1[count] = Release;
	filterPress2[count] = Release;
	count++;
}

int verifyStateO(int i){
	if (timerFlag(DEBOUNCE_TIMER)){
		timerSet(DEBOUNCE_TIMER, DEBOUNCE_DUR);
		validPress[i] = filterPress1[i];
		filterPress1[i] = filterPress2[i];
		filterPress2[i] = HAL_GPIO_ReadPin(ButtonArray[i].PORT, ButtonArray[i].PIN);
	}
	if (validPress[i] == Press && filterPress1[i] == Press && filterPress2[i] == Press){
		return Press;
	}
	else if (validPress[i] == Release && filterPress1[i] == Release && filterPress2[i] == Release){
		return Release;
	}
	return 2;
}

void buttonsFSMO(){
	for (int i = 0; i < NO_OF_BUTTONS; i++){
		switch(currentState[i]){
			case INIT:
				timerSet(DEBOUNCE_TIMER, DEBOUNCE_DUR);

				currentState[i] = RELEASE;
				break;
			case RELEASE:

				if (verifyStateO(i) == Press){
					currentState[i] = PRESS;
				}
				break;
			case PRESS:
				timerSet(HOLD1_TIMER, HOLD_DUR);

				currentState[i] = PREHOLD;
				break;
			case PREHOLD:

				if (verifyStateO(i) == Release){
					currentState[i] = RELEASE;
				}
				if (timerFlag(HOLD1_TIMER) && verifyStateO(i) == Press){
					timerSet(HOLD1_TIMER, HOLD_DUR);
					currentState[i] = HOLD;
				}
				break;
			case HOLD:

				currentState[i] = POSTHOLD;
				break;
			case POSTHOLD:

				if (verifyStateO(i) == Release){
					currentState[i] = RELEASE;
				}
				if (timerFlag(HOLD1_TIMER) && verifyStateO(i) == Press){
					timerSet(HOLD1_TIMER, HOLD_DUR);
					currentState[i] = HOLD;
				}
				break;
			default:
				break;
		}
	}
}

int buttonIsPressedO(int index){
	if (currentState[index] == PRESS) return 1;
	else return 0;
}

int buttonIsHoldO(int index){
	if (currentState[index] == HOLD) return 1;
	else return 0;
}

