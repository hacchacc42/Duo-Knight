#pragma once
#include <SDL.h>
#include "Commons.h"
#include "constants.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include <iostream>
#include <string>

class Texture2D;

class StaticBlock
{
public:
	StaticBlock(SDL_Renderer* render, LevelMap* map, Vector2D position, Vector2D texPos);
	~StaticBlock();
	virtual void Render();
	virtual Rect2D GetCollisionBox();
	Vector2D m_position;
protected:
	std::string _imagePath;
	SDL_Renderer* m_renderer;
	Vector2D t_position;
	Vector2D t_dimension;
	Texture2D* m_texture;
	LevelMap* m_level_map;
};

