#include <SDL.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	// Example code:
	//CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
	//ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	//XInputGetState(m_controllerIndex, &currentState);
	//auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	//buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
	//buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...
	}

	return true;
}
