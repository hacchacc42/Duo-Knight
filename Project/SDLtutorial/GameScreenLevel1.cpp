 #include "GameScreenLevel1.h"
#include "GameScreenManager.h"
#include <iostream>
#include <fstream>
/*
	LEVEL 1-1

	UI
	PLAYER 2
	STATIC BLOCKS
	COINS
*/
GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, int SP_S, int SP_L) : GameScreen(renderer)
{
	_SP_L = SP_L;
	_SP_S = SP_S;
	Mix_Chunk* newLevelSound = Mix_LoadWAV("Audio/NewLevel.wav");
	Mix_PlayChannel(-1, newLevelSound, 0);
	_s_coinCollect = Mix_LoadWAV("Audio/Coin.wav");
	m_level_map = nullptr;
	SetUpLevel();
}
GameScreenLevel1::~GameScreenLevel1()
{
	delete m_background_texture;
	m_background_texture = nullptr;

	delete my_character2;
	my_character2 = nullptr;

	for (int i = 0; i < m_static_block.size(); i++)
	{
		delete m_static_block.at(i);
	}
	m_static_block.clear();

	for (int i = 0; i < _coin.size(); i++)
	{
		delete _coin.at(i);
	}
	_coin.clear();

	for (int i = 0; i < _UI.size(); i++)
	{
		delete _UI.at(i);
	}
	_UI.clear();

	delete _text;
	_text = nullptr;

	delete _text2;
	_text2 = nullptr;

	Mix_FreeChunk(_s_coinCollect);
}
void GameScreenLevel1::Render()
{
	m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE); // BackGround
	for (int i = 0; i < m_static_block.size(); i++) // Stiatic Blocks
	{
		m_static_block[i]->Render();
	}
	for (int i = 0; i < _coin.size(); i++) // Coins
	{
		_coin[i]->Render();
	}
	my_character2->Render(); // Player 2
	for (int i = 0; i < _UI.size(); i++) // UI
	{
		_UI[i]->Render(my_character2->m_lives);
	}
	_text->Render(400,23); // Level
	_text2->loadFont(std::to_string(my_character2->_points) , { 255,255,255 }); // Update Coins
	_text2->Render(100, SCREEN_HEIGHT-40); // Coins
}

void GameScreenLevel1::TileCollision()
{
	PlayerCollision(my_character2);
}

void GameScreenLevel1::PlayerCollision(Character *player) {
	for (int i = 0; i < m_static_block.size(); i++) // Static Blocks Collision
	{
		if (Collision::Instance()->Box(player->GetCollisionBox(), m_static_block[i]->GetCollisionBox()))
		{
			if (player->m_moving_right)
			{
				player->SetPosition(Vector2D(player->GetPosition().x - 2, player->GetPosition().y));
			}
			if (player->m_moving_left)
			{
				player->SetPosition(Vector2D(player->GetPosition().x + 2.5, player->GetPosition().y));
			}
			//collided while jumping
			if (player->IsJumping())
			{
				player->SetPosition(Vector2D(player->GetPosition().x, player->GetPosition().y + 2));
				player->CancelJump();
			}
		}
	}
	for (int i = 0; i < _coin.size(); i++)  // Coin Collision
	{
		if (Collision::Instance()->Box(player->GetCollisionBox(), _coin[i]->GetCollisionBox()))
		{
			Mix_PlayChannel(-1, _s_coinCollect, 0);
			my_character2->_points += _coin[i]->_points;
			_coin.erase(_coin.begin() + i);
		}
	}
	if (player->GetCollisionBox().x + player->GetCollisionBox().width > SCREEN_WIDTH) // Border Right
	{
		GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL2, 0, 3, my_character2->_points, my_character2->m_lives);
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	my_character2->Update(deltaTime, e);
	TileCollision();
}

