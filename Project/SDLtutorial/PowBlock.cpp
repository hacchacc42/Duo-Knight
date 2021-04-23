#include "PowBlock.h"

#include <iostream>;
PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map, Vector2D position)
{
	std::string imagePath = "Images/PowBlock.png";
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture." << std::endl;
		return;
	}

	m_level_map = map;
	m_single_sprite_w = m_texture->GetWidth() / 3; //there are three images in this sprite sheet in a row
	m_single_sprite_h = m_texture->GetHeight();
	m_num_hits_left = 3;
	m_position = position;

}

PowBlock::~PowBlock()
{
	m_renderer = nullptr;
	delete m_texture;
	m_texture = nullptr;
	m_level_map = nullptr;
}

void PowBlock::TakeHit()
{
	m_num_hits_left--;
	if (m_num_hits_left >= 0)
	{
		//m_num_hits_left = 0;
		m_level_map->GetTileAt(m_position.x/64, m_position.y/64,0);
	}
}
void PowBlock::Render()
{
	if (m_num_hits_left > 0)
	{
		//get the portion of the sheet we want to draw
		int left = m_single_sprite_w * (m_num_hits_left - 1);

		//				xPos,	yPos, sprite sheet width,sprite sheet height
		SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };

		//determine where to draw it
		SDL_Rect dest_rect = {
	static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_single_sprite_w*2, m_single_sprite_h*2
		};

		//draw the sprite
		m_texture->Render(portion_of_sprite, dest_rect, SDL_FLIP_NONE);

	}

}
Rect2D PowBlock::GetCollisionBox()
{

	return Rect2D(m_position.x, m_position.y,
	64, 64);
}