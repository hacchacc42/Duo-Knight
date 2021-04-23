#include "C_Player_2.h"
C_Player_2::C_Player_2(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, int points, int lives) : Character(renderer, imagePath, start_position, map)
{
	std::cout << "LUIGI!" << std::endl;
	_points = points;
	m_lives = lives;
}
void C_Player_2::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = true;
			break;
		case SDLK_d:
			m_moving_right = true;
			break;
		case SDLK_w:
			if (m_can_jump)
			{
				Jump();
			}
			break;
		case SDLK_s:
			Attack();
			break;
		}
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = false;
			break;
		case SDLK_d:
			m_moving_right = false;
			break;
		}
		break;
	}

	Character::Update(deltaTime, e);
}