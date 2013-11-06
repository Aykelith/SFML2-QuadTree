#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <SFML/Graphics.hpp>

#include <vector>

#include "Global.hpp"

class QuadTree
{
public:
	QuadTree(sf::FloatRect size);

	bool insert(sf::RectangleShape &object);
	sf::FloatRect getRect();
	std::vector<sf::RectangleShape*> getObjects(sf::Vector2f pos);

	void draw(sf::RenderWindow &window);
private:
	static const uint m_maxSize = 4;
	uint m_actualSize = 0;

	sf::FloatRect m_size;
	std::vector<sf::RectangleShape*> m_objects;

	sf::RectangleShape m_rect;

	QuadTree* m_NW;
	QuadTree* m_NE;
	QuadTree* m_SW;
	QuadTree* m_SE;

	bool subdivide();
};

#endif