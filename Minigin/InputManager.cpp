#include <SDL.h>
#include "InputManager.h"
#include <iostream>

using namespace dae;

bool InputManager::ProcessInput()
{
	//Keyboard part
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			break;
		}

		//Keyboard OnPress and OnRelease
		for (auto& mapPair : m_KeyboardActionMap)
		{
			if (unsigned int(e.key.keysym.sym) == mapPair.first.key)
			{
				if (e.type == SDL_KEYDOWN)
				{
					if (mapPair.first.type == InputType::OnPress)
					{
						mapPair.second->Execute();
						std::cout << "onpress keydown" << std::endl;
					}
				}
				else if (mapPair.first.type == InputType::OnRelease && e.type == SDL_KEYUP)
				{
					mapPair.second->Execute();
				}
			}
		}
	}

	//Keyboard OnHold
	const Uint8* state = SDL_GetKeyboardState(NULL);
	for (auto& mapPair : m_KeyboardActionMap)
	{
		if (mapPair.first.type == InputType::OnHold)
		{
			if (state[SDL_GetScancodeFromKey(mapPair.first.key)])
			{
				mapPair.second->Execute();
			}
		}
	}


	// Controller
	for (auto& controller : m_pControllers)
	{
		controller->Update();

		for (auto& mapPair : m_ControllerActionMap)
		{
			if (mapPair.first.controllerID == controller->GetIdx())
			{
				if (mapPair.first.type == InputType::OnPress && controller->IsDown(mapPair.first.button))
				{
					mapPair.second->Execute();
				}
				else if (mapPair.first.type == InputType::OnHold && controller->IsPressed(mapPair.first.button))
				{
					mapPair.second->Execute();
				}
				else if (mapPair.first.type == InputType::OnRelease && controller->IsUp(mapPair.first.button))
				{
					mapPair.second->Execute();
				}
			}
		}
	}

	return true;
}

XBoxController* InputManager::GetController(unsigned int controllerIdx)
{
	for (auto& controller : m_pControllers)
	{
		if (controller->GetIdx() == controllerIdx)
		{
			return controller.get();
		}
	}
	return nullptr;
}


void InputManager::AddControllerCommand(XBoxController::XBoxButton button, unsigned int controllerID, InputType type, std::unique_ptr<Command> pCommand)
{
	bool doesControllerExist{ false };
	for (const auto& controller : m_pControllers)
	{
		if (controller->GetIdx() == controllerID)
		{
			doesControllerExist = true;
			break;
		}
	}

	if (doesControllerExist == false)
	{
		//make new controller
		m_pControllers.push_back(std::make_unique<XBoxController>(controllerID));
	}

	//make the action and add it to the map
	InputDataController inputData{ controllerID, button, type };
	m_ControllerActionMap[inputData] = std::move(pCommand);
}

void InputManager::AddKeyboardCommand(unsigned int keyboardKey, InputType type, std::unique_ptr<Command> pCommand)
{
	//make the action and add it to the map
	InputDataKeyboard inputData{ keyboardKey, type };
	m_KeyboardActionMap[inputData] = std::move(pCommand);
}