
#include "sxbCommon/Cursor.h"

SXB_NAMESPACE_BEGIN

void Cursor::handleEvent(sf::Event event)
{
    if (event.type == sf::Event::TouchBegan && event.touch.finger < SXB_MAX_FINGER )
    {
        touch[event.touch.finger].press = true;
        touch[event.touch.finger].x = event.touch.x;
        touch[event.touch.finger].y = event.touch.y;
    }
    else if (event.type == sf::Event::TouchMoved && event.touch.finger < SXB_MAX_FINGER )
    {
        touch[event.touch.finger].press = true;
        touch[event.touch.finger].deltaX = event.touch.x - touch[event.touch.finger].x;
        touch[event.touch.finger].deltaY = event.touch.y - touch[event.touch.finger].y;
        touch[event.touch.finger].x = event.touch.x;
        touch[event.touch.finger].y = event.touch.y;
    }
    else if (event.type == sf::Event::TouchEnded && event.touch.finger < SXB_MAX_FINGER )
    {
        touch[event.touch.finger].press = false;
        touch[event.touch.finger].deltaX = 0;
        touch[event.touch.finger].deltaY = 0;
        touch[event.touch.finger].x = 0;
        touch[event.touch.finger].y = 0;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button < sf::Mouse::Button::ButtonCount )
    {
        mouse[event.mouseButton.button].press = true;
        mouse[event.mouseButton.button].x = event.mouseButton.x;
        mouse[event.mouseButton.button].y = event.mouseButton.y;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button < sf::Mouse::Button::ButtonCount )
    {
        mouse[event.mouseButton.button].press = false;
        mouse[event.mouseButton.button].deltaX = 0;
        mouse[event.mouseButton.button].deltaY = 0;
        mouse[event.mouseButton.button].x = 0;
        mouse[event.mouseButton.button].y = 0;
    }
    else if ( event.type == sf::Event::MouseMoved )
    {
        mouse_moved = true;
        if (mouse[sf::Mouse::Button::Left].press)
        {
            mouse[sf::Mouse::Button::Left].deltaX = event.mouseMove.x - mouse[sf::Mouse::Button::Left].x;
            mouse[sf::Mouse::Button::Left].deltaY = event.mouseMove.y - mouse[sf::Mouse::Button::Left].y;
            mouse[sf::Mouse::Button::Left].x = event.mouseMove.x;
            mouse[sf::Mouse::Button::Left].y = event.mouseMove.y;
        }
        else if (mouse[sf::Mouse::Button::Right].press)
        {
            mouse[sf::Mouse::Button::Right].deltaX = event.mouseMove.x - mouse[sf::Mouse::Button::Right].x;
            mouse[sf::Mouse::Button::Right].deltaY = event.mouseMove.y - mouse[sf::Mouse::Button::Right].y;
            mouse[sf::Mouse::Button::Right].x = event.mouseMove.x;
            mouse[sf::Mouse::Button::Right].y = event.mouseMove.y;
        }
        else if (mouse[sf::Mouse::Button::Middle].press)
        {
            mouse[sf::Mouse::Button::Middle].deltaX = event.mouseMove.x - mouse[sf::Mouse::Button::Middle].x;
            mouse[sf::Mouse::Button::Middle].deltaY = event.mouseMove.y - mouse[sf::Mouse::Button::Middle].y;
            mouse[sf::Mouse::Button::Middle].x = event.mouseMove.x;
            mouse[sf::Mouse::Button::Middle].y = event.mouseMove.y;
        }
    }
}

void Cursor::update()
{
    if ( !mouse_moved )
    {
        if (mouse[sf::Mouse::Button::Left].press)
        {
            mouse[sf::Mouse::Button::Left].deltaX = 0;
            mouse[sf::Mouse::Button::Left].deltaY = 0;
        }
        else if (mouse[sf::Mouse::Button::Right].press)
        {
            mouse[sf::Mouse::Button::Right].deltaX = 0;
            mouse[sf::Mouse::Button::Right].deltaY = 0;
        }
        else if (mouse[sf::Mouse::Button::Middle].press)
        {
            mouse[sf::Mouse::Button::Middle].deltaX = 0;
            mouse[sf::Mouse::Button::Middle].deltaY = 0;
        }
    }
    else
    {
        mouse_moved = false;
    }
}

SXB_NAMESPACE_END
