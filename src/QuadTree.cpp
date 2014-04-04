#include "QuadTree.hpp"

namespace xal
{

QuadTree::QuadTree():
	m_actualObjects (0),
	m_NW (nullptr),
	m_NE (nullptr),
	m_SW (nullptr),
	m_SE (nullptr)
{ }

QuadTree::QuadTree(sf::FloatRect size):
	m_actualObjects (0),
	m_NW (nullptr),
	m_NE (nullptr),
	m_SW (nullptr),
	m_SE (nullptr)
{
	setSize(size);
}

void QuadTree::setSize(sf::FloatRect size)
{
	m_size = size;

	#if DEV
		m_rect.setSize(sf::Vector2f(size.width, size.height));
		m_rect.setPosition(size.left, size.top);
		m_rect.setFillColor(sf::Color::Transparent);
		m_rect.setOutlineThickness(1.f);
		m_rect.setOutlineColor(sf::Color::Green);
		#if CONSOLE
			//std::cout << "(" << m_rect.getSize().x << ", " << m_rect.getSize().y << ") (" << m_rect.getPosition().x << ", " << m_rect.getPosition().y << ")\n";
		#endif
	#endif
}

bool QuadTree::insert(sf::Sprite &object, bool isStatic)
{
	if (!m_size.intersects(object.getGlobalBounds()))
	{
		return false;
	}

	if (m_actualObjects < m_maxObjects)
	{
		if (isStatic) m_staticObjects.push_back(&object);
		else m_dynamicObjects.push_back(&object);
		++m_actualObjects;

		#if TILESRECT
			if (isStatic)
			{
				sf::RectangleShape staticRect;
				staticRect.setSize(sf::Vector2f(object.getGlobalBounds().width, object.getGlobalBounds().height));
				staticRect.setPosition(object.getPosition());
				staticRect.setFillColor(sf::Color::Transparent);
				staticRect.setOutlineColor(sf::Color::Red);
				staticRect.setOutlineThickness(1.f);
				m_staticRects.push_back(staticRect);
			}
		#endif

		return true;
	}
	else
	{
		if (m_NW == nullptr)
			subdivide();

		int verifier = 0;
		if (m_NW->insert(object, isStatic)) ++verifier;
		if (m_NE->insert(object, isStatic)) ++verifier;
		if (m_SW->insert(object, isStatic)) ++verifier;
		if (m_SE->insert(object, isStatic)) ++verifier;

		if (verifier>0) return true;
	}

	return false;
}

sf::FloatRect QuadTree::getRect()
{
	return m_size;
}

std::vector<sf::Sprite*> QuadTree::getStaticObjectsWhereIs(sf::FloatRect object)
{
	if (m_size.intersects(object))
	{
		if (m_NW != nullptr)
		{
			std::vector<sf::Sprite*> objects;
			if (m_NW->getRect().intersects(object))
			{
				std::vector<sf::Sprite*> subObjects = m_NW->getStaticObjectsWhereIs(object);
				objects.insert(objects.end(), subObjects.begin(), subObjects.end());
			}
			if (m_NE->getRect().intersects(object))
			{
				std::vector<sf::Sprite*> subObjects = m_NE->getStaticObjectsWhereIs(object);
				objects.insert(objects.end(), subObjects.begin(), subObjects.end());
			}
			if (m_SW->getRect().intersects(object))
			{
				std::vector<sf::Sprite*> subObjects = m_SW->getStaticObjectsWhereIs(object);
				objects.insert(objects.end(), subObjects.begin(), subObjects.end());
			}
			if (m_SE->getRect().intersects(object))
			{
				std::vector<sf::Sprite*> subObjects = m_SE->getStaticObjectsWhereIs(object);
				objects.insert(objects.end(), subObjects.begin(), subObjects.end());
			}
			return objects;
		}
		else
		{
			std::cout << "(" << m_size.left << ", " << m_size.top << ", " << m_size.width << ", " << m_size.height << ", " << m_staticObjects.size() << ")";
			return m_staticObjects;
		}
	}
	else return std::vector<sf::Sprite*>();
}

bool QuadTree::subdivide()
{
	if (m_NW != nullptr || m_NE != nullptr || m_SW != nullptr || m_SE != nullptr)
		return false;

	float width = m_size.width/2;
	float height = m_size.height/2;
	float x = m_size.left;
	float y = m_size.top;
	m_NW = new QuadTree(sf::FloatRect(x,y,width, height));
	m_NE = new QuadTree(sf::FloatRect(x+width,y,width, height));
	m_SW = new QuadTree(sf::FloatRect(x,y+height,width, height));
	m_SE = new QuadTree(sf::FloatRect(x+width,y+height,width, height));

	for (std::size_t i=0; i<m_staticObjects.size(); ++i)
	{
		sf::FloatRect objectRect = m_staticObjects[i]->getGlobalBounds();
		if (m_NW->getRect().intersects(objectRect))
		{
			m_NW->insert(*m_staticObjects[i], true);
		}
		if (m_NE->getRect().intersects(objectRect))
		{
			m_NE->insert(*m_staticObjects[i], true);
		}
		if (m_SW->getRect().intersects(objectRect))
		{
			m_SW->insert(*m_staticObjects[i], true);
		}
		if (m_SE->getRect().intersects(objectRect))
		{
			m_SE->insert(*m_staticObjects[i], true);
		}
	}

	for (std::size_t i=0; i<m_dynamicObjects.size(); ++i)
	{
		sf::FloatRect objectRect = m_dynamicObjects[i]->getGlobalBounds();
		if (m_NW->getRect().intersects(objectRect))
		{
			m_NW->insert(*m_dynamicObjects[i], false);
		}
		if (m_NE->getRect().intersects(objectRect))
		{
			m_NE->insert(*m_dynamicObjects[i], false);
		}
		if (m_SW->getRect().intersects(objectRect))
		{
			m_SW->insert(*m_dynamicObjects[i], false);
		}
		if (m_SE->getRect().intersects(objectRect))
		{
			m_SE->insert(*m_dynamicObjects[i], false);
		}
	}
	m_staticObjects.clear();
	m_dynamicObjects.clear();

	#if TILESRECT
		m_staticRects.clear();
	#endif

	return true;
}

#if DEV
void QuadTree::draw(sf::RenderWindow &window)
{
	window.draw(m_rect);

	if (m_NW != nullptr)
	{
		m_NW->draw(window);
		m_NE->draw(window);
		m_SW->draw(window);
		m_SE->draw(window);
	}

	#if TILESRECT
		if (!m_staticRects.empty())
		{
			for (std::size_t i=0; i<m_staticRects.size(); ++i)
			{
				window.draw(m_staticRects[i]);
			}
		}
	#endif
}
#endif

}