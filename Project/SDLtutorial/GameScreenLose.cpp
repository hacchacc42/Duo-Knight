#include "GameScreenLose.h"
#include "GameScreenManager.h"
/*
	LEVEL 1-1

	UI
	PLAYER 2
	STATIC BLOCKS
	COINS
*/
GameScreenLose::GameScreenLose(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L) : GameScreen(renderer)
{
	std::cout << "L";
	soundEffect = Mix_LoadWAV("Audio/Select.wav");
	Mix_Chunk* loseSound = Mix_LoadWAV("Audio/Lost.wav");
	Mix_PlayChannel(-1, loseSound, 0);
	m_level_map = nullptr;
	SetUpLevel();
}
GameScreenLose::~GameScreenLose()
{
	for (int i = 0; i < m_background_texture.size(); i++)
	{
		delete m_background_texture.at(i);
	}
	m_background_texture.clear();

	for (int i = 0; i < _UI.size(); i++)
	{
		delete _UI.at(i);
	}
	_UI.clear();

}
void GameScreenLose::Render()
{
	m_background_texture[0]->Render(Vector2D(0, 0), SDL_FLIP_NONE); // BackGround
		_UI[0]->Render(0);
}
void GameScreenLose::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_e:
			Mix_PlayChannel(-1, soundEffect, 0);
			
				GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVELMENU, 0, 0, 0, 0);
			break;
		}
		break;
	}
}

bool GameScreenLose::SetUpLevel()
{

	//load texture
	m_background_texture.push_back(new Texture2D(m_renderer));
	if (!m_background_texture[0]->LoadFromFile("Images/LoseScreen.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	CreateUI();
}

void GameScreenLose::CreateUI()
{
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(350, 630), Vector2D(0, 0), "Images/Press_E.png", Vector2D(150, 11), false, 10, 2)); // Press E
}