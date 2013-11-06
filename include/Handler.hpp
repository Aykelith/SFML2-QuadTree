#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <SFML/Graphics.hpp>

#include <vector>

#include "QuadTree.hpp"

class Handler
{
public:
	Handler();

private:
	sf::RenderWindow m_window;

	std::vector<sf::RectangleShape> m_objects;
	static const uint m_objectSize = 20;

	Quadtree m_qt;

	void run();
	sf::Vector2f getNoCollisionPosition(sf::Vector2f pos);
};

#endif