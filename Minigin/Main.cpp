#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#ifdef _MSVC_LANG
static_assert(_MSVC_LANG >= 202002L, "C++20 required");
#else
static_assert(__cplusplus >= 202002L, "C++20 required");
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FPSComponent.h"
#include "Scene.h"
#include "OrbitComponent.h"
#include "PlotComponent.h" // Add include for the new component
#include <filesystem>
#include <iostream>

void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    // Add background
    auto backgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");
    auto background = std::make_shared<dae::GameObject>();
    background->SetLocalPosition(0, 0);

    // Create texture component using std::make_unique
    auto textureComponent = std::make_unique<dae::TextureComponent>(background.get(), backgroundTexture);
    background->AddComponent(std::move(textureComponent));
    scene.Add(background);

    // Add center object
    auto centerObject = std::make_shared<dae::GameObject>();
    centerObject->SetLocalPosition(320, 240);
    scene.Add(centerObject);

    // Add char1
    auto char1Texture = dae::ResourceManager::GetInstance().LoadTexture("char1.png");
    auto char1 = std::make_shared<dae::GameObject>();
    char1->SetLocalPosition(0, 0);

    // Create TextureComponent with unique_ptr
    auto char1TextureComponent = std::make_unique<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(std::move(char1TextureComponent));

    // Make char1 child of centerObject
    char1->SetParent(centerObject.get());

    // Add OrbitComponent
    auto orbitComponent1 = std::make_unique<dae::OrbitComponent>(char1.get(), 30.0f, -5.f);
    char1->AddComponent(std::move(orbitComponent1));
    scene.Add(char1);

    // Add char2
    auto char2Texture = dae::ResourceManager::GetInstance().LoadTexture("char2.png");
    auto char2 = std::make_shared<dae::GameObject>();
    char2->SetLocalPosition(0, 0);

    // Create TextureComponent with unique_ptr
    auto char2TextureComponent = std::make_unique<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(std::move(char2TextureComponent));

    // Make char2 a child of char1
    char2->SetParent(char1.get());

    // Add OrbitComponent
    auto orbitComponent2 = std::make_unique<dae::OrbitComponent>(char2.get(), 50.0f, 4.f);
    char2->AddComponent(std::move(orbitComponent2));
    scene.Add(char2);

    // Add title text
    auto textObject = std::make_shared<dae::GameObject>();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    // Create TextComponent with unique_ptr
    auto textComponent = std::make_unique<dae::TextComponent>(textObject.get(), "Programming 4 Assignment", font);
    textObject->AddComponent(std::move(textComponent));
    textObject->SetLocalPosition(150, 20);
    scene.Add(textObject);

    // FPS Counter
    auto fpsCounter = std::make_shared<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    // Create TextComponent with unique_ptr
    auto fpsTextComponent = std::make_unique<dae::TextComponent>(fpsCounter.get(), "0 FPS", font);
    fpsCounter->AddComponent(std::move(fpsTextComponent));

    // Create FPSComponent with unique_ptr
    auto fpsComponent = std::make_unique<dae::FPSComponent>(fpsCounter.get());
    fpsCounter->AddComponent(std::move(fpsComponent));

    fpsCounter->SetLocalPosition(10, 10);
    scene.Add(fpsCounter);
    
    // Add a plot component
    auto plotObject = std::make_shared<dae::GameObject>();
    auto plotComponent = std::make_unique<dae::PlotComponent>(plotObject.get());
    plotObject->AddComponent(std::move(plotComponent));
    scene.Add(plotObject);
}

int main(int, char* [])
{
    dae::Minigin engine("Data");
    engine.Run(load);
    return 0;
}
