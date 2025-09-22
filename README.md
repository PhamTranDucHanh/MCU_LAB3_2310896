# MCU LAB3: Button/Switch & FSM
## Introduction

I am a student of the Microcontroller course at HCMUT. In this lab, I continued working with STM32CubeIDE and Proteus simulation, using the STM32F103C6 microcontroller. Lab 3 is structured to gradually build up from basic button debounce techniques, to implementing finite state machines (FSM), and finally integrating these concepts into a complete traffic light system with adjustable timing and operating modes. Through these exercises, I learned how to reliably handle button inputs, design and optimize state machines, and combine these modules into a flexible, real-world embedded application.

---

## Button Debouncing

To ensure accurate button press detection and eliminate false triggers caused by mechanical bouncing, each button uses a dedicated `software_timer` (reused from Lab 2) to sample the GPIO input every 10ms. The input signal (with pull-up configuration) is read and shifted through three consecutive variables: `filterPress2`, `filterPress1`, and `validPress`. Only when all three variables consistently register the same state (either pressed or released) is the button state considered stable and valid—meaning the button has maintained a consistent signal for at least 30ms.

This method provides robust debouncing by requiring three consecutive, identical readings before confirming a press or release event.

Reference implementation:
```c
int verifyStateO(int i){
    if (timerFlag(debounce_timer[i])){
        timerSet(debounce_timer[i], DEBOUNCE_DUR);
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
```

---

## Button State Machine (detect Press or Hold signal)
<img width="1233" height="446" alt="image" src="https://github.com/user-attachments/assets/12ac6c56-42cb-487b-922e-aa441eaf006f" />

The diagram above illustrates the finite state machine (FSM) used for robust button handling in this lab. The FSM transitions through several states to accurately detect and distinguish between short presses and long holds, while also filtering out noise and mechanical bouncing.

- **INIT**: Initial state, system setup.
- **RELEASE**: Button is not pressed.
- **PRESS**: Button is detected as pressed.
- **PRE-HOLD**: Button remains pressed; waiting to confirm a hold.
- **HOLD**: Button has been held for at least 1 second.
- **POST-HOLD**: Button continues to be held after the initial hold period.

Transitions between states are triggered by the result of `verifyStateO(i)`, which checks the current button input, and by timing conditions (e.g., holding for 1 second).  
- A transition from RELEASE to PRESS occurs when a press is detected.
- If the button remains pressed, the FSM moves to PRE-HOLD, then to HOLD after 1 second.
- Continuing to hold the button leads to POST-HOLD, with further transitions based on time and release events.
- Releasing the button at any stage returns the FSM to RELEASE.

**Edge-based signal detection:**  
In this implementation, both press and hold signals are detected as rising edges—meaning the event is triggered only once when the state changes from not pressed to pressed (for press), or from not held to held (for hold). This ensures that actions are performed only once per event, avoiding repeated triggers while the button remains pressed or held.

**Hold repeat principle:**  
After a hold is detected (button held for 1 second), if the button continues to be held, the FSM will generate a new hold edge every subsequent 1 second. This allows for repeated hold actions (e.g., auto-increment, scrolling) as long as the button remains pressed.

**Interface usage:**  
- `void buttonsFSMO1(int i);`, `void buttonsFSMO2(int i);`, `void buttonsFSMO3(int i);`  
  Call these functions regularly (e.g., in the main loop) to update the FSM for each button.
- `int buttonIsPressedO(int index);`  
  Returns `1` if a press event (edge) is detected for the specified button.
- `int buttonIsHoldO(int index);`  
  Returns `1` if a hold event (edge) is detected for the specified button (including repeated edges every 1 second if the button is continuously held).

By using these interfaces, your application can reliably respond to user input, distinguishing between short presses and long holds, and triggering actions only on the correct signal edges, including repeated hold events.

---

## Modifiable Traffic Light FSM
<img width="1611" height="913" alt="image" src="https://github.com/user-attachments/assets/1b12df30-31d9-4dc6-8d3f-a8cca99c82b4" />

The diagram above shows the finite state machine (FSM) structure used to implement a modifiable traffic light system in this lab. The FSM is designed to handle both automatic and manual modes, allowing the user to adjust the duration of each light (red, green, yellow) using button inputs.

**FSM States:**
- **INIT**: Initialization state, sets up default durations and prepares the system.
- **AUTO_RED / AUTO_GREEN / AUTO_YELLOW**: Automatic cycling through red, green, and yellow lights based on their respective durations. The system decrements counters and switches states when the timer expires.
- **MAN_RED / MAN_GREEN / MAN_YELLOW**: Manual adjustment states for each light. In these states, the user can increase or decrease the duration using the change button (press to increase, hold to decrease), and save the new value with the set button.

**Button Functions:**
- **Mode button**: Switches between automatic and manual modes when pressed.
- **Change button**: In manual mode, increases the duration when pressed and decreases it when held.
- **Set button**: Saves the current temporary value and moves to the next manual adjustment state.

**Transitions:**
- The FSM transitions between states based on button events and timer expirations. For example, pressing the mode button in any AUTO state switches to manual adjustment, while holding the set button in any state resets all durations to their default values.
- After adjusting and saving each duration in manual mode, the FSM moves to the next color or returns to INIT.

This modular FSM approach makes the traffic light system flexible and user-friendly, allowing real-time adjustment of light durations and seamless switching between automatic and manual operation. The code structure closely follows the state diagram, with clear separation of logic for each state and event, ensuring maintainability and scalability for future enhancements.

---

## Proteus Schematic Image

The image below shows the Proteus schematic for the modifiable traffic light system implemented in this lab. The circuit is fully pre-wired, including the STM32 microcontroller, buttons, and display components, allowing all exercises to be tested and the traffic light durations to be adjusted as required.<!-- Replace the link below with your actual schematic image or a relative path to the image in your Proteus folder -->
<img width="1186" height="687" alt="image" src="https://github.com/user-attachments/assets/37bd680b-5510-4d58-a951-35f22d622016" />


---

## .ioc File Image (STM32CubeMX)

All necessary connections are defined and fully synchronized with the Proteus schematic above, ensuring consistency between the hardware simulation and the project setup.
<!-- Replace the link below with your actual .ioc screenshot or a relative path to the image -->
<img width="758" height="656" alt="image" src="https://github.com/user-attachments/assets/67b36af2-60d7-4cd2-acf6-890513df29a4" />


---

## Source Folder Structure

The main code for each exercise and module is organized in the `Core/Src/` directory as follows:
```text
Core/
│
├── Src/                        # Main embedded source code
│   ├── button.c                # Basic button input handling
│   ├── button_optimize.c       # Optimized button input handling
│   ├── display.c               # Display output module
│   ├── lab3_fsm.c              # Finite state machine logic
│   ├── software_timer.c        # Common software timer module
│   ├── main.c                  # Entry point, macro to select exercise
│   ├── stm32f1xx_hal_msp.c     # HAL MSP init
│   ├── stm32f1xx_it.c          # Interrupt handlers
│   ├── syscalls.c              # System call stubs
│   ├── sysmem.c                # System memory allocation
│   └── system_stm32f1xx.c      # System clock/config
│
├── Inc/                        # Header files for each module and main
│   ├── button.h
│   ├── button_optimize.h
│   ├── display.h
│   ├── lab3_fsm.h
│   ├── software_timer.h
│   ├── main.h
│   ├── stm32f1xx_hal_conf.h
│   └── stm32f1xx_it.h
│
└── Startup/
    └── startup_stm32f103c6ux.s   # Startup assembly code
