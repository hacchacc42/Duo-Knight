#include "LevelMap.h"
/*
 * when making a map, remember:
 * 0 = empty space,	1 = blocked/occupied space
 */
#include <iostream>
LevelMap::LevelMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
	//Allocate memory for the level map
	m_map = new int* [MAP_HEIGHT-2];
	for (unsigned int i = 0; i < MAP_HEIGHT-2; i++)
	{
		m_map[i] = new int[MAP_WIDTH-2];
	}

	//populate the array
	for (int i = 0; i < MAP_HEIGHT-2;i++)
	{
		for (int j = 0; j < MAP_WIDTH-2; j++)
		{
			m_map[i][j] = map[i+1][j+1];
		}
	}
	for (int i = 0; i < MAP_HEIGHT-2; i++)
	{
		for (int j = 0; j < MAP_WIDTH-2; j++)
		{
			std::cout << m_map[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

LevelMap::~LevelMap()
{
	//delete all elements of the array
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		delete[] m_map[i];
	}
	delete[]m_map;
}

int LevelMap::GetTileAt(unsigned int h, unsigned int w,unsigned int new_value)
{
	if (h < MAP_HEIGHT && w < MAP_WIDTH)
	{
		if (new_value > 0)
		{
			return m_map[h][w];
		}
	}

	return 0;

}