#pragma once
#include "StaticBlock.h"

class UI : public StaticBlock
{
public:
	UI(SDL_Renderer* render, LevelMap* map, Vector2D position, Vector2D texPos, std::string imagePath, Vector2D dimensions, bool staticV,int frames,float frameSpeedMultiplier);
	~UI();
	void Render(int lives);
	void ChangePos(int posX, int posY);
private:
	bool _static;
	int _frames;
	float _FSP;
};
