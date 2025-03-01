// PlotComponent.h
#pragma once
#include "BaseComponent.h"
#include <vector>

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID;
};

namespace dae
{
	class PlotComponent final : public BaseComponent
	{
	public:
		PlotComponent(GameObject* gameObject);
		virtual ~PlotComponent() = default;

		virtual void Render()  override;

		void GeneratePlotData(int exercise = 0);
	private:
		int m_NumSamples1;
		int m_NumSamples2;
		bool m_IsGeneratingData;
		bool m_DataReady; // Flag to indicate if plot data is available.
		std::vector<float> m_PlotValuesX;
		std::vector<float> m_PlotValuesY;
	};
}