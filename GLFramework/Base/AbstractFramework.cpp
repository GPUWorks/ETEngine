#include "stdafx.hpp"
#include "AbstractFramework.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "../SceneGraph/SceneManager.hpp"
#include "../GraphicsHelper/LightVolume.hpp"
#include "../GraphicsHelper/ShadowRenderer.hpp"

AbstractFramework::AbstractFramework()
{
}
AbstractFramework::~AbstractFramework()
{
	Logger::Release();
	Settings* pSet = Settings::GetInstance();
	Context* pCon = Context::GetInstance();
	SceneManager* pScMan = SceneManager::GetInstance();
	InputManager* pInMan = InputManager::GetInstance();
	PointLightVolume* pVol = PointLightVolume::GetInstance();
	DirectLightVolume* pDirVol = DirectLightVolume::GetInstance();
	ShadowRenderer* pShadowRenderer = ShadowRenderer::GetInstance();

	ContentManager::Release();

	SDL_GL_DeleteContext(m_GlContext);
	SDL_Quit();

	pSet->DestroyInstance();
	pScMan->DestroyInstance();
	pInMan->DestroyInstance();
	pCon->DestroyInstance();
	pVol->DestroyInstance();
	pDirVol->DestroyInstance();
	pShadowRenderer->DestroyInstance();
}

void AbstractFramework::Run()
{
	InitializeSDL();
	InitializeWindow();
	InitializeDevIL();
	BindOpenGL();
	InitializeDebug();
	InitializeGame();

	GameLoop();
}



void AbstractFramework::InitializeSDL()
{
	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Request a debug context.
#if defined(DEBUG) | defined(_DEBUG)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	// Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		sdl_die("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);
}

void AbstractFramework::InitializeWindow()
{
	Settings* pSet = Settings::GetInstance();//Initialize Game Settings
	SceneManager* pScMan = SceneManager::GetInstance();//Initialize SceneManager
	InputManager* pInMan = InputManager::GetInstance();//init input manager
	pInMan->Init();

	//Create Window
	if (pSet->Window.Fullscreen)
	{
		pSet->Window.pWindow = SDL_CreateWindow(pSet->Window.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pSet->Window.Width, pSet->Window.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		pSet->Window.pWindow = SDL_CreateWindow(pSet->Window.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pSet->Window.Width, pSet->Window.Height, SDL_WINDOW_OPENGL);
	}
	if (pSet->Window.pWindow == NULL)sdl_die("Couldn't set video mode");

	//OpenGl context creation
	m_GlContext = SDL_GL_CreateContext(pSet->Window.pWindow);
	if (m_GlContext == NULL)sdl_die("Failed to create OpenGL context");

	// Use v-sync
	SDL_GL_SetSwapInterval(1);
}

void AbstractFramework::InitializeDevIL()
{
	//Init DevIL
	ilInit();
	iluInit();
}

void AbstractFramework::BindOpenGL()
{
	// Check OpenGL properties and create open gl function pointers
	std::cout << "OpenGL loaded" << std::endl << std::endl;
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	std::cout << "Vendor: \t" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer:\t" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version:\t" << glGetString(GL_VERSION) << std::endl << std::endl;
}

void AbstractFramework::InitializeDebug()
{
	Logger::Initialize();
//	// Enable the debug callback
//#ifdef _DEBUG
//	glEnable(GL_DEBUG_OUTPUT);
//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//	glDebugMessageCallback(openglCallbackFunction, nullptr);
//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
//#endif
}

void AbstractFramework::InitializeGame()
{
	//Initialize Managers
	ContentManager::Initialize();

	SceneManager::GetInstance()->Initialize();
	ShadowRenderer::GetInstance()->Initialize();

	//Initialize Game
	Initialize();
}

void AbstractFramework::GameLoop()
{
	Settings* pSet = Settings::GetInstance();
	while (true)
	{
		InputManager::GetInstance()->UpdateEvents();
		if (InputManager::GetInstance()->IsExitRequested())return;

		Update();
		//******
		//UPDATE
		SceneManager::GetInstance()->Update();

		//****
		//DRAW
		SceneManager::GetInstance()->Draw();

		//Swap front and back buffer
		SDL_GL_SwapWindow(pSet->Window.pWindow);
	}
}

void AbstractFramework::ClearTarget()
{
	// Clear the screen to white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}