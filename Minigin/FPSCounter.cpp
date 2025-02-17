//#include "FPSCounter.h"
//#include "FPSCounterComponent.h"
//#include "TextRendererComponent.h"
//
//
//dae::FPSCounter::FPSCounter()
//{
//    // Add the FPSCounterComponent
//    m_pFPSCounter = AddComponent<FPSCounterComponent>();
//
//    // Add the TextRendererComponent (positioned at the top-right corner)
//    m_pTextRenderer = AddComponent<TextRendererComponent>("FPS: 0", 2.f, 2.f);
//}
//
//void dae::FPSCounter::Update()
//{
//    // Update the FPS text
//    float fps = m_pFPSCounter->GetFPS();
//    m_pTextRenderer->SetText("FPS: " + std::to_string(fps));
//    
//
//    // Call the base GameObject update
//    GameObject::Update();
//}
//void dae::FPSCounter::Render() const
//{
//   //render the text
//    m_pTextRenderer->Render();
//}