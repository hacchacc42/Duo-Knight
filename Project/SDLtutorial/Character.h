#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include "Commons.h"
#include "constants.h"
#include "LevelMap.h"
#include <iostream>
class Texture2D;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	virtual void AddGravity(float deltaTime);
	virtual void Jump();
	bool IsJumping() { return m_jumping; }
	void CancelJump() { m_jumping = false; }
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox();
	void SetAlive(bool isAlive) { m_alive = isAlive; }
	bool GetAlive() { return m_alive; }

	bool m_moving_left;
	bool m_moving_right;
	int _points;
	int m_lives;
	bool m_attack;
	FACING m_facing_direction;
	void Attack();
protected:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	float t_startWidth;
	float t_startHeight;

	Mix_Chunk* jumpSound;
	Mix_Chunk* attackSound;

	float m_speed;

	bool m_jumping;
	bool m_faling;
	bool m_can_jump;
	float m_jump_force;

	float m_collision_radius;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);

	bool m_alive;

	float m_attack_time;

private:
	LevelMap* m_current_level_map;
};

