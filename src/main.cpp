#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tools/Log.h"
#include "Tools/Timer.h"

// ---- CONFIG ----
int sizeX = 128;
int sizeY = 128;
int tileSize = 6;

void Render(sf::RenderWindow& window) {
    window.clear();
    
    window.display();
}

int main()
{

#ifdef DIST
    FreeConsole();
#else
    Debug::Log::Init();
#endif

    LOG_TRACE("Started Engine.");

    Debug::Timer wtmr;
    sf::RenderWindow window(sf::VideoMode(sizeX*tileSize, sizeY*tileSize), "BioSim C++");
    LOG_TRACE("Initialised window in {0}s", wtmr.Elapsed());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        Render(window);
    }

    return 0;
}
