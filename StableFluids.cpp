#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"
#include "SFML/System.hpp"
#include "SFML/System/Clock.hpp"
#include "Visualization/Visualization.hpp"
#include <iostream>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
	Visualization::ScalarField scalf;
	std::cout << "Press LMB to add fluid.\n";
	std::cout << "Hold L and move mouse, releasing L would disturb the fluid in the direction of mouse movement.\n";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			scalf.ProcessMouse(window, event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

		}
		scalf.Update();
		window.clear(sf::Color::Black);
		scalf.Draw(window);
		window.display();
	}
	system("pause");
	return 0;
}
