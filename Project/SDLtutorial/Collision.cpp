#include "Collision.h"
#include <math.h>
//initialise instance to nullptr
Collision* Collision::m_instance = nullptr;
Collision* Collision::Instance()
{
	if (m_instance==nullptr)
	{
		m_instance = new Collision();
	}

	return m_instance;

}
Collision::Collision()
{

}
bool Collision::Circle(Character* character1, Character* character2)
{
	Vector2D vec = Vector2D((character1->GetPosition().x - character2->GetPosition().x),
		(character1->GetPosition().y - character2->GetPosition().y));
	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double combined_distance = (character1->GetCollisionRadius() + character2->GetCollisionRadius());
	return distance < combined_distance;
}
bool Collision::Box(Rect2D rect1, Rect2D rect2)
{
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y)
	{
		return true;
	}
	return false;

}
Collision::~Collision()
{
	m_instance = nullptr;
}