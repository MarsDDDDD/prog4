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
#include <filesystem>
#include <iostream>
#include "InputManager.h"

namespace fs = std::filesystem;
using namespace dae;

//TODO: change all the shared_ptr into unique_ptr

void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    // Add background
    auto backgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");
    auto background = std::make_shared<dae::GameObject>();
    background->SetLocalPosition(0, 0);

    auto textureComponent = std::make_unique<dae::TextureComponent>(background.get(), backgroundTexture);
    background->AddComponent(std::move(textureComponent));
    scene.Add(background);
    // Add char1
    auto char1Texture = dae::ResourceManager::GetInstance().LoadTexture("char1.png");
    auto char1 = std::make_shared<dae::GameObject>();
    char1->SetLocalPosition(320, 240);

    auto char1TextureComponent = std::make_unique<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(std::move(char1TextureComponent));

    // Input
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadUp, 0, InputManager::InputType::OnHold, std::make_unique<MoveUpCommand>(char1.get(), 100.f));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadDown, 0, InputManager::InputType::OnHold, std::make_unique<MoveDownCommand>(char1.get(), 100.f));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadLeft, 0, InputManager::InputType::OnHold, std::make_unique<MoveLeftCommand>(char1.get(), 100.f));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadRight, 0, InputManager::InputType::OnHold, std::make_unique<MoveRightCommand>(char1.get(), 100.f));

    // Temporary code to test on press
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::A, 0, InputManager::InputType::OnPress, std::make_unique<MoveRightCommand>(char1.get(), 100.f));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::A, 0, InputManager::InputType::OnRelease, std::make_unique<MoveRightCommand>(char1.get(), 100.f));
    scene.Add(char1);

    // Add char2
    auto char2Texture = dae::ResourceManager::GetInstance().LoadTexture("char2.png");
    auto char2 = std::make_shared<dae::GameObject>();
    char2->SetLocalPosition(320, 340);

    auto char2TextureComponent = std::make_unique<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(std::move(char2TextureComponent));

    // Input
    InputManager::GetInstance().AddKeyboardCommand('w', InputManager::InputType::OnHold, std::make_unique<MoveUpCommand>(char2.get(), 200.f));
    InputManager::GetInstance().AddKeyboardCommand('s', InputManager::InputType::OnHold, std::make_unique<MoveDownCommand>(char2.get(), 200.f));
    InputManager::GetInstance().AddKeyboardCommand('a', InputManager::InputType::OnHold, std::make_unique<MoveLeftCommand>(char2.get(), 200.f));
    InputManager::GetInstance().AddKeyboardCommand('d', InputManager::InputType::OnHold, std::make_unique<MoveRightCommand>(char2.get(), 200.f));

    // Temporary code to test on press
    InputManager::GetInstance().AddKeyboardCommand('x', InputManager::InputType::OnPress, std::make_unique<MoveRightCommand>(char2.get(), 200.f));
    InputManager::GetInstance().AddKeyboardCommand('z', InputManager::InputType::OnPress, std::make_unique<MoveRightCommand>(char2.get(), 200.f));

    scene.Add(char2);

    // Add title text
    auto textObject = std::make_shared<dae::GameObject>();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    auto textComponent = std::make_unique<dae::TextComponent>(textObject.get(), "Programming 4 Assignment", font);
    textObject->AddComponent(std::move(textComponent));
    textObject->SetLocalPosition(150, 20);
    scene.Add(textObject);

    // FPS Counter
    auto fpsCounter = std::make_shared<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto fpsTextComponent = std::make_unique<dae::TextComponent>(fpsCounter.get(), "0 FPS", font);
    fpsCounter->AddComponent(std::move(fpsTextComponent));

    auto fpsComponent = std::make_unique<dae::FPSComponent>(fpsCounter.get());
    fpsCounter->AddComponent(std::move(fpsComponent));

    fpsCounter->SetLocalPosition(10, 10);
    scene.Add(fpsCounter);

    // Instruction text
    auto instructionText = std::make_shared<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    auto instructionTextComponent = std::make_unique<dae::TextComponent>(instructionText.get(), "Use WASD and D-Pad to move the two characters.", font);
    instructionText->AddComponent(std::move(instructionTextComponent));

    instructionText->SetLocalPosition(10, 50); // Position below the FPS counter
    scene.Add(instructionText);
}

int main(int, char* [])
{

    //std::string data_location = "./Data/";
    //if (!fs::exists(data_location))
    //    data_location = "../Data/";
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}
