# MCU LAB3: Button/Switch & FSM
## Introduction

I am a student of the Microcontroller course at HCMUT. In this lab, I continued working with STM32CubeIDE and Proteus simulation, using the STM32F103C6 microcontroller. Lab 3 is structured to gradually build up from basic button debounce techniques, to implementing finite state machines (FSM), and finally integrating these concepts into a complete traffic light system with adjustable timing and operating modes. Through these exercises, I learned how to reliably handle button inputs, design and optimize state machines, and combine these modules into a flexible, real-world embedded application.

---

## Demo video (Google Drive link)
https://drive.google.com/file/d/1_mhcp9KD0Gh-E9pva6tFR-ycQ44c0vx_/view?usp=sharing

---

## Button Debouncing

To ensure accurate button press detection and eliminate false triggers caused by mechanical bouncing, each button uses a dedicated `software_timer` (reused from Lab 2) to sample the GPIO input every 10ms. The input signal (with pull-up configuration) is read and shifted through three consecutive variables: `filterPress2`, `filterPress1`, and `validPress`. Only when all three variables consistently register the same state (either pressed or released) is the button state considered stable and valid—meaning the button has maintained a consistent signal for at least 30ms.

This method provides robust debouncing by requiring three consecutive, identical readings before confirming a press or release event.

