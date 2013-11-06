#include <QuadTree.hpp>

QuadTree::QuadTree(sf::FloatRect size):
	m_size (size),
	m_NW (nullptr),
	m_NE (nullptr),
	m_SW (nullptr),
	m_SE (nullptr)
{
	m_rect.setSize(sf::Vector2f(size.width, size.height));
	m_rect.setPosition(size.left, size.top);
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineThickness(1);
	m_rect.setOutlineColor(sf::Color::Green);
	#if DEBUG
		std::cout << "(" << m_rect.getSize().x << ", " << m_rect.getSize().y << ") (" << m_rect.getPosition().x << ", " << m_rect.getPosition().y << ")\n";
	#endif
}

bool QuadTree::insert(sf::RectangleShape &object)
{
	#if DEBUG
		//std::cout << "inserting\n";
	#endif
	if (!m_size.contains(object.getPosition()))
	{
		return false;
	}

	if (m_actualSize < m_maxSize)
	{
		m_objects.push_back(&object);
		++m_actualSize;
	}
	else
	{
		if (m_NW == nullptr)
			subdivide();

		if (m_NW->insert(object) || m_NE->insert(object) || m_SW->insert(object) || m_SE->insert(object))
			return true;
	}

	return false;
}

sf::FloatRect QuadTree::getRect()
{
	return m_size;
}

std::vector<sf::RectangleShape*> QuadTree::getObjects(sf::Vector2f pos)
{
	if (m_NW != nullptr)
	{
		std::vector<sf::RectangleShape*> objects;
		if (m_NW->getRect().contains(pos))
		{
			objects = m_NW->getObjects(pos);
		}
		else if (m_NE->getRect().contains(pos))
		{
			objects = m_NE->getObjects(pos);
		}
		else if (m_SW->getRect().contains(pos))
		{
			objects = m_SW->getObjects(pos);
		}
		else if (m_SE->getRect().contains(pos))
		{
			objects = m_SE->getObjects(pos);
		}
		return objects;
	}
	else
	{
		return m_objects;
	}
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

	for (short i=0; i<4; ++i)
	{
		sf::Vector2f objectPos(m_objects[i]->getPosition());
		if (m_NW->getRect().contains(objectPos))
		{
			m_NW->insert(*m_objects[i]);
		}
		else if (m_NE->getRect().contains(objectPos))
		{
			m_NE->insert(*m_objects[i]);
		}
		else if (m_SW->getRect().contains(objectPos))
		{
			m_SW->insert(*m_objects[i]);
		}
		else if (m_SE->getRect().contains(objectPos))
		{
			m_SE->insert(*m_objects[i]);
		}
	}

	return true;
}

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
}