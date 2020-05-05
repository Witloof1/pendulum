#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

const int nWIDTH = 800, nHEIGHT = 600;
const float PI = 3.14159265;

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode(nWIDTH, nHEIGHT), "Single Pendulum");
	sf::Clock clock;
	window.setFramerateLimit(60);

	sf::Vector2f vOorsprong { nWIDTH / 2, 150 };
	
	const float fRadius = 15;
	sf::CircleShape massa { fRadius, 32 };
	sf::Vertex touw[] = {
		sf::Vertex(vOorsprong),
		sf::Vertex()
	};

	const float fLengte = 300.0f;
	const float fValversnelling = 9.81;
	float fBeginhoek = PI / 2;
	float fHoek;
	float x; float y;

	auto bepaalHoek = [](const float theta0, const float g, const float l, const float t) -> float
	{
		return theta0 * cos(sqrt(g / l) * t);
	};

	auto setX = [&vOorsprong](const float theta, const float l) -> float
	{
		return vOorsprong.x + sin(theta) * l;
	};

	auto setY = [&vOorsprong](const float theta, const float l) -> float
	{
		return vOorsprong.y + cos(theta) * l;
	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		fHoek = bepaalHoek(fBeginhoek, fValversnelling, fLengte, clock.getElapsedTime().asSeconds());
		x = setX(fHoek, fLengte);
		y = setY(fHoek, fLengte);

		touw[1] = sf::Vertex({ x, y });
		massa.setPosition(x - fRadius, y - fRadius);

		window.clear();

		window.draw(touw, 2, sf::Lines);
		window.draw(massa);

		window.display();
	}

	return 0;
}