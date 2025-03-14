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
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "Scene.h"
#include <filesystem>
#include <iostream>
#include "InputManager.h"
#include "HudObserver.h"
#include "Event.h"

namespace fs = std::filesystem;
using namespace dae;

void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    // Add background
    auto backgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");
    auto background = std::make_unique<dae::GameObject>();
    background->SetLocalPosition(0, 0);

    auto textureComponent = std::make_unique<dae::TextureComponent>(background.get(), backgroundTexture);
    background->AddComponent(std::move(textureComponent));
    scene.Add(std::move(background));
    // Add char1
    auto char1Texture = dae::ResourceManager::GetInstance().LoadTexture("char1.png");
    auto char1 = std::make_unique<dae::GameObject>();
    char1->SetLocalPosition(320, 240);

    auto char1TextureComponent = std::make_unique<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(std::move(char1TextureComponent));

    // Add health component
    auto char1HealthComponent = std::make_unique<HealthComponent>(char1.get());
    char1->AddComponent(std::move(char1HealthComponent));
    // Add score component
    auto char1ScoreComponent = std::make_unique<ScoreComponent>(char1.get());
    char1->AddComponent(std::move(char1ScoreComponent));


    // Input
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadUp, 0, InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char1.get(), 100.f, Direction::Up));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadDown, 0, InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char1.get(), 100.f, Direction::Down));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadLeft, 0, InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char1.get(), 100.f, Direction::Left));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadRight, 0, InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char1.get(), 100.f, Direction::Right));
    // Damage self
    InputManager::GetInstance().AddKeyboardCommand('u', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char1.get(), 1, Observer::EventId::HEALTH_UPDATED));
    // Add score
    InputManager::GetInstance().AddKeyboardCommand('i', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char1.get(), 1, Observer::EventId::SCORE_UPDATED));
    InputManager::GetInstance().AddKeyboardCommand('o', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char1.get(), 5, Observer::EventId::SCORE_UPDATED));
    GameObject* pChar1 = char1.get();

    scene.Add(std::move(char1));

    // Add char2
    auto char2Texture = dae::ResourceManager::GetInstance().LoadTexture("char2.png");
    auto char2 = std::make_unique<dae::GameObject>();
    char2->SetLocalPosition(320, 340);

    auto char2TextureComponent = std::make_unique<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(std::move(char2TextureComponent));

	// Add health component
	auto healthComponent = std::make_unique<HealthComponent>(char2.get()); 
    char2->AddComponent(std::move(healthComponent));
	// Add score component
	auto scoreComponent = std::make_unique<ScoreComponent>(char2.get());
    char2->AddComponent(std::move(scoreComponent));


    // Input
    InputManager::GetInstance().AddKeyboardCommand('w', InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char2.get(), 200.f, Direction::Up));
    InputManager::GetInstance().AddKeyboardCommand('s', InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char2.get(), 200.f, Direction::Down));
    InputManager::GetInstance().AddKeyboardCommand('a', InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char2.get(), 200.f, Direction::Left));
    InputManager::GetInstance().AddKeyboardCommand('d', InputManager::InputType::OnHold, std::make_unique<MoveCommand>(char2.get(), 200.f, Direction::Right));
	// Damage self
	InputManager::GetInstance().AddKeyboardCommand('j', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char2.get(), 1, Observer::EventId::HEALTH_UPDATED));
    // Add score
	InputManager::GetInstance().AddKeyboardCommand('k', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char2.get(), 1, Observer::EventId::SCORE_UPDATED));
	InputManager::GetInstance().AddKeyboardCommand('l', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(char2.get(), 5, Observer::EventId::SCORE_UPDATED));
    GameObject* pChar2 = char2.get();

    scene.Add(std::move(char2));

    // Add title text
    auto titleObject = std::make_unique<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    auto titleTextComponent = std::make_unique<dae::TextComponent>(titleObject.get(), "Programming 4 Assignment", font);
    titleObject->AddComponent(std::move(titleTextComponent));
    titleObject->SetLocalPosition(150, 20);
    scene.Add(std::move(titleObject));

    // FPS Counter
    auto fpsCounter = std::make_unique<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto fpsTextComponent = std::make_unique<dae::TextComponent>(fpsCounter.get(), "0 FPS", font);
    fpsCounter->AddComponent(std::move(fpsTextComponent));

    auto fpsComponent = std::make_unique<dae::FPSComponent>(fpsCounter.get());
    fpsCounter->AddComponent(std::move(fpsComponent));

    fpsCounter->SetLocalPosition(10, 10);
    scene.Add(std::move(fpsCounter));

    // Instruction text
    auto instructionText1 = std::make_unique<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

    auto instructionTextComponent1 = std::make_unique<dae::TextComponent>(instructionText1.get(), "Char1: Use D-Pad to move.", font);
    instructionText1->AddComponent(std::move(instructionTextComponent1));

    instructionText1->SetLocalPosition(10, 50); // Position below the FPS counter
    scene.Add(std::move(instructionText1));

    auto instructionText2 = std::make_unique<dae::GameObject>();

    auto instructionTextComponent2 = std::make_unique<dae::TextComponent>(instructionText2.get(), "Char2: Use WASD to move, J to inflict damage on self, K and L to add score.", font);
    instructionText2->AddComponent(std::move(instructionTextComponent2));

    instructionText2->SetLocalPosition(10, 70); // Position below the FPS counter
    scene.Add(std::move(instructionText2));

    // Char1 label
    auto char1Label = std::make_unique<dae::GameObject>();
    auto char1LabelComponent = std::make_unique<dae::TextComponent>(char1Label.get(), "Char1:", font);
    char1Label->AddComponent(std::move(char1LabelComponent));
    char1Label->SetLocalPosition(10, 90);
    scene.Add(std::move(char1Label));

    // Char1 text
    auto char1HealthText = std::make_unique<dae::GameObject>();
    auto char1HealthTextComponent = std::make_unique<dae::TextComponent>(char1HealthText.get(), "Health: 5", font);
    char1HealthText->AddComponent(std::move(char1HealthTextComponent));
    char1HealthText->SetLocalPosition(30, 110);

    auto char1ScoreText = std::make_unique<dae::GameObject>();
    auto char1ScoreTextComponent = std::make_unique<dae::TextComponent>(char1ScoreText.get(), "Score: 0", font);
    char1ScoreText->AddComponent(std::move(char1ScoreTextComponent));
    char1ScoreText->SetLocalPosition(30, 130);

    GameObject* pChar1HealthText = char1HealthText.get();
    GameObject* pChar1ScoreText = char1ScoreText.get();

    // Char1 HudObserver
    auto char1HudObserver = std::make_unique<HudObserver>();
    char1HudObserver->SetActor(pChar1);
    char1HudObserver->SetHealthDisplay(pChar1HealthText);
    char1HudObserver->SetScoreDisplay(pChar1ScoreText);
    Event::GetInstance().AddObserver(std::move(char1HudObserver));

    scene.Add(std::move(char1HealthText));
    scene.Add(std::move(char1ScoreText));

    // Char2 label
    auto char2Label = std::make_unique<dae::GameObject>();
    auto char2LabelComponent = std::make_unique<dae::TextComponent>(char2Label.get(), "Char2:", font);
    char2Label->AddComponent(std::move(char2LabelComponent));
    char2Label->SetLocalPosition(10, 150);
    scene.Add(std::move(char2Label));

    // Char2 text
    auto char2HealthText = std::make_unique<dae::GameObject>();
    auto char2HealthTextComponent = std::make_unique<dae::TextComponent>(char2HealthText.get(), "Health: 5", font);
    char2HealthText->AddComponent(std::move(char2HealthTextComponent));
    char2HealthText->SetLocalPosition(30, 170);

    auto char2ScoreText = std::make_unique<dae::GameObject>();
    auto char2ScoreTextComponent = std::make_unique<dae::TextComponent>(char2ScoreText.get(), "Score: 0", font);
    char2ScoreText->AddComponent(std::move(char2ScoreTextComponent));
    char2ScoreText->SetLocalPosition(30, 190);

    GameObject* pChar2HealthText = char2HealthText.get();
    GameObject* pChar2ScoreText = char2ScoreText.get();

    // Char2 HudObserver
    auto char2HudObserver = std::make_unique<HudObserver>();
    char2HudObserver->SetActor(pChar2);
    char2HudObserver->SetHealthDisplay(pChar2HealthText);
    char2HudObserver->SetScoreDisplay(pChar2ScoreText);
    Event::GetInstance().AddObserver(std::move(char2HudObserver));

    scene.Add(std::move(char2HealthText));
    scene.Add(std::move(char2ScoreText));

    std::cout << std::endl; }

int main(int, char* [])
{
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}
