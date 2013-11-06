#include "Handler.hpp"

Handler::Handler():
    m_qt(sf::FloatRect(0,0,800,600))
{
	m_window.create(sf::VideoMode(800,600), "QuadTree");

	run();
}

void Handler::run()
{
	while(m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                m_window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
            	if (event.mouseButton.button == sf::Mouse::Left)
            	{
            		sf::RectangleShape box(sf::Vector2f(m_objectSize,m_objectSize));
            		box.setFillColor(sf::Color::Red);
            		box.setOutlineThickness(1);
            		box.setOutlineColor(sf::Color::Black);
            		box.setPosition(getNoCollisionPosition(mousePos));
            		m_objects.push_back(box);
                    m_qt.insert(m_objects[m_objects.size()-1]);
            	}
            	else
            	{
            		for (std::size_t i=0; i<m_objects.size(); ++i)
            		{
            			sf::FloatRect object = m_objects[i].getGlobalBounds();
            			if (object.contains(mousePos))
            			{
            				m_objects.erase(m_objects.begin()+i);
            			}
            		}
            	}
            }
        }

        m_window.clear(sf::Color::White);
        m_qt.draw(m_window);

        for (std::size_t i=0; i<m_objects.size(); ++i)
        {
        	m_window.draw(m_objects[i]);
        }

        m_window.display();
	}
}

sf::Vector2f Handler::getNoCollisionPosition(sf::Vector2f pos)
{
    std::vector<sf::RectangleShape*> objects = m_qt.getObjects(pos);
    sf::FloatRect object(pos.x, pos.y, m_objectSize, m_objectSize);
    sf::Vector2f finalPos(pos);
    for (std::size_t i=0; i<objects.size(); ++i)
    {
        if (objects[i]->getGlobalBounds().intersects(object))
        {
            sf::FloatRect halfRect(objects[i]->getGlobalBounds());
            halfRect.width /= 2;
            if (halfRect.intersects(object))
            {
                object.left = objects[i]->getGlobalBounds().left - objects[i]->getGlobalBounds().width;
            }
            else
            {
                object.left = objects[i]->getGlobalBounds().left + objects[i]->getGlobalBounds().width;
            }
            finalPos = getNoCollisionPosition(sf::Vector2f(object.left, object.top));
        }
    }
    return finalPos;
}