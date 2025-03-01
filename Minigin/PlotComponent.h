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

		void RenderExercise1();
		void RenderExercise2();

		void GeneratePlotData(int exercise = 0);
	private:
		int m_NumSamples1;
		int m_NumSamples2;
		bool m_IsGeneratingData;
		bool m_IsGeneratingDataGO;
		bool m_IsGeneratingDataAlt;
		bool m_DataReady; // Flag to indicate if plot data is available.
		bool m_DataReadyGO;
		bool m_DataReadyAlt;
		bool m_BothPlotsGenerated;
		std::vector<float> m_PlotValuesX;
		std::vector<float> m_PlotValuesY;
		std::vector<float> m_PlotValuesY_GO;   // Y values for GameObject3D
		std::vector<float> m_PlotValuesY_Alt;  // Y values for GameObject3DAlt
	};
}