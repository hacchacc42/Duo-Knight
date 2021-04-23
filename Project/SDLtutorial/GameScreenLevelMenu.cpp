#include "GameScreenLevelMenu.h"
#include "GameScreenManager.h"
#include <iostream>
#include <fstream>
#include <string>
/*
	LEVEL 1-1

	UI
	PLAYER 2
	STATIC BLOCKS
	COINS
*/
GameScreenLevelMenu::GameScreenLevelMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	soundEffect = Mix_LoadWAV("Audio/Select.wav");
	_quit = false;
	_offset = 0;
	_menu = 0;
	_select = 0;
	m_level_map = nullptr;
	SetUpLevel();
}
GameScreenLevelMenu::~GameScreenLevelMenu()
{
	std::cout << "FA";
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
void GameScreenLevelMenu::Render()
{
	m_background_texture[0]->Render(Vector2D(0, 0), SDL_FLIP_NONE); // BackGround
	if (_offset < -2012)
	{
		_offset = 0;
	}
	m_background_texture[1]->Render(Vector2D(_offset -= .005, 0), SDL_FLIP_NONE); // Moving BackG
	m_background_texture[2]->Render(Vector2D(0, 0), SDL_FLIP_NONE); // Texts
	_UI[0]->Render(0);
	_UI[1]->Render(0);
	if (_menu == 2)
	{
		for (int i = 0; i < _HST.size(); i++)
		{
			_HST[i]->Render(64, 500-i*35); // HS TEXT
		}
	}
}
void GameScreenLevelMenu::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (_menu != 3 && _menu != 2)
			{
				if (_UI[0]->m_position.y > 150)
				{
					_select -= 1;
					_UI[0]->ChangePos(_UI[0]->m_position.x, _UI[0]->m_position.y - 89);
				}
			}
			break;
		case SDLK_s:
				if (_UI[0]->m_position.y < 506)
				{
					_select += 1;
					_UI[0]->ChangePos(_UI[0]->m_position.x, _UI[0]->m_position.y + 89);
				}
			break;
		case SDLK_e:
			Mix_PlayChannel(-1, soundEffect, 0);
			if (_menu == 0)
			{
				if (_select == 0)
				{
					GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL1, 0, 0, 0, 3);
				}
				else if (_select == 1)
				{
					_menu = 1;
					m_background_texture[2]->LoadFromFile("Images/Layer041.png");
				}
				else if (_select == 2)
				{
					_menu = 2;
					m_background_texture[2]->LoadFromFile("Images/Layer043.png");
					_UI[0]->ChangePos(_UI[0]->m_position.x, 506);
					_select = 4;
				}
				else if (_select == 3)
				{
					m_background_texture[2]->LoadFromFile("Images/Layer042.png");
					_UI[0]->ChangePos(_UI[0]->m_position.x, 506);
					_select = 4;
					_menu = 3;
				}
				else if (_select == 4)
				{
					
					GameScreenManager::GetInstance()->QuitSelected();
				}
			}
			else if (_menu == 1) // Level Select
			{
				if (_select == 0)
				{
					GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL1, 0, 3, 0, 3);
				}
				else if (_select == 1)
				{
					GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL2, 0, 3, 0, 3);
				}
				else if (_select == 2)
				{
					GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL3, 0, 3, 0, 3);
				}
				else if (_select == 4)
				{
					_menu = 0;
					m_background_texture[2]->LoadFromFile("Images/Layer04.png");
				}
			}
			else if (_menu == 3 || _menu == 2)
			{
				_menu = 0;
				m_background_texture[2]->LoadFromFile("Images/Layer04.png");
			}
			else if (_menu == 4)
			{
				quitSelected = true;
			}
			break;
		}
		break;	
	}
}

bool GameScreenLevelMenu::SetUpLevel()
{

	//load texture
	m_background_texture.push_back( new Texture2D(m_renderer));
	if (!m_background_texture[0]->LoadFromFile("Images/Layer01.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	m_background_texture.push_back(new Texture2D(m_renderer));
	if (!m_background_texture[1]->LoadFromFile("Images/Layer02.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	m_background_texture.push_back(new Texture2D(m_renderer));
	if (!m_background_texture[2]->LoadFromFile("Images/Layer04.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	CreateUI();
	CreateText();
}

void GameScreenLevelMenu::CreateUI()
{
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(-20, 150), Vector2D(0, 0), "Images/Gold_Coin_UI.png", Vector2D(64, 64), false,4,1)); // Coin Select
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(350, 630), Vector2D(0, 0), "Images/Press_E.png", Vector2D(150, 11), false,10,2)); // Press E
}
void GameScreenLevelMenu::CreateText()
{
	LoadFile();
	std::string name;
	std::string show;
	int score;
	std::fstream File;
	File.open("Maps/Scores.txt");
	// Lazy fix for spacing on 10
	show = "";
	show.append("10 ");
	File >> score;
	File >> name;
	show.append(name);
	show.append(" ");
	show.append(std::to_string(score));
	_HST.push_back(new Text(20, show, { 255,255,255 }, m_renderer)); // Level
	for(int i=9;i>0;i--)
	{
		show = "";
		show.append(std::to_string(i));
		show.append("  ");
		File >> score;
		File >> name;
		show.append(name);
		show.append(" ");
		show.append(std::to_string(score));
		_HST.push_back(new Text(20, show , { 255,255,255 }, m_renderer)); // Level
	}
	File.close();
}

void GameScreenLevelMenu::LoadFile()
{
	std::string name;
	int score;
	std::fstream File;
	File.open("Maps/Scores.txt", std::ios::in);
	int len = 0;
	while (File >> score)
	{
		File >> name;
		FileScore[len] = score;
		FileName[len] = name;
		len++;
	}
	if (len < 10)
	{
		for (int i = len; i < 10; i++)
		{
			FileScore[i] = 0;
			FileName[i] = "AAA";
		}
	}
	File.close();
	SaveFile();
}

void GameScreenLevelMenu::SaveFile()
{
	std::fstream File;
	File.open("Maps/Scores.txt", std::ios::out);
	for (int i = 0; i < 10; i++)
	{
		File << FileScore[i] << " " << FileName[i]<<" ";
	}
	File.close();
}