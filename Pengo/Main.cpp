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
#include "GridComponent.h"
#include "GridMoveCommand.h"
#include "DebugEventCommand.h"

#include "ServiceLocator.h"
#include "LoggingSoundSystem.h"
#include "SDLSoundSystem.h"


namespace fs = std::filesystem;
using namespace dae;

void load()
{
	// Load the sound for the game
    ResourceManager::GetInstance().LoadSound(1, "Death.mp3");

    // Define grid parameters
    // Screen size is 640x480, adjust grid to fit properly
    const int gridWidth = 13;
    const int gridHeight = 15;
    // Calculate cell size to fit the grid within the screen
    // Leave some margin for UI elements
    const float cellSize = 30.0f; // Reduced from 40.0f to fit the screen

    // Calculate grid offset to center it on screen horizontally
    // and provide space for UI vertically
    //const float gridOffsetX = (640 - gridWidth * cellSize) / 2;
    //const float gridOffsetY = 100; // Space for UI elements at top

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
    char1->SetLocalPosition(0, 0);

    auto char1TextureComponent = std::make_unique<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(std::move(char1TextureComponent));

    // Add grid component to char1 with offset
    auto char1GridComponent = std::make_unique<GridComponent>(char1.get(), gridWidth, gridHeight, cellSize);
    // Set initial grid position
    char1GridComponent->SetGridPosition(4, 6);
    char1->AddComponent(std::move(char1GridComponent));

    // Add health component
    auto char1HealthComponent = std::make_unique<HealthComponent>(char1.get());
    char1->AddComponent(std::move(char1HealthComponent));
    // Add score component
    auto char1ScoreComponent = std::make_unique<ScoreComponent>(char1.get());
    char1->AddComponent(std::move(char1ScoreComponent));

    // Input with GridMoveCommand - changed from OnHold to OnPress
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadUp, 0, InputManager::InputType::OnPress, 
                                                    std::make_unique<GridMoveCommand>(char1.get(), Direction::Up));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadDown, 0, InputManager::InputType::OnPress, 
                                                    std::make_unique<GridMoveCommand>(char1.get(), Direction::Down));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadLeft, 0, InputManager::InputType::OnPress, 
                                                    std::make_unique<GridMoveCommand>(char1.get(), Direction::Left));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadRight, 0, InputManager::InputType::OnPress, 
                                                    std::make_unique<GridMoveCommand>(char1.get(), Direction::Right));
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
    char2->SetLocalPosition(0, 0);

    auto char2TextureComponent = std::make_unique<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(std::move(char2TextureComponent));

    // Add grid component to char2
    auto char2GridComponent = std::make_unique<GridComponent>(char2.get(), gridWidth, gridHeight, cellSize);
    // Set initial grid position
    char2GridComponent->SetGridPosition(8, 8);
    char2->AddComponent(std::move(char2GridComponent));

    // Add health component
    auto healthComponent = std::make_unique<HealthComponent>(char2.get());
    char2->AddComponent(std::move(healthComponent));
    // Add score component
    auto scoreComponent = std::make_unique<ScoreComponent>(char2.get());
    char2->AddComponent(std::move(scoreComponent));

    // Input with GridMoveCommand - changed from OnHold to OnPress
    InputManager::GetInstance().AddKeyboardCommand('w', InputManager::InputType::OnPress, 
                                                std::make_unique<GridMoveCommand>(char2.get(), Direction::Up));
    InputManager::GetInstance().AddKeyboardCommand('s', InputManager::InputType::OnPress, 
                                                std::make_unique<GridMoveCommand>(char2.get(), Direction::Down));
    InputManager::GetInstance().AddKeyboardCommand('a', InputManager::InputType::OnPress, 
                                                std::make_unique<GridMoveCommand>(char2.get(), Direction::Left));
    InputManager::GetInstance().AddKeyboardCommand('d', InputManager::InputType::OnPress, 
                                                std::make_unique<GridMoveCommand>(char2.get(), Direction::Right));
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

    // Instruction text - Updated to reflect press instead of hold
    auto instructionText1 = std::make_unique<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

    auto instructionTextComponent1 = std::make_unique<dae::TextComponent>(instructionText1.get(), "Char1: Press D-Pad to move on grid.", font);
    instructionText1->AddComponent(std::move(instructionTextComponent1));

    instructionText1->SetLocalPosition(10, 50); // Position below the FPS counter
    scene.Add(std::move(instructionText1));

    auto instructionText2 = std::make_unique<dae::GameObject>();

    auto instructionTextComponent2 = std::make_unique<dae::TextComponent>(instructionText2.get(), "Char2: Press WASD to move on grid, J to inflict damage on self, K and L to add score.", font);
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
    auto char1HudObserver = std::make_shared<HudObserver>();
    char1HudObserver->SetActor(pChar1);
    char1HudObserver->SetHealthDisplay(pChar1HealthText);
    char1HudObserver->SetScoreDisplay(pChar1ScoreText);

    // Register observer with the specific components it should observe
    auto* healthComponent1 = pChar1->GetComponent<HealthComponent>();
    auto* scoreComponent1 = pChar1->GetComponent<ScoreComponent>();
    if (healthComponent1) healthComponent1->AddObserver(char1HudObserver.get());
    if (scoreComponent1) scoreComponent1->AddObserver(char1HudObserver.get());

    // Store the observer in the scene to keep it alive
    scene.AddObserver(char1HudObserver);

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
    auto char2HudObserver = std::make_shared<HudObserver>();
    char2HudObserver->SetActor(pChar2);
    char2HudObserver->SetHealthDisplay(pChar2HealthText);
    char2HudObserver->SetScoreDisplay(pChar2ScoreText);

    // Register observer with the specific components it should observe
    auto* healthComponent2 = pChar2->GetComponent<HealthComponent>();
    auto* scoreComponent2 = pChar2->GetComponent<ScoreComponent>();
    if (healthComponent2) healthComponent2->AddObserver(char2HudObserver.get());
    if (scoreComponent2) scoreComponent2->AddObserver(char2HudObserver.get());

    // Store the observer in the scene to keep it alive
    scene.AddObserver(char2HudObserver);

    scene.Add(std::move(char2HealthText));
    scene.Add(std::move(char2ScoreText));

    std::cout << std::endl;
}

int main(int, char* [])
{
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}
