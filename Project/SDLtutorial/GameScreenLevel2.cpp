#include "GameScreenLevel2.h"
#include "GameScreenManager.h"
#include <iostream>
#include <fstream>
/*
	LEVEL 1-2

	UI
	PLAYER 1
	PLAYER 2
	STATIC BLOCKS
	POW BLOCKS
	COINS
*/
GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L) : GameScreen(renderer)
{
	_FP_L = FP_L;
	_FP_S = FP_S;
	_SP_S = SP_S;
	_SP_L = SP_L;
	Mix_Chunk* newLevelSound = Mix_LoadWAV("Audio/NewLevel.wav");
	Mix_PlayChannel(-1, newLevelSound, 0);
	_s_coinCollect = Mix_LoadWAV("Audio/Coin.wav");
	_s_powBlock = Mix_LoadWAV("Audio/PowBlock.wav");
	m_level_map = nullptr;
	SetUpLevel();
}

GameScreenLevel2::~GameScreenLevel2()
{
	delete m_background_texture;
	m_background_texture = nullptr;

	delete my_character;
	my_character = nullptr;

	delete my_character2;
	my_character2 = nullptr;

	m_pow_block.clear();

	m_static_block.clear();

}
void GameScreenLevel2::Render()
{
	//draw the enemies
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
	for (int i = 0; i < m_static_block.size(); i++)
	{
		m_static_block[i]->Render();
	}
	for (int i = 0; i < _coin.size(); i++)
	{
		_coin[i]->Render();
	}

	for (int i = 0; i < m_pow_block.size(); i++)
	{
		m_pow_block[i]->Render();
	}

	my_character2->Render();
	my_character->Render();

	for (int i = 0; i < _UI.size()-1; i++)
	{
		_UI[i]->Render(my_character2->m_lives);
	}

	_UI[3]->Render(my_character->m_lives);
	_text->Render(400, 23); // Level
	_text2->loadFont(std::to_string(my_character2->_points), { 255,255,255 }); // Update Coins Player 2
	_text3->loadFont(std::to_string(my_character->_points), { 255,255,255 }); // Update Coins Player 2
	_text2->Render(100, SCREEN_HEIGHT - 40); // Coins Coins Player 2
	_text3->Render(100+ UI_offset, SCREEN_HEIGHT - 40); // Coins Coins Player 2
}

void GameScreenLevel2::TileCollision()
{
	PlayerCollision(my_character,1);
	PlayerCollision(my_character2,2);


}
void GameScreenLevel2::PlayerCollision(Character* player,int player_id) {
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
			player->_points += _coin[i]->_points;
			_coin.erase(_coin.begin() + i);
		}
	}
	for (int i = 0; i < m_pow_block.size(); i++) //Pow Block
	{
		UpdatePOWBlock(m_pow_block[i], my_character,1);
		UpdatePOWBlock(m_pow_block[i], my_character2,2);
	}
	if (player->GetCollisionBox().x + player->GetCollisionBox().width > SCREEN_WIDTH) // Border Right
	{
		player->SetPosition(Vector2D(player->GetPosition().x - 2, player->GetPosition().y));
		if (player_id == 2)
		{
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL3, my_character->_points, my_character->m_lives, my_character2->_points, my_character2->m_lives);
		}
	}
	if (player->GetCollisionBox().x < 0) // Border Left
	{
		player->SetPosition(Vector2D(player->GetPosition().x + 1, player->GetPosition().y));
	}
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	//update character
	my_character->Update(deltaTime, e);
	my_character2->Update(deltaTime, e);
	TileCollision();
	//do the screen shake if required
	if (m_screenshake)
	{
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;

		//end shake after duration
		if (m_shake_time <= 0.0f)
		{
			m_shake_time = false;
			m_background_yPos = 0.0f;
		}
	}


}

void GameScreenLevel2::UpdatePOWBlock(PowBlock* pow, Character* player, int player_id)
{
	if (pow->IsAvailable())
	{
		if (Collision::Instance()->Box(player->GetCollisionBox(), pow->GetCollisionBox()))
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
				if (player_id == 1)
				{
					Mix_PlayChannel(-1, _s_powBlock, 0);
					DoScreenShake();
					pow->TakeHit();
				}
				player->CancelJump();
			}
		}
	}

}

bool GameScreenLevel2::SetUpLevel()
{

	//load texture
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackGround.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	//set up map
	m_screenshake = false;
	m_background_yPos = 0.0f;
	SetLevelMap();
	//set up player character
	my_character = new C_Player_1(m_renderer, "Images/Player1.png", my_char_pos, m_level_map, _FP_S, _FP_L);
	my_character2 = new C_Player_2(m_renderer, "Images/Player2.png", my_char2_pos, m_level_map, _SP_S, _SP_L);


	CreateUI();
	CreateText();

}
void GameScreenLevel2::SetLevelMap()
{
	int x;
	int map[MAP_HEIGHT][MAP_WIDTH] = { 0 };
	std::fstream File;
	File.open("Maps/map1-2.txt");
	int cntw = 0;
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
				bool left = false, right = false, up = false, down = false;
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
			else if (map[i][j] == 4) // GOLD COINS
			{
				_coin.push_back(new Coin(m_renderer, m_level_map, Vector2D(j * 64 - 96, i * 64 - 96), Vector2D(0, 0), "Images/Gold_Coin_R.png", 3));
			}
			else if (map[i][j] == 5) // SILVER COINS
			{
				_coin.push_back(new Coin(m_renderer, m_level_map, Vector2D(j * 64 - 96, i * 64 - 96), Vector2D(0, 0), "Images/Silver_Coin_R.png", 1));
			}
			else if (map[i][j] == 2) // Character 2
			{
				my_char2_pos = Vector2D(j * 64 - 124, i * 64 - 64);
			}
			else if (map[i][j] == 3) // Character 1
			{
			my_char_pos = Vector2D(j * 64 - 124, i * 64 - 64);
			}
			else if (map[i][j] == 6) // PowBlock
			{
			m_pow_block.push_back(new PowBlock(m_renderer, m_level_map, Vector2D(j * 64 - 64, i * 64 - 64)));
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

void GameScreenLevel2::DoScreenShake()
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;
}

void GameScreenLevel2::CreateUI()
{
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16, SCREEN_HEIGHT - 97), Vector2D(0, 0), "Images/Silver_Coin_UI.png", Vector2D(64, 64), false,4,1)); // Player 2 Coin
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16 + 124, SCREEN_HEIGHT - 55), Vector2D(0, 0), "Images/Player2_UI.png", Vector2D(60, 21), true,4,1)); // Player 2 Life

	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16+ UI_offset, SCREEN_HEIGHT-96), Vector2D(0, 0), "Images/Silver_Coin_UI.png", Vector2D(64, 64), false,4,1)); // Player 1 Coin
	_UI.push_back(new UI(m_renderer, m_level_map, Vector2D(16 + 124 + UI_offset, SCREEN_HEIGHT - 55), Vector2D(0, 0), "Images/Player1_UI.png", Vector2D(60, 21), true,4,1)); // Player 1 Life
}
void GameScreenLevel2::CreateText()
{
	_text = new Text(20, "Level 1-2", { 255,255,255 }, m_renderer); // Level
	_text2 = new Text(20, std::to_string(11), { 255,255,255 }, m_renderer); // Char 2 points
	_text3 = new Text(20, std::to_string(11), { 255,255,255 }, m_renderer); // Char 2 points
}