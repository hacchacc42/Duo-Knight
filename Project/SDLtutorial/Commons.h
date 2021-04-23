#pragma once
struct Vector2D
{
public:

	float x;
	float y;
	Vector2D()
	{
		x = 0;
		y = 0;
	}
	Vector2D(float initial_x,float initial_y)
	{
		x = initial_x;
		y = initial_y;
	}

};
struct Rect2D
{
public:
	Rect2D(float x_pos, float y_pos, float width, float height)
	{
		x = x_pos;
		y = y_pos;
		this->width = width;
		this->height = height;
	}
	float x;
	float y;
	float width;
	float height;
};
enum SCREENS
{
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_LEVEL3,
	SCREEN_LEVELMENU,
	SCREEN_WIN,
	SCREEN_LOSE,
};
enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
};