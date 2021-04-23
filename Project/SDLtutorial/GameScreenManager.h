#pragma once
#include "Commons.h"
#include <SDL.h>
#include "GameScreen.h"
class GameScreen;

class GameScreenManager
{
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();
	static GameScreenManager* GetInstance() { return inst; }
	void Render();
	bool Update(float deltaTime, SDL_Event e);
	void ChangeScreen(SCREENS new_screen, int FP_S, int FP_L, int SP_S, int SP_L);

	void QuitSelected() { quitSelected = true; }



private:
	SDL_Renderer* m_renderer;
	GameScreen* m_current_screen;
	static GameScreenManager* inst;

	bool quitSelected = false;
};

