#include "Coin.h"
Coin::Coin(SDL_Renderer* renderer, LevelMap* map, Vector2D position, Vector2D texPos, std::string imagePath,int points) :StaticBlock(renderer, map, position, texPos)
{
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture." << std::endl;
		return;
	}
	t_dimension.x = 64;
	t_dimension.y = 64;
	_points = points;
}

Coin::~Coin()
{
}

void Coin::Render()
{
	t_position.x = t_dimension.x * (int(SDL_GetTicks() / (FRAMESPEED/1.5)) % 4);
	StaticBlock::Render();
}
Rect2D Coin::GetCollisionBox()
{
	return Rect2D(m_position.x + 32, m_position.y + 32,
		t_dimension.x - 32, t_dimension.y - 32);
}