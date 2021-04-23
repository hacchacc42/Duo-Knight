#pragma once
#include "Character.h"

class C_Mimic : public Character
{
public:
	C_Mimic(SDL_Renderer* renderer, std::string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	~C_Mimic();
	void Update(float deltaTime, SDL_Event e);
	void Render() ;
	void TakeDamage();
	void Jump();
	bool GetInjured() { return m_injured; }
	void SwitchDirections();
private:
	float m_single_sprite_w;
	float m_single_sprite_h;

	bool m_injured;
	float m_injured_time;

	void FlipRightwayUp();
};

