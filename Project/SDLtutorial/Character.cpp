#include "Character.h"
#include "Texture2D.h"
Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map)
{
	m_attack = false;
	jumpSound = Mix_LoadWAV("Audio/Jump.wav");
	attackSound = Mix_LoadWAV("Audio/Sword.wav");
	m_alive = true;
	m_current_level_map = map;
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(renderer);
	m_jump_force = INITIAL_JUMP_FORCE * 2;
	m_speed = 200;
	m_facing_direction = FACING_RIGHT;
	t_startHeight = 96;
	t_startWidth = 0;
	m_collision_radius = 15.0f;
	m_moving_left = false;
	m_moving_right = false;
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "can't load character texture!" << std::endl;
	}
	std::cout << "Character Created" << std::endl;
}

Character::~Character()
{
	m_renderer = nullptr;
	m_texture->Free();
}
void Character::Render()
{
	SDL_Rect portion_of_sprite = { t_startWidth,t_startHeight,TILE_WIDTH, TILE_HEIGHT };
	t_startHeight = TILE_HEIGHT;
	t_startWidth = TILE_WIDTH * int(SDL_GetTicks() / FRAMESPEED % 4) + 96 * 2;
	//determine where you want it drawn
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y),TILE_WIDTH * 2,TILE_HEIGHT * 2 };

	//then draw it facing the correct direction
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	//collision position variables
	int centralX_position1 = (int)(m_position.x + 75) / 64;
	int centralX_position2 = (int)(m_position.x + 115) / 64;
	int foot_position = (int)(m_position.y + 64) / 64;
	//deal with jumping first
	if (m_jumping)
	{
		//adjust position
		m_position.y -= m_jump_force * deltaTime;

		//reduce jump force
		m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;

		//is jump force 0?
		if (m_jump_force <= 0.0f)
			m_jumping = false;
	}
	//deal with gravity
	if (m_current_level_map->GetTileAt(foot_position + 1, centralX_position1, 2) != 1 && m_current_level_map->GetTileAt(foot_position + 1, centralX_position2, 2) != 1 )
	{
		AddGravity(deltaTime);
	}
	else
	{
		m_faling = false;
		m_can_jump = true;
	}

	if (m_jumping)
	{
		t_startHeight = 96 * 2;
		t_startWidth = 3 * 96;
	}

	//handle the events
	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right)
	{
		MoveRight(deltaTime);
	}
	if (m_attack)
	{
		t_startHeight = 1*TILE_WIDTH;
		t_startWidth = 0;
		m_attack_time -= deltaTime;
		if (m_attack_time <= 0)
		{
			m_attack = false;
		}
	}

}

void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}

Vector2D Character::GetPosition()
{
	return m_position;
}

void Character::MoveLeft(float deltaTime)
{
	if (!(m_jumping || m_faling))
	{
		t_startHeight = 0;
		t_startWidth = TILE_WIDTH * int(SDL_GetTicks() / FRAMESPEED % 6);
	}
	m_facing_direction = FACING_LEFT;
	m_position.x -= m_speed * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	if (!(m_jumping || m_faling))
	{
		t_startHeight = 0;
		t_startWidth = TILE_WIDTH * int(SDL_GetTicks() / FRAMESPEED % 6);
	}
	m_facing_direction = FACING_RIGHT;
	m_position.x += m_speed * deltaTime;
}

void Character::AddGravity(float deltaTime)
{
	if (m_position.y + 128 <= SCREEN_HEIGHT)
	{
		t_startHeight = 96 * 2;
		t_startWidth = 4 * 96;
		m_position.y += GRAVITY * deltaTime;
		m_can_jump = false;
		m_faling = true;
	}
}

void Character::Jump()
{
	if (m_can_jump) {
		Mix_PlayChannel(-1, jumpSound, 0);
		m_can_jump = false;
		std::cout << "F" << std::endl;
		m_jump_force = INITIAL_JUMP_FORCE;
		m_jumping = true;
	}
}

float Character::GetCollisionRadius()
{
	return m_collision_radius;
}

Rect2D Character::GetCollisionBox()
{
	return Rect2D(m_position.x + 75, m_position.y + 75,
		40, 40);
}
void Character::Attack()
{
	Mix_PlayChannel(-1, attackSound, 0);
	m_attack = true;
	m_attack_time = ATTACK_TIME;
}