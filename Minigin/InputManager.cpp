#include <SDL.h>
#include "InputManager.h"
#include <iostream>

using namespace dae;

bool InputManager::ProcessInput(float deltaTime)
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
						if (m_HeldKeys.find(mapPair.first.key) == m_HeldKeys.end())
						{
							mapPair.second->Execute(deltaTime);
							m_HeldKeys.insert(mapPair.first.key);
						}
					}
				}
				else if (e.type == SDL_KEYUP)
				{
					std::cout << "Release" << std::endl;
					m_HeldKeys.erase(mapPair.first.key);
					if (mapPair.first.type == InputType::OnRelease)
					{
						mapPair.second->Execute(deltaTime);
					}
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
				mapPair.second->Execute(deltaTime);
			}
		}
	}


	// Controller
	for (auto& controller : m_pControllers)
	{
		controller->Update();

		for (auto& mapPair : m_ControllerActionMap)
		{
			if (mapPair.first.controllerID == controller->GetIndex())
			{
				if (mapPair.first.type == InputType::OnPress && controller->IsDown(mapPair.first.button))
				{
					ControllerButtonState buttonState{ mapPair.first.controllerID, mapPair.first.button };
					if (m_HeldButtons.find(buttonState) == m_HeldButtons.end())
					{
						mapPair.second->Execute(deltaTime);
						m_HeldButtons.insert(buttonState);
					}
				}
				else if (mapPair.first.type == InputType::OnHold && controller->IsPressed(mapPair.first.button))
				{
					mapPair.second->Execute(deltaTime);
				}
				else if (mapPair.first.type == InputType::OnRelease && controller->IsUp(mapPair.first.button))
				{
					mapPair.second->Execute(deltaTime);
					ControllerButtonState buttonState{ mapPair.first.controllerID, mapPair.first.button };
					m_HeldButtons.erase(buttonState);
				}
			}
		}
	}

	return true;
}

XBoxController* InputManager::GetController(unsigned int controllerIndex)
{
	for (auto& controller : m_pControllers)
	{
		if (controller->GetIndex() == controllerIndex)
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
		if (controller->GetIndex() == controllerID)
		{
			doesControllerExist = true;
			break;
		}
	}

	if (doesControllerExist == false)
	{
		m_pControllers.emplace_back(std::make_unique<XBoxController>(controllerID));
	}

	InputDataController inputData{ controllerID, button, type };
	m_ControllerActionMap[inputData] = std::move(pCommand);
}

void InputManager::AddKeyboardCommand(unsigned int key, InputType type, std::unique_ptr<Command> pCommand)
{
	InputDataKeyboard inputData{ key, type };
	m_KeyboardActionMap[inputData] = std::move(pCommand);
}