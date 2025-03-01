#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class PlotComponent final : public BaseComponent
	{
	public:
		PlotComponent(GameObject* gameObject);
		virtual ~PlotComponent() = default;

		virtual void Render() const override;

		void GeneratePlotData() const;
	private:
		int m_NumSamples;
		bool m_IsGeneratingData;
		std::vector<float> m_PlotValuesX;
		std::vector<float> m_PlotValuesY;
	};
}