#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <SDL_ttf.h>

#include "constants.h"
#include "Commons.h"
#include "Texture2D.h"
#include "GameScreenManager.h"

#include <iostream>
#include <string>

#include "GameScreenLevelMenu.h"

using namespace std;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
GameScreenManager* game_screen_manager;
Uint32 g_old_time;
Mix_Music* g_music = nullptr;

// Function prototypes
bool InitSDL();
void CLoseSDL();
bool Update();
void LoadMusic(string filePath);
void Render();

int main(int argc, char* args[])
{
	//check if sdl was setup correctly
	if (InitSDL())
	{
		LoadMusic("Audio/song.wav"); // Loading Song
		if (Mix_PlayingMusic() == 0) // if a music is not playing then play the song
		{
			Mix_PlayMusic(g_music, -1);
		}
		bool quit = false; //flag to check if we wish to quit
		game_screen_manager = new GameScreenManager(g_renderer, SCREEN_LEVELMENU);
		g_old_time = SDL_GetTicks(); //set the time

		//Game Loop
		while (!quit)
		{
			Render();
			quit = Update();
		}
	}
	CLoseSDL();
	return 0;
}

bool InitSDL()
{
	//Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//setup passed so create window
		g_window = SDL_CreateWindow("Untiled Knight Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		//did the window get created?
		if (g_window == nullptr)
		{
			//window failed
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
	}
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); //Setting the render if everything is ok
	if (g_renderer != nullptr)
	{
		//init PNG loading
		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
			return false;
		}
	}
	else
	{
		cout << "Renderer could not initialise. Error: " << SDL_GetError();
		return false;
	}
	if (TTF_Init()<0) // init TTF ( TEXT )
	{
		cout << "TTF could not init. Error: ";
		return false;
	}
	//initialise the mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Mixer could not init. Error: " << Mix_GetError();
		return false;
	}
	return true;
}


void CLoseSDL()
{
	//release the window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	//quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
	//release the renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
	//destroy the game screen manager
	delete game_screen_manager;
	game_screen_manager = nullptr;
	//clear up music
	Mix_FreeMusic(g_music);
	g_music = nullptr;
}

bool Update()
{
	Uint32 new_time = SDL_GetTicks();
	//Event handler
	SDL_Event e;
	//get events
	SDL_PollEvent(&e);
	//handle the events
	bool quit = false;

	switch (e.type)
	{
		//click the 'X' to quit
	case SDL_QUIT:
		return true;
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_q:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVELMENU,0,0,0,0); // QUICK TEST ONLY
			break;
		}
	}
	quit = game_screen_manager->Update((float)(new_time - g_old_time) / 1000.0f, e);
	g_old_time = new_time;
	return quit;
}

void Render()
{
	//Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);
	game_screen_manager->Render();

	//update the screen
	SDL_RenderPresent(g_renderer);
}
void LoadMusic(string filePath)
{
	g_music = Mix_LoadMUS(filePath.c_str());
	if (g_music == nullptr)
	{
		cout << "Failed to load music. Error: " << Mix_GetError() << endl;
	}
}