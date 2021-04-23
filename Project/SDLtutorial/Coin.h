#pragma once
#include <SDL.h>
#include "StaticBlock.h"

class Coin : public StaticBlock
{
public:
	Coin(SDL_Renderer* render, LevelMap* map, Vector2D position, Vector2D texPos, std::string imagePath,int points);
	~Coin();
	void Render();
	Rect2D GetCollisionBox();
	int _points;
};