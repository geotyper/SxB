
#ifndef _CURSOR_H_9F13E0EC73F4A90684B3C8C583364638_
#define _CURSOR_H_9F13E0EC73F4A90684B3C8C583364638_

#include <SFML/Window/Event.hpp>

#include "sxbCommon/defines.h"

SXB_NAMESPACE_BEGIN

struct Cursor
{
    struct m2
    {
        bool    press { false };
        int     x { 0 };
        int     y { 0 };
        int     deltaX { 0 };
        int     deltaY { 0 };
    };
    Cursor();
    
    void handleEvent(const sf::Event &event);
    void update();

    m2      touch[SXB_DEFAULT_MAX_FINGER];
    bool    button_type[sf::Mouse::Button::ButtonCount];
    m2      mouse;
    float   wheelDelta{ 0.0f };
    
    bool    mouse_moved{ false };
    int     wheel_moved{ 0 };
    
    bool    mouse_lock { false };
};

SXB_NAMESPACE_END

#endif // _CURSOR_H_9F13E0EC73F4A90684B3C8C583364638_
