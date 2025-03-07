//#pragma once
//#include "BaseComponent.h"
//#include <vector>
//#include <string>
//
//struct Transform
//{
//    float matrix[16] = {
//        1,0,0,0,
//        0,1,0,0,
//        0,0,1,0,
//        0,0,0,1 };
//};
//
//class GameObject3D
//{
//public:
//    Transform transform;
//    int ID{0};
//};
//
//class GameObject3DAlt
//{
//public:
//    Transform* transform;
//    int ID{ 0 };
//};
//
//namespace dae
//{
//    class GuiComponent : public BaseComponent
//    {
//    public:
//        // Constructor
//        GuiComponent(GameObject* pOwner);
//        ~GuiComponent();
//
//        // Update the content of an existing ImGui window
//        void UpdateImGui() const;
//
//        // Generate timing data for different step sizes (GameObject3D)
//        void GenerateTimingData();
//
//        // Generate timing data for different step sizes (GameObject3DAlt)
//        void GenerateTimingDataAlt();
//
//        // Generate timing data for different step sizes (Int)
//        void GenerateTimingDataInt();
//
//    private:
//        std::vector<std::pair<int, double>> m_timingData; // Stores step size and average time for GameObject3D
//        std::vector<std::pair<int, double>> m_timingDataAlt; // Stores step size and average time for GameObject3DAlt
//        std::vector<std::pair<int, double>> m_timingDataInt; // Stores step size and average time for int
//        std::string m_graphTitle;  // Title of the graph
//        int m_numSamples{ 10 };    // Number of samples to take per step size
//        int m_numSamplesInt{ 10 };    // Number of samples to take per step size
//        float m_minScale{ 0.0f };  // Minimum scale for the graph
//        float m_maxScale{ 100.0f }; // Maximum scale for the graph (adjust based on your data)
//        float m_minScaleInt{ 0.0f };  // Minimum scale for the graph
//        float m_maxScaleInt{ 100.0f }; // Maximum scale for the graph (adjust based on your data)
//        mutable bool m_generateGraph{ false }; // Flag to control graph generation for GameObject3D
//        mutable bool m_generateGraphAlt{ false }; // Flag to control graph generation for GameObject3DAlt
//        mutable bool m_generateGraphInt{ false };
//         
//    };
//}