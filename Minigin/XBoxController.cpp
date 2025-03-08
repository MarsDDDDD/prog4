#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

#include "XBoxController.h"

using namespace dae;

class XBoxController::XBoxControllerImpl final
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame;
	WORD buttonsReleasedThisFrame;

	int m_ControllerIndex;

public:
	XBoxControllerImpl(int controllerIndex)
		:m_ControllerIndex{ controllerIndex }
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &currentState);

		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const { return buttonsPressedThisFrame & button; }
	bool IsUpThisFrame(unsigned int button) const { return buttonsReleasedThisFrame & button; }
	bool IsPressed(unsigned int button) const { return currentState.Gamepad.wButtons & button; }

	glm::vec2 GetLeftStickPos() const { return glm::vec2{ currentState.Gamepad.sThumbLX, currentState.Gamepad.sThumbLY }; }
};



XBoxController::XBoxController(unsigned int controllerIndex)
	: m_ControllerIndex{ controllerIndex }
{
	pImpl = new XBoxControllerImpl(controllerIndex);
}

XBoxController::~XBoxController()
{
	delete pImpl;
}

void XBoxController::Update()
{
	pImpl->Update();
}

bool XBoxController::IsDown(XBoxButton button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool XBoxController::IsUp(XBoxButton button) const
{
	return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool XBoxController::IsPressed(XBoxButton button) const
{
	return pImpl->IsPressed(static_cast<unsigned int>(button));
}

glm::vec2 XBoxController::GetLeftStickPos() const
{
	return pImpl->GetLeftStickPos();
}

