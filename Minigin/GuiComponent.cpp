#include "GuiComponent.h"
#include "imgui.h"
#include "Renderer.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm> 
using namespace std::chrono;
using namespace std;

#include "GuiComponent.h"
#include "imgui.h"
#include <iostream>

namespace dae
{
    GuiComponent::GuiComponent(GameObject* pOwner)
        : BaseComponent(pOwner), m_graphTitle("Step Size vs Time")
    {
        // Register this component with the Renderer
        Renderer::GetInstance().RegisterGuiComponent(this);
    }

    GuiComponent::~GuiComponent()
    {
        // Unregister this component from the Renderer
        Renderer::GetInstance().UnregisterGuiComponent(this);
    }

    void GuiComponent::GenerateTimingData()
    {
        std::vector<GameObject3D> arr(1 << 26); // Initialize vector with 2^26 elements

        m_timingData.clear(); // Clear previous timing data

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            std::vector<long long> samples(m_numSamples); // Store timing results

            for (int sample = 0; sample < m_numSamples; ++sample)
            {
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < arr.size(); i += stepsize)
                {
                    arr[i].ID *= 2; // Perform the operation
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                samples[sample] = elapsedTime; // Store the timing result
            }

            // Sort the samples to easily remove the highest and lowest values
            std::sort(samples.begin(), samples.end());

            // Remove the highest and lowest values (outliers)
            long long totalTime = samples.size();
            for (int i = 1; i < m_numSamples - 1; ++i) // Skip first and last elements
            {
                totalTime += samples[i];
            }

            // Calculate the average time (excluding outliers)
            double averageTime = totalTime / static_cast<double>(m_numSamples - 2);

            // Store the step size and average time
            m_timingData.emplace_back(stepsize, averageTime / 1000.0); // Convert to milliseconds
        }

