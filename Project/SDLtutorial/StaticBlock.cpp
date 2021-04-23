#include "StaticBlock.h"
StaticBlock::StaticBlock(SDL_Renderer* renderer, LevelMap* map,Vector2D position ,Vector2D texPos)
{
	_imagePath = "Images/Tileset.png";
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(_imagePath.c_str()))
	{
		std::cout << "Failed to load texture." << std::endl;
		return;
	}

	m_level_map = map;
	m_position = position;
	t_position = texPos;
	t_dimension.x = 32;
	t_dimension.y = 32;
}

StaticBlock::~StaticBlock()
{
	m_renderer = nullptr;
	delete m_texture;
	m_texture = nullptr;
	m_level_map = nullptr;
}


void StaticBlock::Render()
{
		SDL_Rect portion_of_sprite = { t_position.x, t_position.y, t_dimension.x, t_dimension.y };

		//determine where to draw it
		SDL_Rect dest_rect = {
	static_cast<int>(m_position.x), static_cast<int>(m_position.y), t_dimension.x * 2, t_dimension.y * 2
		};

		//draw the sprite
		m_texture->Render(portion_of_sprite, dest_rect, SDL_FLIP_NONE);

}
Rect2D StaticBlock::GetCollisionBox()
{
	return Rect2D(m_position.x, m_position.y,
		t_dimension.x * 2, t_dimension.y * 2);
}