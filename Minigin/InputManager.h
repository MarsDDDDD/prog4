#pragma once
#include <map>
#include <memory>
#include <set>

#include "Singleton.h"
#include "XBoxController.h"
#include "Command.h"

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);
		XBoxController* GetController(unsigned int controllerIndex);

		enum class InputType
		{
			OnPress,
			OnHold,
			OnRelease
		};

		void AddControllerCommand(XBoxController::XBoxButton button, unsigned int controllerID, InputType type, std::unique_ptr<Command> pCommand);
		void AddKeyboardCommand(unsigned int keyboardKey, InputType type, std::unique_ptr<Command> pCommand);

	private:
		struct InputDataController
		{
			unsigned int controllerID{};
			XBoxController::XBoxButton button{};
			InputType type{};

			// Custom less-than operator for std::map
			bool operator<(const InputDataController& other) const
			{
				if (controllerID < other.controllerID) return true;
				if (controllerID > other.controllerID) return false;

				if (button < other.button) return true;
				if (button > other.button) return false;

				return type < other.type;
			}
		};

		struct InputDataKeyboard
		{
			unsigned int key{};
			InputType type{};

			// Custom less-than operator for std::map
			bool operator<(const InputDataKeyboard& other) const
			{
				if (key < other.key) return true;
				if (key > other.key) return false;

				return type < other.type;
			}
		};

		std::map<InputDataController, std::unique_ptr<Command>> m_ControllerActionMap{};
		std::vector<std::unique_ptr<XBoxController>> m_pControllers{};

		std::map<InputDataKeyboard, std::unique_ptr<Command>> m_KeyboardActionMap{};
        
		// Track keys that are currently held down to prevent repeated OnPress events
		std::set<unsigned int> m_HeldKeys{};
        
		// Track controller buttons that are currently held down
		struct ControllerButtonState {
			unsigned int controllerID{};
			XBoxController::XBoxButton button{};
            
			bool operator<(const ControllerButtonState& other) const {
				if (controllerID < other.controllerID) return true;
				if (controllerID > other.controllerID) return false;
				return button < other.button;
			}
		};
		std::set<ControllerButtonState> m_HeldButtons{};
	};
}
