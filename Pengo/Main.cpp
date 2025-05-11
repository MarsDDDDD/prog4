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
#include "GridEntityComponent.h"
#include "NewGridComponent.h"
#include "GridMoveCommand.h"
#include "DebugEventCommand.h"

#include "ServiceLocator.h"
#include "LoggingSoundSystem.h"
#include "SDLSoundSystem.h"
#include "LevelLoader.h"
namespace fs = std::filesystem;
using namespace dae;

void load()
{
    // Load the sound for the game
    ResourceManager::GetInstance().LoadSound(1, "Death.mp3");

    // Define grid parameters
    const int gridWidth = 13;
    const int gridHeight = 15;
    const float cellSize = 16.0f;

    // Calculate grid offset to center it on screen
    const float gridOffsetX = (640 - gridWidth * cellSize) / 2;
    const float gridOffsetY = (480 - gridHeight * cellSize) / 2;

    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    // Add grid background and grid object
    auto gridBackgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("bg.png");
    auto gridGameObject = std::make_unique<dae::GameObject>();
    gridGameObject->SetLocalPosition(gridOffsetX, gridOffsetY);

    auto gridBackgroundTextureComponent = std::make_unique<dae::TextureComponent>(gridGameObject.get(), gridBackgroundTexture);
    gridGameObject->AddComponent(std::move(gridBackgroundTextureComponent));

    // Add the NewGridComponent to our grid game object
    auto newGridComponent = std::make_unique<NewGridComponent>(gridGameObject.get(), gridWidth, gridHeight, cellSize, cellSize * 0.5f, cellSize * 0.5f);
    auto* pGridComponent = newGridComponent.get();
    gridGameObject->AddComponent(std::move(newGridComponent));

    GameObject* pGridGameObject = gridGameObject.get();
    scene.Add(std::move(gridGameObject));

    // Create characters as before
    auto char1 = std::make_unique<dae::GameObject>();
    auto char1Texture = dae::ResourceManager::GetInstance().LoadTexture("char1.png");
    auto char1TextureComponent = std::make_unique<dae::TextureComponent>(char1.get(), char1Texture);
    char1->AddComponent(std::move(char1TextureComponent));
    auto char1GridEntityComponent = std::make_unique<GridEntityComponent>(char1.get());
    char1->AddComponent(std::move(char1GridEntityComponent));
    auto char1HealthComponent = std::make_unique<HealthComponent>(char1.get());
    char1->AddComponent(std::move(char1HealthComponent));
    auto char1ScoreComponent = std::make_unique<ScoreComponent>(char1.get());
    char1->AddComponent(std::move(char1ScoreComponent));

    GameObject* pChar1 = char1.get();
    scene.Add(std::move(char1));

    auto char2 = std::make_unique<dae::GameObject>();
    auto char2Texture = dae::ResourceManager::GetInstance().LoadTexture("char2.png");
    auto char2TextureComponent = std::make_unique<dae::TextureComponent>(char2.get(), char2Texture);
    char2->AddComponent(std::move(char2TextureComponent));
    auto char2GridEntityComponent = std::make_unique<GridEntityComponent>(char2.get());
    char2->AddComponent(std::move(char2GridEntityComponent));
    auto healthComponent = std::make_unique<HealthComponent>(char2.get());
    char2->AddComponent(std::move(healthComponent));
    auto scoreComponent = std::make_unique<ScoreComponent>(char2.get());
    char2->AddComponent(std::move(scoreComponent));

    GameObject* pChar2 = char2.get();
    scene.Add(std::move(char2));

    // Define the level layout
    std::string levelLayout =
        "0000000100000"
        "0111110111110"
        "0000010100000"
        "1111020101111"
        "0001010000000"
        "0101021111120"
        "0100010100000"
        "0111110101111"
        "0100000100000"
        "0101110111110"
        "0101010100010"
        "1101010101010"
        "0001000101010"
        "0111011101010"
        "0100000001000";

    // Parse and load the level
    std::vector<int> levelData = LevelLoader::ParseLevelString(levelLayout);
    std::vector<GameObject*> characters = { pChar1, pChar2 };
    LevelLoader::LoadLevel(levelData, gridWidth, gridHeight, pGridComponent,
         characters, scene);

    // Setup input commands for characters
    // Input with GridMoveCommand for char1
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadUp, 0, InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar1, pGridGameObject, Direction::Up));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadDown, 0, InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar1, pGridGameObject, Direction::Down));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadLeft, 0, InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar1, pGridGameObject, Direction::Left));
    InputManager::GetInstance().AddControllerCommand(XBoxController::XBoxButton::DPadRight, 0, InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar1, pGridGameObject, Direction::Right));

    // Damage self
    InputManager::GetInstance().AddKeyboardCommand('u', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar1, 1, Observer::EventId::HEALTH_UPDATED));
    // Add score
    InputManager::GetInstance().AddKeyboardCommand('i', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar1, 1, Observer::EventId::SCORE_UPDATED));
    InputManager::GetInstance().AddKeyboardCommand('o', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar1, 5, Observer::EventId::SCORE_UPDATED));


    // Input with GridMoveCommand for char2
    InputManager::GetInstance().AddKeyboardCommand('w', InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar2, pGridGameObject, Direction::Up));
    InputManager::GetInstance().AddKeyboardCommand('s', InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar2, pGridGameObject, Direction::Down));
    InputManager::GetInstance().AddKeyboardCommand('a', InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar2, pGridGameObject, Direction::Left));
    InputManager::GetInstance().AddKeyboardCommand('d', InputManager::InputType::OnHold,
        std::make_unique<GridMoveCommand>(pChar2, pGridGameObject, Direction::Right));

    // Damage self
    InputManager::GetInstance().AddKeyboardCommand('j', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar2, 1, Observer::EventId::HEALTH_UPDATED));
    // Add score
    InputManager::GetInstance().AddKeyboardCommand('k', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar2, 1, Observer::EventId::SCORE_UPDATED));
    InputManager::GetInstance().AddKeyboardCommand('l', InputManager::InputType::OnPress, std::make_unique<DebugEventCommand>(pChar2, 5, Observer::EventId::SCORE_UPDATED));

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

    // Instruction text - Updated to reflect hold instead of press
    auto instructionText1 = std::make_unique<dae::GameObject>();
    font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

    auto instructionTextComponent1 = std::make_unique<dae::TextComponent>(instructionText1.get(), "Char1: Hold D-Pad to move on grid.", font);
    instructionText1->AddComponent(std::move(instructionTextComponent1));

    instructionText1->SetLocalPosition(10, 50); // Position below the FPS counter
    scene.Add(std::move(instructionText1));

    auto instructionText2 = std::make_unique<dae::GameObject>();

    auto instructionTextComponent2 = std::make_unique<dae::TextComponent>(instructionText2.get(), "Char2: Hold WASD to move on grid, J to inflict damage on self, K and L to add score.", font);
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
