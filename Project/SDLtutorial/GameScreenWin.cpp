#include "GameScreenWin.h"
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
GameScreenWin::GameScreenWin(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L) : GameScreen(renderer)
{
	std::cout << "HO";
	letPos = 0;
	_FP_L = FP_L;
	_FP_S = FP_S;
	_SP_S = SP_S;
	_SP_L = SP_L;

	Name[0] = "P";
	Name[1] = "R";
	Name[2] = "S";

	soundEffect = Mix_LoadWAV("Audio/Select.wav");
	_select.x = 0;
	_select.y = 0;
	m_level_map = nullptr;
	SetUpLevel();
}
GameScreenWin::~GameScreenWin()
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
void GameScreenWin::Render()
{
	m_background_texture[0]->Render(Vector2D(0, 0), SDL_FLIP_NONE); // BackGround
	m_background_texture[1]->Render(Vector2D(0, 0), SDL_FLIP_NONE); // Texts
	if (letPos < 3)
	{
		_UI[0]->Render(0);
		_UI[1]->Render(0);
	}
	else
	{
		_UI[2]->Render(0);
		_HST[6]->Render(125, 445); // Rank
	}
	_HST[0]->Render(640, 445); // Total Points
	_HST[1]->Render(500, 445); // P1 points
	_HST[2]->Render(410, 445); // P2 points
	_HST[3]->loadFont(Name[0], { 255,255,255 }); // First Letter
	if (letPos >= 1)
	{
		_HST[3]->Render(790, 445); // Name 1
	}
	_HST[4]->loadFont(Name[1], { 255,255,255 }); // Second Letter
	if (letPos >= 2)
	{
		_HST[4]->Render(820, 445); // Name 2
	}
	_HST[5]->loadFont(Name[2], { 255,255,255 }); // Third Letter
	if (letPos >= 3)
	{
		_HST[5]->Render(850, 445); // Name 3
	}

}
void GameScreenWin::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (_UI[0]->m_position.y > 225)
			{
				_select.y -= 1;
				_UI[0]->ChangePos(_UI[0]->m_position.x, _UI[0]->m_position.y - 56);
			}
			break;
		case SDLK_s:
			if (_UI[0]->m_position.y < 337 && !(_select.y==1 && _select.x==8))
			{
				_select.y += 1;
				_UI[0]->ChangePos(_UI[0]->m_position.x, _UI[0]->m_position.y + 56);
			}
			break;
		case SDLK_d:
			if (_UI[0]->m_position.x < 659 && !(_select.y == 2 && _select.x == 7))
			{
				_select.x += 1;
				_UI[0]->ChangePos(_UI[0]->m_position.x + 48, _UI[0]->m_position.y);
			}
			break;
		case SDLK_a:
			if (_UI[0]->m_position.x > 275)
			{
				_select.x -= 1;
				_UI[0]->ChangePos(_UI[0]->m_position.x - 48, _UI[0]->m_position.y);
			}
			break;
		case SDLK_e:
			Mix_PlayChannel(-1, soundEffect, 0);
			if (letPos < 3)
			{
				if (letPos < 2)
				{
					_UI[1]->ChangePos(_UI[1]->m_position.x + 35, _UI[1]->m_position.y);
				}
				ChangeLetter(letPos);
				letPos++;
				if (letPos == 3)
				{
					_name.append(Name[0]);
					_name.append(Name[1]);
					_name.append(Name[2]);
					int _rank = Sorting(_score, _name);
					std::string _rankT;
					if (_rank == -1)
					{
						_rankT = "10+";
					}
					else
					{
						_rankT = std::to_string(10-_rank);
					}
					_HST.push_back(new Text(20, _rankT, { 255,255,255 }, m_renderer)); // Rank
				}
			}
			else
			{
				GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVELMENU, 0, 0, 0, 0);
			}
			break;
		}
		break;
	}
}

bool GameScreenWin::SetUpLevel()
{

	//load texture
	m_background_texture.push_back(new Texture2D(m_renderer));
	if (!m_background_texture[0]->LoadFromFile("Images/Layer01.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	m_background_texture.push_back(new Texture2D(m_renderer));
	if (!m_background_texture[1]->LoadFromFile("Images/HighScore.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	CreateUI();
	CreateText();
}

void GameScreenWin::CreateUI()
{
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(275, 225), Vector2D(0, 0), "Images/Select.png", Vector2D(14, 3), false, 10, 2.5)); // Select Letter
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(780, 475), Vector2D(0, 0), "Images/Select.png", Vector2D(14, 3), false, 10, 1.5)); // Letter Pos
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(350, 630), Vector2D(0, 0), "Images/Press_E.png", Vector2D(150, 11), false, 10, 2)); // Press E
}
void GameScreenWin::CreateText()
{
	int formula = 0;
	if (_FP_S > _SP_S)
	{
		formula== (10 - (_FP_S - _SP_S));
	}
	else
	{
		formula = (10 - (_SP_S - _FP_S));
	}
	if (formula < 0)
	{
		formula=0;
	}
	_score = _FP_S + _SP_S + _FP_L * 5 + _SP_L * 5 + formula; // CRAZY MATH FORMULA
	_HST.push_back(new Text(20, std::to_string(_score), { 255,255,255 }, m_renderer)); // Total Points
	_HST.push_back(new Text(20, std::to_string(_FP_S), { 255,255,255 }, m_renderer)); // P1
	_HST.push_back(new Text(20, std::to_string(_SP_S), { 255,255,255 }, m_renderer)); // P2
	_HST.push_back(new Text(20, Name[0], { 255,255,255 }, m_renderer)); // Name 1 
	_HST.push_back(new Text(20, Name[1], { 255,255,255 }, m_renderer)); // Name 2 
	_HST.push_back(new Text(20, Name[2], { 255,255,255 }, m_renderer)); // Name 3 
}

