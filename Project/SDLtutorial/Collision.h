#pragma once
#include "Character.h"
class Character;
class Collision
{
private:
	Collision();
	static Collision* m_instance;
public:
	~Collision();

	static Collision* Instance();

	bool Circle(Character* character1, Character* character2);
	bool Box(Rect2D rect1, Rect2D rect2);
};

