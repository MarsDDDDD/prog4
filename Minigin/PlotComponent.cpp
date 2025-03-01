// PlotComponent.cpp
#include "PlotComponent.h"
#include "GameObject.h"
#include "imgui.h"
#include "imgui_plot.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric> // For std::accumulate
#include <ranges>
#include <algorithm>

dae::PlotComponent::PlotComponent(GameObject* gameObject) :
	BaseComponent(gameObject)
	, m_NumSamples1(10)
	, m_IsGeneratingData(false)
	, m_DataReady(false)
{
}

void dae::PlotComponent::Render() 
{
	ImGui::Begin("Exercise 1");
	ImGui::InputInt("##numsamples", &m_NumSamples1);
	bool numSamplesChanged = ImGui::IsItemEdited(); // Check if edited *before* the buttons.
	ImGui::SameLine();
	ImGui::Text("# samples");

	if (numSamplesChanged)
	{
		if (m_NumSamples1 < 1)
			m_NumSamples1 = 1;
	}



	if (ImGui::Button("Trash the cache"))
	{
		GeneratePlotData();
	}

	if (m_IsGeneratingData)
	{
		ImGui::Text("Wait for it...");
	}
	else if (m_DataReady) // Use a flag to indicate data is ready.
	{
		ImGui::PlotConfig conf{};
		conf.values.xs = m_PlotValuesX.data();
		conf.values.ys = m_PlotValuesY.data();
        conf.values.count = static_cast<int>(m_PlotValuesX.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_PlotValuesY.begin(),m_PlotValuesY.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.0f, y=%.0f us";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(400, 400);
		conf.line_thickness = 2.f;

		ImGui::Plot("plot1", conf);
	}

	ImGui::End();
}

void dae::PlotComponent::GeneratePlotData(int exerciseNr)
{
	m_IsGeneratingData = true;
	m_DataReady = false; // Reset the flag.
	m_PlotValuesX.clear();
	m_PlotValuesY.clear();

	// Run the test multiple times and average the results
	std::vector<std::vector<float>> all_runs_y;

	int steps_max = 1024;
	int steps_count = 0;
	for (int temp_step = 1; temp_step <= steps_max; temp_step *= 2)
	{
		steps_count++;
	}


	for (int sample = 0; sample < m_NumSamples1; ++sample)
	{
		std::vector<int> vecFloat(67108864); // Allocates 2^26 integers on the heap
		std::vector<GameObject3D> vecGO(67108864);
		std::vector<GameObject3DAlt> vecAlt(67108864);

		switch (exerciseNr)
		{
		case 0:
			for (int i = 0; i < 67108864; i++)
			{
				vecFloat[i] = i;
			}
			break;

		case 1:
			for (int i = 0; i < 67108864; i++)
			{
				vecGO[i].transform = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
				vecGO[i].ID = i;
			}
			break;

		case 2:
			for (int i = 0; i < 67108864; i++)
			{
				vecAlt[i].transform = new Transform();
				vecAlt[i].ID = i;
			}
			break;
		}

		std::vector<float> run_results_y;
		int steps = 1;
		int step_idx = 0;

		do
		{

			const auto start = std::chrono::high_resolution_clock::now();
			switch (exerciseNr)
			{
			case 0:

				for (int i = 0; i < 67108864; i += steps)
				{
					vecFloat[i] *= 2;
				}
				break;

			case 1:
				for (int i = 0; i < 67108864; i++)
				{
					vecGO[i].ID *= 2;
				}
				break;

			case 2:
				for (int i = 0; i < 67108864; i++)
				{
					vecAlt[i].ID *= 2;
				}
				break;
			}
			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			if (sample == 0)
				m_PlotValuesX.push_back(static_cast<float>(steps)); // Only need to store X values once.
			run_results_y.push_back(static_cast<float>(total));
			steps *= 2;
			step_idx++;
		} while (steps <= steps_max);

		all_runs_y.push_back(run_results_y);
	}


	// Average the results
	for (int i = 0; i < steps_count; ++i)
	{
		float sum = 0.0f;
		for (const auto& run : all_runs_y)
		{
			sum += run[i];
		}
		m_PlotValuesY.push_back(sum / static_cast<float>(m_NumSamples1));
	}


	m_IsGeneratingData = false;
	m_DataReady = true; // Set the flag when data is ready.
}
