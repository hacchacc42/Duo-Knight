#pragma once
#include <SDL.h>
#include "Commons.h"
#include "constants.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include <iostream>
#include <string>

class Texture2D;

class PowBlock
{
public:
	PowBlock(SDL_Renderer* render, LevelMap* map, Vector2D position);
	~PowBlock();
	void Render();
	Rect2D GetCollisionBox();
	void TakeHit();
	bool IsAvailable() { return m_num_hits_left > 0; }
	Vector2D m_position;
private:
	SDL_Renderer* m_renderer;
	Texture2D* m_texture;
	LevelMap* m_level_map;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_num_hits_left;

};

