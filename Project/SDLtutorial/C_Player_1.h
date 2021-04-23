#pragma once
#include "Character.h"
class C_Player_1 : public Character
{
public:
	C_Player_1(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, int points, int lives);
	void Update(float deltaTime, SDL_Event e);
	~C_Player_1();
};
