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

dae::PlotComponent::PlotComponent(GameObject * gameObject) :
    BaseComponent(gameObject)
    , m_NumSamples1(10)
    , m_NumSamples2(10)
    , m_IsGeneratingData(false)
    , m_IsGeneratingDataGO(false)
    , m_IsGeneratingDataAlt(false)
    , m_DataReady(false)
    , m_DataReadyGO(false)
    , m_DataReadyAlt(false)
    , m_BothPlotsGenerated(false)
{
}

void dae::PlotComponent::Render()
{
    RenderExercise1();
    RenderExercise2();
}

void dae::PlotComponent::RenderExercise1()
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



    if (ImGui::Button("Thrash the cache"))
    {
        GeneratePlotData(0);
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
        conf.scale.max = *std::max_element(m_PlotValuesY.begin(), m_PlotValuesY.end());
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.0f us";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(300, 200);
        conf.line_thickness = 2.f;

        ImGui::Plot("plot1", conf);
    }

    ImGui::End();
}

void dae::PlotComponent::RenderExercise2()
{
    ImGui::Begin("Exercise 2");
    ImGui::InputInt("##numsamples2", &m_NumSamples2);
    bool numSamplesChanged = ImGui::IsItemEdited();
    ImGui::SameLine();
    ImGui::Text("# samples");


    if (numSamplesChanged)
    {
        if (m_NumSamples2 < 1)
            m_NumSamples2 = 1;
    }

    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {
        GeneratePlotData(1);
    }

    if (m_IsGeneratingDataGO)
    {
        ImGui::Text("Wait for it... (GameObject3D)");
    }
    else if (m_DataReadyGO)
    {
        ImGui::PlotConfig conf{};
        conf.values.xs = m_PlotValuesX.data(); // Assuming X values are the same
        conf.values.ys = m_PlotValuesY_GO.data();
        conf.values.count = static_cast<int>(m_PlotValuesX.size());
        conf.scale.min = 0;
        conf.scale.max = *std::max_element(m_PlotValuesY_GO.begin(), m_PlotValuesY_GO.end());
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.0f us";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(300, 200);
        conf.line_thickness = 2.f;

        ImGui::Plot("GameObject3D Plot", conf);
    }


    if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
    {
        GeneratePlotData(2);
    }

    if (m_IsGeneratingDataAlt)
    {
        ImGui::Text("Wait for it... (GameObject3DAlt)");
    }
    else if (m_DataReadyAlt)
    {
        ImGui::PlotConfig conf{};
        conf.values.xs = m_PlotValuesX.data();  // Assuming X values are the same
        conf.values.ys = m_PlotValuesY_Alt.data();
        conf.values.count = static_cast<int>(m_PlotValuesX.size());
        conf.scale.min = 0;
        conf.scale.max = *std::max_element(m_PlotValuesY_Alt.begin(), m_PlotValuesY_Alt.end());
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.0f us";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(300, 200);
        conf.line_thickness = 2.f;

        ImGui::Plot("GameObject3DAlt Plot", conf);
    }
    if (m_BothPlotsGenerated)
    {

        float maxY = std::max(*std::max_element(m_PlotValuesY_GO.begin(), m_PlotValuesY_GO.end()),
            *std::max_element(m_PlotValuesY_Alt.begin(), m_PlotValuesY_Alt.end()));

        ImGui::PlotConfig conf{};
        conf.values.xs = m_PlotValuesX.data();
        conf.values.ys_list = new const float* [2];  
        conf.values.ys_list[0] = m_PlotValuesY_GO.data();  // First series: GameObject3D
        conf.values.ys_list[1] = m_PlotValuesY_Alt.data(); // Second series: GameObject3DAlt
        conf.values.ys_count = 2;  
        conf.values.count = static_cast<int>(m_PlotValuesX.size()); 
        conf.values.colors = new ImU32[2]{ IM_COL32(255, 0, 0, 255),IM_COL32(0, 255, 0, 255) };
        conf.scale.min = 0; 
        conf.scale.max = maxY;
        conf.tooltip.show = true; 
        conf.tooltip.format = "x=%.0f, y=%.0f us";
        conf.grid_x.show = true; 
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(300, 200);  
        conf.line_thickness = 2.f;

        ImGui::Plot("Combined Plot", conf); 

        delete[] conf.values.ys_list;
        delete[] conf.values.colors;
    }

    ImGui::End();
}

void dae::PlotComponent::GeneratePlotData(int exerciseNr)
{

    m_PlotValuesX.clear();

    // Run the test multiple times and average the results
    std::vector<std::vector<float>> all_runs_y;

    int steps_max = 1024;
    int steps_count = 0;
    for (int temp_step = 1; temp_step <= steps_max; temp_step *= 2)
    {
        steps_count++;
    }
    if (exerciseNr == 0)
    {
        m_IsGeneratingData = true;
        m_DataReady = false; // Reset the flag.
        m_PlotValuesY.clear();
    }
    else if (exerciseNr == 1)
    {
        m_IsGeneratingDataGO = true;
        m_DataReadyGO = false;
        m_PlotValuesY_GO.clear();  // Clear previous data
    }
    else if (exerciseNr == 2)
    {
        m_IsGeneratingDataAlt = true;
        m_DataReadyAlt = false;
        m_PlotValuesY_Alt.clear(); // Clear previous data
    }
    int numSamples;

    if (exerciseNr == 0)
    {
        numSamples = m_NumSamples1;
    }
    else
    {
        numSamples = m_NumSamples2;
    }


    for (int sample = 0; sample < numSamples; ++sample)
    {
        std::vector<int> vecFloat(10000000); 
        std::vector<GameObject3D> vecGO(10000000);
        std::vector<GameObject3DAlt> vecAlt(10000000);

        switch (exerciseNr)
        {
        case 0:
            for (int i = 0; i < 10000000; i++)
            {
                vecFloat[i] = i;
            }
            break;

        case 1:
            for (int i = 0; i < 10000000; i++)
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
            for (int i = 0; i < 10000000; i++)
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

                for (int i = 0; i < 10000000; i += steps)
                {
                    vecFloat[i] *= 2;
                }
                break;

            case 1:
                for (int i = 0; i < 10000000; i += steps)
                {
                    vecGO[i].ID *= 2;
                }
                break;

            case 2:
                for (int i = 0; i < 10000000; i += steps)
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
        if (exerciseNr == 2)
        {
            for (GameObject3DAlt& obj : vecAlt)
            {
                delete obj.transform;
            }
        }

    }


    // Average the results
    std::vector<float> averaged_results;
    for (int i = 0; i < steps_count; ++i)
    {
        float sum = 0.0f;
        for (const auto& run : all_runs_y)
        {
            sum += run[i];
        }
        averaged_results.push_back(sum / static_cast<float>(m_NumSamples1));
    }
    if (exerciseNr == 0)
    {
        m_PlotValuesY = averaged_results;
        m_IsGeneratingData = false;
        m_DataReady = true; // Set the flag when data is ready.
    }
    else if (exerciseNr == 1)
    {
        m_PlotValuesY_GO = averaged_results;  // Store results for GO
        m_IsGeneratingDataGO = false;
        m_DataReadyGO = true;
    }
    else if (exerciseNr == 2)
    {

        m_PlotValuesY_Alt = averaged_results; // Store results for Alt
        m_IsGeneratingDataAlt = false;
        m_DataReadyAlt = true;
    }
    if (m_DataReadyGO && m_DataReadyAlt)
    {
        m_BothPlotsGenerated = true;
    }
    else
    {
        m_BothPlotsGenerated = false;
    }

}