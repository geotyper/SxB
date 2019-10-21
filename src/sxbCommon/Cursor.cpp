
#include "sxbCommon/Cursor.h"

SXB_NAMESPACE_BEGIN

Cursor::Cursor()
{
    for ( int i = 0; i < (int)sf::Mouse::Button::ButtonCount; ++i )
    {
        button_type[i] = false;
    }
}

void Cursor::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::TouchBegan && event.touch.finger < SXB_DEFAULT_MAX_FINGER )
    {
        touch[event.touch.finger].press = true;
        touch[event.touch.finger].x = event.touch.x;
        touch[event.touch.finger].y = event.touch.y;
    }
    else if (event.type == sf::Event::TouchMoved && event.touch.finger < SXB_DEFAULT_MAX_FINGER )
    {
        touch[event.touch.finger].press = true;
        touch[event.touch.finger].deltaX = event.touch.x - touch[event.touch.finger].x;
        touch[event.touch.finger].deltaY = event.touch.y - touch[event.touch.finger].y;
        touch[event.touch.finger].x = event.touch.x;
        touch[event.touch.finger].y = event.touch.y;
    }
    else if (event.type == sf::Event::TouchEnded && event.touch.finger < SXB_DEFAULT_MAX_FINGER )
    {
        touch[event.touch.finger].press = false;
        touch[event.touch.finger].deltaX = 0;
        touch[event.touch.finger].deltaY = 0;
        touch[event.touch.finger].x = 0;
        touch[event.touch.finger].y = 0;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button < sf::Mouse::Button::ButtonCount )
    {
        button_type[event.mouseButton.button] = true;
        mouse.press = true;
        mouse.x = event.mouseButton.x;
        mouse.y = event.mouseButton.y;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button < sf::Mouse::Button::ButtonCount )
    {
        mouse.press = false;
        mouse_lock = false;
        button_type[event.mouseButton.button] = false;
        mouse.deltaX = 0;
        mouse.deltaY = 0;
    }
    else if ( event.type == sf::Event::MouseMoved )
    {
        mouse_moved = true;
        mouse.deltaX = event.mouseMove.x - mouse.x;
        mouse.deltaY = event.mouseMove.y - mouse.y;
        mouse.x = event.mouseMove.x;
        mouse.y = event.mouseMove.y;
    }
    else if ( event.type == sf::Event::MouseWheelScrolled )
    {
        if ( event.mouseWheelScroll.delta != 0 )
        {
            wheelDelta = event.mouseWheelScroll.delta;
        }
        wheel_moved++;
    }
}

void Cursor::update()
{
    wheel_moved--;
    if ( wheel_moved < 0 )
    {
        wheel_moved = 0;
    }
    if ( !mouse_moved )
    {
        mouse.deltaX = 0;
        mouse.deltaY = 0;
    }
    else
    {
        mouse_moved = false;
    }
    
    if ( wheel_moved == 0 )
    {
        wheelDelta = 0;
    }
}

SXB_NAMESPACE_END
