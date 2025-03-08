#pragma once
#include <glm.hpp>

namespace dae
{
	class XBoxController final
	{
	private:
		class XBoxControllerImpl;
		XBoxControllerImpl* pImpl{};

		unsigned int m_ControllerIndex{};

	public:
		enum class XBoxButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftStickClick = 0x0040,
			RightStickClick = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000,
			LeftStick
		};

		void Update();

		bool IsDown(XBoxButton button) const;
		bool IsUp(XBoxButton button) const;
		bool IsPressed(XBoxButton button) const;

		glm::vec2 GetLeftStickPos() const;

		unsigned int GetIndex() const { return m_ControllerIndex; }

		explicit XBoxController(unsigned int controllerIndex);
		~XBoxController();

	};
}