        // Calculate m_maxScale based on the first result + 100
        if (!m_timingData.empty())
        {
            m_maxScale = static_cast<float>(m_timingData[0].second) + 100.0f;
        }
    }

    void GuiComponent::GenerateTimingDataAlt()
    {
        std::vector<GameObject3DAlt> arr(1 << 26); // Initialize vector with 2^26 elements

        m_timingDataAlt.clear(); // Clear previous timing data

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            std::vector<long long> samples(m_numSamples); // Store timing results

            for (int sample = 0; sample < m_numSamples; ++sample)
            {
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < arr.size(); i += stepsize)
                {
                    arr[i].ID *= 2; // Perform the operation
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                samples[sample] = elapsedTime; // Store the timing result
            }

            // Sort the samples to easily remove the highest and lowest values
            std::sort(samples.begin(), samples.end());

            // Remove the highest and lowest values (outliers)
            long long totalTime = 0;
            for (int i = 1; i < m_numSamples - 1; ++i) // Skip first and last elements
            {
                totalTime += samples[i];
            }

            // Calculate the average time (excluding outliers)
            double averageTime = totalTime / static_cast<double>(m_numSamples - 2);

            // Store the step size and average time
            m_timingDataAlt.emplace_back(stepsize, averageTime / 1000.0); // Convert to milliseconds
        }

        // Calculate m_maxScale based on the first result + 100
        if (!m_timingDataAlt.empty())
        {
            m_maxScale = static_cast<float>(m_timingDataAlt[0].second) + 100.0f;
        }
    }

   void GuiComponent::UpdateImGui() const
{
    // Ensure ImGui is within a frame
    if (!ImGui::GetCurrentContext())
    {
        return; // Skip if ImGui is not initialized
    }

    // Create an ImGui window for the graph
    ImGui::Begin("Step Size vs Time");

    // Add a slider to control the number of samples
    ImGui::SliderInt("Sample Size", &const_cast<int&>(m_numSamples), 10, 100); // Allow sample size between 1 and 100

    // Add a button to generate the timing data for GameObject3D
    if (ImGui::Button("Generate Graph (GameObject3D)"))
    {
        std::cout << "Generate Graph (GameObject3D) button clicked\n";
        const_cast<GuiComponent*>(this)->GenerateTimingData(); // Call GenerateTimingData
        m_generateGraph = true; // Set the flag to true
        m_generateGraphAlt = false; // Disable the other graph
    }

    // Add a button to generate the timing data for GameObject3DAlt
    if (ImGui::Button("Generate Graph (GameObject3DAlt)"))
    {
        std::cout << "Generate Graph (GameObject3DAlt) button clicked\n";
        const_cast<GuiComponent*>(this)->GenerateTimingDataAlt(); // Call GenerateTimingDataAlt
        m_generateGraphAlt = true; // Set the flag to true
        m_generateGraph = false; // Disable the other graph
    }

    // If the generate graph flag is true, display the graph for GameObject3D
    if (m_generateGraph && !m_timingData.empty())
    {
        // Prepare the data for ImGui::PlotLines
        std::vector<float> stepSizes;
        std::vector<float> averageTimes;

        for (const auto& [stepsize, avgTime] : m_timingData)
        {
            stepSizes.push_back(static_cast<float>(stepsize));
            averageTimes.push_back(static_cast<float>(avgTime));
        }

        // Plot the data using ImGui::PlotLines
        ImGui::PlotLines(
            "GameObject3D",                         // Label for the graph
            averageTimes.data(),                    // Pointer to the data
            static_cast<int>(averageTimes.size()),  // Number of data points
            0,                                      // Index offset
            nullptr,                                // Overlay text (optional)
            m_minScale,                             // Scale minimum
            m_maxScale,                             // Scale maximum
            ImVec2(0, 200)                          // Graph size
        );

        // Display step sizes on the X-axis
        ImDrawList* drawList = ImGui::GetWindowDrawList(); // Get the draw list for custom rendering
        ImVec2 graphPos = ImGui::GetCursorScreenPos(); // Get the position of the graph
        float graphWidth = ImGui::GetContentRegionAvail().x; // Get the width of the graph

        for (size_t i = 0; i < stepSizes.size(); ++i)
        {
            // Calculate the X position for each step size label
            float xPos = graphPos.x + (graphWidth / (stepSizes.size() - 1)) * i;

            // Draw the step size label
            std::string label = std::to_string(static_cast<int>(stepSizes[i]));
            ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
            drawList->AddText(ImVec2(xPos - textSize.x / 2, graphPos.y + 210), IM_COL32_WHITE, label.c_str());
        }
    }

    // If the generate graph flag is true, display the graph for GameObject3DAlt
    if (m_generateGraphAlt && !m_timingDataAlt.empty())
    {
        // Prepare the data for ImGui::PlotLines
        std::vector<float> stepSizes;
        std::vector<float> averageTimes;

        for (const auto& [stepsize, avgTime] : m_timingDataAlt)
        {
            stepSizes.push_back(static_cast<float>(stepsize));
            averageTimes.push_back(static_cast<float>(avgTime));
        }

        // Plot the data using ImGui::PlotLines
        ImGui::PlotLines(
            "GameObject3DAlt",   // Label for the graph
            averageTimes.data(),   // Pointer to the data
            static_cast<int>(averageTimes.size()), // Number of data points
            0,                      // Index offset
            nullptr,                // Overlay text (optional)
            m_minScale,             // Scale minimum
            m_maxScale,             // Scale maximum
            ImVec2(0, 200)          // Graph size
        );

        // Display step sizes on the X-axis
        ImDrawList* drawList = ImGui::GetWindowDrawList(); // Get the draw list for custom rendering
        ImVec2 graphPos = ImGui::GetCursorScreenPos(); // Get the position of the graph
        float graphWidth = ImGui::GetContentRegionAvail().x; // Get the width of the graph

        for (size_t i = 0; i < stepSizes.size(); ++i)
        {
            // Calculate the X position for each step size label
            float xPos = graphPos.x + (graphWidth / (stepSizes.size() - 1)) * i;

            // Draw the step size label
            std::string label = std::to_string(static_cast<int>(stepSizes[i]));
            ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
            drawList->AddText(ImVec2(xPos - textSize.x / 2, graphPos.y + 210), IM_COL32_WHITE, label.c_str());
        }
    }

    // End the ImGui window
    ImGui::End();
}
}