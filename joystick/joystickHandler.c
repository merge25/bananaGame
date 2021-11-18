/*
 *	Source for handling the joystick input
 */

/// Includes
	#include "joystickHandler.h"	//own header

	#include "../../../joystickPosition.h"	//an external header, containing joystick's data
	#include "em_gpio.h"	//for abstract GPIO handling
	#include "../input_handler/inputHandler.h"		//for the IT flag structure

/// Constants

	// The pin's location, on which data active joystick signal is received
		const GPIO_Port_TypeDef JOYSTICK_ACTIVITY_GPIO_PORT = gpioPortD;
		const unsigned int JOYSTICK_ACTIVITY_GPIO_PIN = 0;
		//(PD0 is pin 4 on Expansion Header)

	// The pin's location, on which data is received from the joystick
		const GPIO_Port_TypeDef JOYSTICK_DATAIN_GPIO_PORT = gpioPortD;
		const unsigned int JOYSTICK_DATAIN_GPIO_PIN = 1;
		//(PD2 is pin 8 on Expansion Header)
	//(the 2 pins are set to even pins, to have the same IT route for them)

/// Global variables
	extern struct InputITFlags inputITFlags;		//a global flag structure, which signs, if a change occurred in inputs

/// Main functions

	// Reads and calculates the new value of joystick's position
	// (this function is called, if an IT showed, that the joystick input is changed)
	JoystickPosition readAndCalculateNewJoystickPosition(void) {

		//check, if the joystick is active (activity pin is in active state)
		if (GPIO_PinInGet(JOYSTICK_ACTIVITY_GPIO_PORT, JOYSTICK_ACTIVITY_GPIO_PIN) == JOYSTICK_ACTIVE) {

			//check, that the active joystick is in left state
			if (GPIO_PinInGet(JOYSTICK_DATAIN_GPIO_PORT, JOYSTICK_DATAIN_GPIO_PIN) == JOYSTICK_LEFT)
				return JOYSTICK_LEFT;
			else 	//if it's not in left state, then it's in right state
				return JOYSTICK_RIGHT;
		}
		else 	//if the joystick is not active
			return JOYSTICK_DEFAULT;

		//unset the IT's showing flag
		inputITFlags.isJoystickChanged = 0;
	}

	// Configure the GPIO ports, used by the joystick input
	void configJoystickGPIO(void) {

		// Set the used pins (see at Constants) as standard inputs
			GPIO_PinModeSet(JOYSTICK_ACTIVITY_GPIO_PORT, JOYSTICK_ACTIVITY_GPIO_PIN, gpioModeInput, false);
			GPIO_PinModeSet(JOYSTICK_DATAIN_GPIO_PORT, JOYSTICK_DATAIN_GPIO_PIN, gpioModeInput, false);

		// Set IT to come, as any of the joystick input pins changes
		//(so enable them, and set them to be sensitive to both rising and falling edges)
			GPIO_IntConfig(JOYSTICK_ACTIVITY_GPIO_PORT, JOYSTICK_ACTIVITY_GPIO_PIN, 1, 1, 1);
			GPIO_IntConfig(JOYSTICK_DATAIN_GPIO_PORT, JOYSTICK_DATAIN_GPIO_PIN, 1, 1, 1);
	}

	// Handle the IT, which comes, if any of the joystick input pins changes
	// (both of them are connected to even pins)
	void GPIO_EVEN_IRQHandler(void) {

		// Only the joystick input pins' ITs are enables,
		// so it's sure, that it gave it.

		// So then, set the IT flag for joystick
		inputITFlags.isJoystickChanged = 1;
	}
