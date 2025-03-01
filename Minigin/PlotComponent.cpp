#include "PlotComponent.h"
#include "GameObject.h"
#include "imgui.h"
#include "imgui_plot.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <chrono>
#include <vector>

dae::PlotComponent::PlotComponent(GameObject* gameObject) :
	BaseComponent(gameObject)
	, m_NumSamples(10)
	, m_IsGeneratingData(false)
{
}

void dae::PlotComponent::Render() const
{
	ImGui::Begin("Exercise 2");

	int numSamples = m_NumSamples;
	ImGui::InputInt("##numsamples", &numSamples);
	if (ImGui::IsItemEdited())
	{
		m_NumSamples = numSamples;
		if (m_NumSamples < 1)
			m_NumSamples = 1;
		if (m_NumSamples > 10) // Limit to 10 steps as per original example
			m_NumSamples = 10;
	}
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		m_NumSamples--;
		if (m_NumSamples < 1)
			m_NumSamples = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		m_NumSamples++;
		if (m_NumSamples > 10) // Limit to 10 steps as per original example
			m_NumSamples = 10;
	}
	ImGui::SameLine();
	ImGui::Text("# samples");


	if (ImGui::Button("Trash the cache"))
	{
		GeneratePlotData();
	}

	if (m_IsGeneratingData)
	{
		ImGui::Text("Wait for it...");
	}
	else if (!m_PlotValuesX.empty())
	{
		ImGui::PlotConfig conf{};
		conf.values.xs = m_PlotValuesX.data();
		conf.values.ys = m_PlotValuesY.data();
		conf.values.count = m_PlotValuesX.size();
		conf.scale.min = 0;
		conf.scale.max = 20000; // Adjust max scale based on expected values, can be auto-scaled if needed
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.0f, y=%.0f us";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(400, 400);
		conf.line_thickness = 2.f;
		conf.x_ticks.show = true;
		conf.y_ticks.show = true;
		conf.x_axis.label = "Step Size";
		conf.y_axis.label = "Time (microseconds)";


		ImGui::Plot("cache_plot", conf);
	}

	ImGui::End();
}

void dae::PlotComponent::GeneratePlotData() const
{
	m_IsGeneratingData = true;
	m_PlotValuesX.clear();
	m_PlotValuesY.clear();

	std::vector<int> vec(67108864); // Allocates 2^26 integers on the heap
	for (int i = 0; i < 67108864; i++)
	{
		vec[i] = i;
	}

	int steps = 1;
	for (int step_index = 0; step_index < m_NumSamples; ++step_index)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < 67108864; i += steps)
		{
			vec[i] *= 2;
		}
		const auto end = std::chrono::high_resolution_clock::now();
		const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << steps << ": " << total << std::endl;
		m_PlotValuesX.push_back(static_cast<float>(steps));
		m_PlotValuesY.push_back(static_cast<float>(total));
		steps *= 2;
	}
	m_IsGeneratingData = false;
}