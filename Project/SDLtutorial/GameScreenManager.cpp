#include "GameScreenManager.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenLevel3.h"
#include "GameScreenLevelMenu.h"
#include "GameScreenWin.h"
#include "GameScreenLose.h"

GameScreenManager* GameScreenManager::inst = nullptr;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	inst = this;
	m_renderer = renderer;
	m_current_screen = nullptr;
	ChangeScreen(startScreen, 0, 0, 0, 0);
}
GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr;
	delete m_current_screen;
	m_current_screen = nullptr;
}
void GameScreenManager::Render()
{
	m_current_screen->Render();
}
bool GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	if (quitSelected)
	{
		return true;
	}

	m_current_screen->Update(deltaTime, e);

	return false;
}
void GameScreenManager::ChangeScreen(SCREENS new_screen, int FP_S, int FP_L, int SP_S, int SP_L)
{
	//clear up the old screen
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}
	GameScreen* tempScreen = nullptr;
	switch (new_screen)
	{
	case SCREEN_LEVEL1:
		tempScreen = (GameScreen*) new GameScreenLevel1(m_renderer, SP_S,SP_L);
		break;
	case SCREEN_LEVEL2:
		tempScreen = (GameScreen*) new GameScreenLevel2(m_renderer, FP_S, FP_L, SP_S, SP_L);
		break;
	case SCREEN_LEVEL3:
		tempScreen = (GameScreen*) new GameScreenLevel3(m_renderer, FP_S, FP_L, SP_S, SP_L);
		break;
	case SCREEN_LEVELMENU:
		tempScreen = (GameScreen*) new GameScreenLevelMenu(m_renderer);
		break;
	case SCREEN_WIN:
		tempScreen = (GameScreen*) new GameScreenWin(m_renderer, FP_S, FP_L, SP_S, SP_L);
		break;
	case SCREEN_LOSE:
		tempScreen = (GameScreen*) new GameScreenLose(m_renderer, FP_S, FP_L, SP_S, SP_L);
		break;
	default:;
	}
	m_current_screen = tempScreen;
}