Reference implementation:
```c
int verifyState(int i){
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

Transitions between states are triggered by the result of `verifyState(i)`, which checks the current button input, and by timing conditions (e.g., holding for 1 second).  
- A transition from RELEASE to PRESS occurs when a press is detected.
- If the button remains pressed, the FSM moves to PRE-HOLD, then to HOLD after 1 second.
- Continuing to hold the button leads to POST-HOLD, with further transitions based on time and release events.
- Releasing the button at any stage returns the FSM to RELEASE.

**Edge-based signal detection:**  
In this implementation, both press and hold signals are detected as rising edges—meaning the event is triggered only once when the state changes from not pressed to pressed (for press), or from not held to held (for hold). This ensures that actions are performed only once per event, avoiding repeated triggers while the button remains pressed or held.

**Hold repeat principle:**  
After a hold is detected (button held for 1 second), if the button continues to be held, the FSM will generate a new hold edge every subsequent 1 second. This allows for repeated hold actions (e.g., auto-increment, scrolling) as long as the button remains pressed.

**Interface usage:**  
- `void buttonsFSM1(int i);`, `void buttonsFSM2(int i);`, `void buttonsFSM3(int i);`  
  Call these functions regularly (e.g., in the main loop) to update the FSM for each button.
- `int buttonIsPressed(int index);`  
  Returns `1` if a press event (edge) is detected for the specified button.
- `int buttonIsHold(int index);`  
  Returns `1` if a hold event (edge) is detected for the specified button (including repeated edges every 1 second if the button is continuously held).

By using these interfaces, your application can reliably respond to user input, distinguishing between short presses and long holds, and triggering actions only on the correct signal edges, including repeated hold events.

---


##  Modifiable Traffic Light FSM

<img width="1361" height="766" alt="image" src="https://github.com/user-attachments/assets/3ab53480-34c6-45af-af34-456c1fb81c73" />

### States:
1. **INIT**:
   - Initialization state.
   - Sets up default durations for red, green, and yellow lights.
   - Prepares the system for operation.
   - Transitions to `RED_GREEN` state after initialization.

2. **RED_GREEN**:
   - Automatic mode: Red light for one direction, green light for the other.
   - Decrements counters (`count1` for red, `count2` for green) based on timer expiration.
   - Transitions:
     - To `RED_YELLOW` when the green light counter (`count2`) reaches zero.
     - To `MAN_RED` when the mode button is pressed (enters manual adjustment).
     - To `INIT` when the set button is held (resets durations to default).

3. **RED_YELLOW**:
   - Automatic mode: Red light for one direction, yellow light for the other.
   - Decrements counters (`count1` for red, `count2` for yellow) based on timer expiration.
   - Transitions:
     - To `GREEN_RED` when counters reach zero.
     - To `MAN_RED` when the mode button is pressed.
     - To `INIT` when the set button is held.

4. **GREEN_RED**:
   - Automatic mode: Green light for one direction, red light for the other.
   - Decrements counters (`count1` for green, `count2` for red) based on timer expiration.
   - Transitions:
     - To `YELLOW_RED` when the green light counter (`count1`) reaches zero.
     - To `MAN_RED` when the mode button is pressed.
     - To `INIT` when the set button is held.

5. **YELLOW_RED**:
   - Automatic mode: Yellow light for one direction, red light for the other.
   - Decrements counters (`count1` for yellow, `count2` for red) based on timer expiration.
   - Transitions:
     - To `RED_GREEN` when counters reach zero.
     - To `MAN_RED` when the mode button is pressed.
     - To `INIT` when the set button is held.

6. **MAN_RED** (Mode 2):
   - Manual mode: Adjust the duration of the red light.
   - User can:
     - Increase duration using the change button (press).
     - Decrease duration using the change button (hold).
     - Save the new duration using the set button.
   - Transitions:
     - To `MAN_GREEN` after saving the red light duration.
     - To `MAN_GREEN` when the mode button is pressed.
     - To `CHECK_DUR` if the timeout timer expires.

7. **MAN_GREEN** (Mode 3):
   - Manual mode: Adjust the duration of the green light.
   - User can:
     - Increase duration using the change button (press).
     - Decrease duration using the change button (hold).
     - Save the new duration using the set button.
   - Transitions:
     - To `MAN_YELLOW` after saving the green light duration.
     - To `MAN_YELLOW` when the mode button is pressed.
     - To `CHECK_DUR` if the timeout timer expires.

8. **MAN_YELLOW** (Mode 4):
   - Manual mode: Adjust the duration of the yellow light.
   - User can:
     - Increase duration using the change button (press).
     - Decrease duration using the change button (hold).
     - Save the new duration using the set button.
   - Transitions:
     - To `CHECK_DUR` after saving the yellow light duration.
     - To `CHECK_DUR` when the mode button is pressed.
     - To `CHECK_DUR` if the timeout timer expires.

9. **CHECK_DUR**:
   - State to verify the set of durations entered by the user before using them in automatic mode.
   - All LEDs and 7-segment displays are turned off to indicate checking.
   - If the durations do not satisfy the constraints:
     - `RED_DUR >= 3`
     - `GREEN_DUR >= 2`
     - `YELLOW_DUR >= 1`
     - `RED_DUR - GREEN_DUR == YELLOW_DUR`
     then the previous valid durations are restored.
   - After 1 second, transitions to `INIT` to start automatic mode with the checked durations.

### Transitions:
- **Mode Button**: Switches between automatic and manual modes.
- **Change Button**: Adjusts the duration of the current light in manual mode.
- **Set Button**: Saves the current duration and moves to the next manual adjustment state.
- **Timeout Timer**: If no user input is detected for a predefined duration in manual mode, transitions to `CHECK_DUR` before returning to automatic mode.

**Note:** All transitions from `MAN_YELLOW` (Mode 4) or any timeout in manual modes will always go through `CHECK_DUR` before returning to automatic mode. This ensures the system checks and validates the durations before using them.

### Button Functions

1. **Mode Button**:
   - Changes between automatic and manual modes when pressed.

2. **Change Button**:
   - Increases the duration when pressed.
   - Decreases the duration when held.

3. **Set Button**:
   - Saves the current temporary value.
   - Moves to the next manual adjustment state.

### Default Durations
- **Red Light**: 5 seconds.
- **Green Light**: 3 seconds.
- **Yellow Light**: 2 seconds.

**Note**: While the system is running in automatic mode, if the **Set button** is held (pressed and held down), the durations for all lights will reset to their default values (Red: 5 seconds, Green: 3 seconds, Yellow: 2 seconds), and the FSM will transition back to the `INIT` state.

### Features
- **Automatic Mode**: Cycles through red, green, and yellow lights based on their durations.
- **Manual Mode**: Allows real-time adjustment of light durations.
- **CHECK_DUR State**: Validates the set durations before using them in automatic mode. All LEDs and 7-segment displays are off during this state.
- **Timeout Mechanism**: Returns to `CHECK_DUR` if no user input is detected in manual mode, then transitions to automatic mode.

### Constraints for Durations
- `RED_DUR >= 3`
- `GREEN_DUR >= 2`
- `YELLOW_DUR >= 1`
- `RED_DUR - GREEN_DUR == YELLOW_DUR`

If these constraints are not met, the system will restore the previous valid durations.

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
