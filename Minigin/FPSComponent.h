#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class FPSComponent : public BaseComponent
	{
	public:
		FPSComponent();
		~FPSComponent() override = default;

		void Update(float deltaTime) override;

		float GetFPS() const;

	private:
		float m_fps;
		float m_timeAccumulator;
		int m_frameCount;
		std::vector<float> m_frameTimes; // To store the times of the last 'x' frames
		static constexpr size_t m_maxSamples = 100;  //Store 100 samples, average those

	};
}