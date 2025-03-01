// PlotComponent.h
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

		virtual void Render()  override;

		void GeneratePlotData(); 
	private:
		int m_NumSamples;
		bool m_IsGeneratingData;
		bool m_DataReady; // Flag to indicate if plot data is available.
		std::vector<float> m_PlotValuesX;
		std::vector<float> m_PlotValuesY;
	};
}