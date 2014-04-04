#ifndef XAL_QUADTREE_HPP
#define XAL_QUADTREE_HPP

#include <SFML/Graphics.hpp>

#include <vector>

#include "Global.hpp"

namespace xal
{

class QuadTree
{
public:
	QuadTree();
	QuadTree(sf::FloatRect size);
	void setSize(sf::FloatRect size);

	bool insert(sf::Sprite &object, bool isStatic);

	bool updateDynamicObjects();

	sf::FloatRect getRect();

	std::vector<sf::Sprite*> getStaticObjectsWhereIs(sf::FloatRect object);

	#if DEV
		void draw(sf::RenderWindow &window);
	#endif
private:
	static const uint m_maxObjects = 4;
	uint m_actualObjects;

	sf::FloatRect m_size;
	std::vector<sf::Sprite*> m_staticObjects;
	std::vector<sf::Sprite*> m_dynamicObjects;

	#if DEV
		sf::RectangleShape m_rect;
	#endif

	#if TILESRECT
		std::vector<sf::RectangleShape> m_staticRects;
	#endif

	QuadTree* m_NW;
	QuadTree* m_NE;
	QuadTree* m_SW;
	QuadTree* m_SE;

	bool subdivide();
};

}

#endif