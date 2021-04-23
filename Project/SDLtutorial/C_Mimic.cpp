#include "C_Mimic.h"
#include "Texture2D.h"
#include <iostream>
C_Mimic::C_Mimic(SDL_Renderer* renderer, std::string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map)
{
	std::cout << "KOOPA CREATED" << std::endl;
	m_facing_direction = start_facing;
	if (m_facing_direction == FACING_LEFT)
	{
		m_moving_left = true;
		m_moving_right = false;
	}
	else if (m_facing_direction == FACING_RIGHT)
	{
		m_moving_right = true;
		m_moving_left = false;
	}
	m_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	m_single_sprite_w = 64;
	m_single_sprite_h = 64;
}
void C_Mimic::TakeDamage()
{
	m_injured = true;
	m_injured_time = INJURED_TIME;
	Jump();
}
void C_Mimic::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE;
		m_jumping = true;
		m_can_jump = false;
	}
}

void C_Mimic::SwitchDirections()
{
	//m_facing_direction to opposite direction
	if (m_facing_direction == FACING_RIGHT)
	{
		m_facing_direction = FACING_LEFT;
		m_moving_left = true;
		m_moving_right = false;
	}
	else if(m_facing_direction == FACING_LEFT)
	{
		m_facing_direction = FACING_RIGHT;
		m_moving_right = true;
		m_moving_left = false;
	}
}

void C_Mimic::FlipRightwayUp()
{
	SwitchDirections();
	m_injured = false;
	Jump();
}
void C_Mimic::Render()
{
	//variable to hold the left position of the sprite we want to draw
	int left = 64 * (int(SDL_GetTicks() / FRAMESPEED) % 12);

	//if injured move the left position to be the left position of the second image of the sprite sheet
	if (m_injured)
	{
		left = 64 * 12;
	}

	//get the portion of the sprite sheet you want to draw
	//							   {xPos, yPos, width of sprite, height of sprite}
	SDL_Rect portion_of_sprite = { left,0,m_single_sprite_w, m_single_sprite_h };

	//determine where you want it drawn
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w*3, m_single_sprite_h*3 };

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
void C_Mimic::Update(float deltaTime, SDL_Event e)
{
	//use the code within the base class
	Character::Update(deltaTime, e);

	if(m_injured)
	{
		//we should not be moving when injured
		m_moving_right = false;
		m_moving_left = false;

		//count down the injured time
		m_injured_time -= deltaTime;

		if (m_injured_time <= 0.0)
			FlipRightwayUp();

	}
}