void GameScreenWin::ChangeLetter(int pos)
{
	if (_select.y == 0) // Top Row
	{
		if (_select.x == 0) // A
		{
			Name[pos] = "A";
		}
		else if (_select.x == 1) // B
		{
			Name[pos] = "B";
		}
		else if (_select.x == 2) // C
		{
			Name[pos] = "C";
		}
		else if (_select.x == 3) // D
		{
			Name[pos] = "D";
		}
		else if (_select.x == 4) // E
		{
			Name[pos] = "E";
		}
		else if (_select.x == 5) // F
		{
			Name[pos] = "F";
		}
		else if (_select.x == 6) // G
		{
			Name[pos] = "G";
		}
		else if (_select.x == 7) // H
		{
			Name[pos] = "H";
		}
		else if (_select.x == 8) // I
		{
			Name[pos] = "I";
		}
	}
	else if (_select.y == 1) // Middle Row
	{
		if (_select.x == 0) // J
		{
			Name[pos] = "J";
		}
		else if (_select.x == 1) // K
		{
			Name[pos] = "K";
		}
		else if (_select.x == 2) // L
		{
			Name[pos] = "L";
		}
		else if (_select.x == 3) // M
		{
			Name[pos] = "M";
		}
		else if (_select.x == 4) // N
		{
			Name[pos] = "N";
		}
		else if (_select.x == 5) // O
		{
			Name[pos] = "O";
		}
		else if (_select.x == 6) // P
		{
			Name[pos] = "P";
		}
		else if (_select.x == 7) // Q
		{
			Name[pos] = "Q";
		}
		else if (_select.x == 8) // R
		{
			Name[pos] = "R";
		}
	}
	else if (_select.y == 2) // Bottom Row
	{
		if (_select.x == 0) // S
		{
			Name[pos] = "S";
		}
		else if (_select.x == 1) // T
		{
			Name[pos] = "T";
		}
		else if (_select.x == 2) // U
		{
			Name[pos] = "U";
		}
		else if (_select.x == 3) // V
		{
			Name[pos] = "V";
		}
		else if (_select.x == 4) // W
		{
			Name[pos] = "W";
		}
		else if (_select.x == 5) // X
		{
			Name[pos] = "X";
		}
		else if (_select.x == 6) // Y
		{
			Name[pos] = "Y";
		}
		else if (_select.x == 7) // Z
		{
			Name[pos] = "Z";
		}
	}
}

void GameScreenWin::LoadFile()
{
	std::string name;
	int score;
	std::fstream File;
	File.open("Maps/Scores.txt", std::ios::in);
	int len = 0;
	while (File >> score)
	{
		File >> name;
		FileName[len] = name;
		FileScore[len] = score;
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
}

int GameScreenWin::Sorting(int currentScore,std::string currentName)
{
	int value = -1;
	LoadFile();
	//Bubble Sort
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10 - i; j++)
		{
			if (FileScore[j] > FileScore[j + 1]) //Swap
			{
				int temp = FileScore[j];
				FileScore[j] = FileScore[j + 1];
				FileScore[j + 1] = temp;

				std::string stemp = FileName[j];
				FileName[j] = FileName[j + 1];
				FileName[j + 1] = stemp;
			}
		}
	}
	//Checking the right position
	for (int i = 0; i < 10; i++)
	{
		if (currentScore > FileScore[i])
		{
			value++;
		}
	}
	//Shifting the others positions
	if (value != -1)
	{
		for (int k = 0; k < value; k++)
		{
			FileScore[k] = FileScore[k + 1];
			FileName[k] = FileName[k + 1];
		}
		FileScore[value] = currentScore;
		FileName[value] = currentName;
	}
	SaveFile();
	return value;
}
void GameScreenWin::SaveFile()
{
	std::fstream File;
	File.open("Maps/Scores.txt", std::ios::out);
	for (int i = 0; i < 10; i++)
	{
		std::cout << FileScore[i] << " " << FileName[i] << std::endl;
		File << FileScore[i] << " " << FileName[i] << " ";
	}
	File.close();
}