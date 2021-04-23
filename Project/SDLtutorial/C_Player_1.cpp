#include "C_Player_1.h"
C_Player_1::C_Player_1(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, int points, int lives) : Character(renderer, imagePath, start_position, map)
{
	std::cout << "MARIO !" << std::endl;
	_points = points;
	m_lives = lives;
}
void C_Player_1::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = true;
			break;
		case SDLK_RIGHT:
			m_moving_right = true;
			break;
		case SDLK_UP:
			if (m_can_jump)
			{
				Jump();
			}
			break;
		case SDLK_DOWN:
			Attack();
			break;
		}
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = false;
			break;
		case SDLK_RIGHT:
			m_moving_right = false;
			break;
		}
		break;
	}

	Character::Update(deltaTime, e);
}