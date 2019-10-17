
#include "ozzh.h"

#define CUR_WIDTH       1136
#define CUR_HEIGHT      640

// Please set platform data window to a CAMetalLayer
int main(int argc, char *argv[])
{
    // Create the main window
    sf::WindowBase window(sf::VideoMode(1366, 768), "SFML window");
    ozzh exampleOzzh;
    exampleOzzh.init(window.getSystemHandle());
    
    sxb::Cursor cursor;
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            cursor.handleEvent(event);
        }
        
        cursor.update();
        exampleOzzh.update(cursor);
    }
    
    return EXIT_SUCCESS;

}
