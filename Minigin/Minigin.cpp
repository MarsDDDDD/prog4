#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <filesystem> 

#include <chrono>
#include <thread>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);


	// Edit: using absolute path to data folder
	// Get the path to the executable
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::filesystem::path executablePath(buffer);

	// Construct the full data path.  Assumes Data is next to the .exe
	std::filesystem::path absoluteDataPath = executablePath.parent_path() / dataPath;
	ResourceManager::GetInstance().Init(absoluteDataPath);

	//ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}


void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;

    //Fixed Time Step
	const float fixedTimeStep{ 0.02f }; // Example: 50 FPS for physics.  Should be a setting.
	float lag = 0.0f;
    auto lastTime = std::chrono::high_resolution_clock::now();

	while (doContinue)
	{
        // Calculate delta time.
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count(); // deltaTime in seconds.
        lastTime = currentTime;

        lag += deltaTime;


		doContinue = input.ProcessInput();

        // Fixed update loop (for physics and other things that need consistent updates).
        while (lag >= fixedTimeStep)
        {
            sceneManager.FixedUpdate(fixedTimeStep); //Pass fixed timestep to a new method
            lag -= fixedTimeStep;
        }

		sceneManager.Update(deltaTime); // Pass delta time to update.
		renderer.Render();

        //Optional: limit frame rate
        const auto sleep_time = currentTime + std::chrono::milliseconds(16) - std::chrono::high_resolution_clock::now(); //Try to maintain around 60 fps render, Should be a setting
        if (sleep_time.count() > 0)
            std::this_thread::sleep_for(sleep_time);

	}
}