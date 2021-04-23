#include "UI.h"
UI::UI(SDL_Renderer* renderer, LevelMap* map, Vector2D position, Vector2D texPos, std::string imagePath, Vector2D dimensions,bool staticV,int frames,float frameSpeedMultiplier) :StaticBlock(renderer, map, position, texPos)
{
	_FSP = frameSpeedMultiplier;
	_frames = frames;
	_static = staticV;
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture." << std::endl;
		return;
	}
	t_position.x = 0;
	t_position.y = 0;
	t_dimension.x = dimensions.x;
	t_dimension.y = dimensions.y;
}

UI::~UI()
{
}

void UI::Render(int lives)
{
	if (!_static)
	{
		t_position.x = t_dimension.x * (int(SDL_GetTicks() / (FRAMESPEED/_FSP)) % _frames);
	}
	else
	{
		t_position.y = 23 * (3-lives);
	}
	StaticBlock::Render();
}

void UI::ChangePos(int posX, int posY)
{
	m_position.x = posX;
	m_position.y = posY;
}