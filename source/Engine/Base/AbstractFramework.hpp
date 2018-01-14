#pragma once

#include "../staticDependancies/glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

//Static stuff
static inline void quit_SDL_error(const char * message)
{
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	std::cin.get();
	exit(2);
}

class AbstractFramework
{
public:
	AbstractFramework();
	virtual ~AbstractFramework();

	void Run();

protected:
	virtual void AddScenes() = 0;
	virtual void Update() = 0;
	void ClearTarget();

private:
	void InitializeSDL();
	void LoadConfig();
	void InitializeWindow();
	void InitializeUtilities();
	void BindOpenGL();
	void InitializeDebug();
	void InitializeGame();

	void GameLoop();

private:
	SDL_GLContext m_GlContext = nullptr;
};

