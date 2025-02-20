#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
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
#include <filesystem> // For std::filesystem::current_path()
#include <iostream>

void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    // Add background
    auto backgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");
    auto background = std::make_shared<dae::GameObject>();
    background->SetLocalPosition(0, 0);
    auto textureComponent = std::make_shared<dae::TextureComponent>(background.get(), backgroundTexture);
    background->AddComponent(textureComponent);
    scene.Add(background);

	// Add center object

	auto centerObject = std::make_shared<dae::GameObject>();
	centerObject->SetLocalPosition(320, 240); // Center of the screen
	scene.Add(centerObject);

    // Add char1
    auto char1Texture = dae::ResourceManager::GetInstance().LoadTexture("char1.png");
    auto char1 = std::make_shared<dae::GameObject>();
    char1->SetLocalPosition(0, 0); 
    auto char1TextureComponent = std::make_shared<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(char1TextureComponent);
	// Set char1 as a child of the center object
	char1->SetParent(centerObject.get());
	// Make char1 orbit around the center object
	auto orbitComponent1 = std::make_shared<dae::OrbitComponent>(char1.get(), 30.0f, -5.f);
	char1->AddComponent(orbitComponent1);
    
    scene.Add(char1);

    // Add char2
    auto char2Texture = dae::ResourceManager::GetInstance().LoadTexture("char2.png");
    auto char2 = std::make_shared<dae::GameObject>();
    // Ensure char2 has a TransformComponent by setting its position
    char2->SetLocalPosition(0, 0); 
    auto char2TextureComponent = std::make_shared<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(char2TextureComponent);
    // Set char2 as a child of char1
	char2->SetParent(char1.get());
	// and make it orbit around char1
	auto orbitComponent2 = std::make_shared<dae::OrbitComponent>(char2.get(), 50.0f, 4.f);
	char2->AddComponent(orbitComponent2);
    scene.Add(char2);

    // Add title text
    auto textObject = std::make_shared<dae::GameObject>();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
    auto textComponent = std::make_shared<dae::TextComponent>(textObject.get(), "Programming 4 Assignment", font);
    textObject->AddComponent(textComponent);
    textObject->SetLocalPosition(150, 20);
    scene.Add(textObject);

    // FPS Counter setup
    auto fpsCounter = std::make_shared<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fpsTextComponent = std::make_shared<dae::TextComponent>(fpsCounter.get(), "0 FPS", font);
    fpsCounter->AddComponent(fpsTextComponent);
    auto fpsComponent = std::make_shared<dae::FPSComponent>(fpsCounter.get());
    fpsCounter->AddComponent(fpsComponent);
    fpsCounter->SetLocalPosition(10, 10); // Position the FPS counter at the top-left corner
    scene.Add(fpsCounter); 
}

int main(int, char* []) {

    dae::Minigin engine("Data"); // Pass relative path
    //dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}
