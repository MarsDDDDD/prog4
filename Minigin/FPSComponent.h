#pragma once
#include "BaseComponent.h"
#include <vector>
#include <memory> // For weak_ptr

namespace dae
{
	class TextComponent; // Forward declare TextComponent
	class GameObject;

	class FPSComponent : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pOwner); // Add GameObject* to constructor
		~FPSComponent() override = default;

		void Update(float deltaTime) override;

		float GetFPS() const;
	private:
		//Use weak_ptr to fix dependency and improve efficiency
		std::weak_ptr<TextComponent> m_textComponent{};

		float m_fps;
		float m_timeAccumulator;
		int m_frameCount;
		std::vector<float> m_frameTimes; // To store the times of the last 'x' frames
		static constexpr size_t m_maxSamples = 100;  //Store 100 samples, average those

	};
}