bool GameScreenLevel1::SetUpLevel()
{

	//load texture
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackGround.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	SetLevelMap(); // Reading map from file and setting it up
	my_character2 = new C_Player_2(m_renderer, "Images/Player2.png", my_char2_pos, m_level_map,_SP_S,_SP_L); // Player 2
	CreateUI();
	CreateText();

}
void GameScreenLevel1::SetLevelMap()
{
	int x; // used for reading map
	int map[MAP_HEIGHT][MAP_WIDTH] = { 0 };
	std::fstream File;
	File.open("Maps/map1-1.txt");
	int cntw = 0; // keeping track of tiles
	while (!File.eof())
	{
		File >> x;
		map[cntw / MAP_WIDTH][cntw % MAP_WIDTH] = x;
		cntw++;
	}
	File.close();
	for (int i = 1; i < MAP_HEIGHT - 1; i++)
	{
		for (int j = 1; j < MAP_WIDTH - 1; j++)
		{
			if (map[i][j] == 1) // TILE
			{
					bool left = false, right = false, up = false, down = false; // PLUS TILE MESHER
					if (map[i][j - 1] == 1)
					{
						left = true;
					}
					if (map[i][j + 1] == 1)
					{
						right = true;
					}
					if (map[i + 1][j] == 1)
					{
						up = true;
					}
					if (map[i - 1][j] == 1)
					{
						down = true;
					}
					if (up)
					{
						if (down)
						{
							if (right)
							{
								if (left)
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(9 * 32, 3 * 32)));
								}
								else
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(12 * 32, 1 * 32)));
								}
							}
							else if (left)
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(12 * 32, 0 * 32)));
							}
							else
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(4 * 32, 1 * 32)));
							}
						}
						else if (right)
						{
							if (left)
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(13 * 32, 0 * 32)));
							}
							else
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(15 * 32, 0 * 32)));
							}
						}
						else if (left)
						{
							m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(16 * 32, 0 * 32)));
						}
						else
						{
							m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(4 * 32, 0 * 32)));
						}
					}
					else
					{
						if (down)
						{
							if (right)
							{
								if (left)
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(13 * 32, 1 * 32)));
								}
								else
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(15 * 32, 1 * 32)));
								}
							}
							else if (left)
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(16 * 32, 1 * 32)));
							}
							else
							{
								m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(4 * 32, 2 * 32)));
							}
						}
						else
						{
							if (right)
							{
								if (left)
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(1 * 32, 4 * 32)));
								}
								else
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(0 * 32, 4 * 32)));
								}
							}
							else
							{
								if (left)
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(2 * 32, 4 * 32)));
								}
								else
								{
									m_static_block.push_back(new StaticBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64), Vector2D(4 * 32, 4 * 32)));
								}
							}
						}
					}
			}
			if (map[i][j] == 4) // GOLD COINS
			{
				_coin.push_back(new Coin(m_renderer, m_level_map, Vector2D(j * 64-96, i * 64-96), Vector2D(0, 0), "Images/Gold_Coin_R.png",3));
			}
			if (map[i][j] == 5) // SILVER COINS
			{
				_coin.push_back(new Coin(m_renderer, m_level_map, Vector2D(j * 64 - 96, i * 64 - 96), Vector2D(0, 0), "Images/Silver_Coin_R.png",1));
			}
			if (map[i][j] == 2) // Character 2
			{
				my_char2_pos = Vector2D(j * 64 -124, i * 64-64);
			}
		}
	}
	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}

	//set the new one
	m_level_map = new LevelMap(map);

}

void GameScreenLevel1::CreateUI()
{
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16, SCREEN_HEIGHT-97), Vector2D(0, 0), "Images/Silver_Coin_UI.png",Vector2D(64, 64),false,4,1)); // Player 2 Coin
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16+124, SCREEN_HEIGHT-55), Vector2D(0, 0), "Images/Player2_UI.png",Vector2D(60, 21),true,4,1)); // Player 2 Life
}
void GameScreenLevel1::CreateText()
{
	_text = new Text(20, "Level 1-1", { 255,255,255 }, m_renderer); // Level
	_text2 = new Text(20, std::to_string(11), { 255,255,255 }, m_renderer); // Char 2 points
}