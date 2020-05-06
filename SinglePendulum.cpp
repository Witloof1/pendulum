#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

const int nWIDTH = 800, nHEIGHT = 600;
const float PI = 3.14159265;

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode(nWIDTH, nHEIGHT), "Single Pendulum");
	window.setFramerateLimit(60);
	sf::Clock clock;

	sf::Vector2f vOorsprong { nWIDTH / 2, 150.0f };
	sf::Vector2f vMouse { 0, 0 };

	const float fRadius = 15;
	sf::CircleShape massa { fRadius, 32 };
	sf::Vertex touw[] = {
		sf::Vertex(vOorsprong),
		sf::Vertex()
	};

	const float fLengte = 300.0f;
	const float fValversnelling = 981.0f;
	float fBeginhoek = -PI / 2;
	float fHoek;
	float x; float y;

	bool bMassaGeselecteerd = false;

	auto bepaalHoek = [](const float theta0, const float g, const float l, const float t) -> float
	{
		return theta0 * cos(sqrt(g / l) * t);
	};

	auto getX = [&vOorsprong](const float theta, const float l) -> float
	{
		return vOorsprong.x + sin(theta) * l;
	};

	auto getY = [&vOorsprong](const float theta, const float l) -> float
	{
		return vOorsprong.y + cos(theta) * l;
	};

	auto squaredDist = [](sf::Vector2f a, sf::Vector2f b) -> float
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonReleased)
				bMassaGeselecteerd = false;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				vMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if (squaredDist(vMouse, { x, y }) < fRadius * fRadius)
					bMassaGeselecteerd = true;
			}
		}
		
		if (bMassaGeselecteerd)
		{
			float fAanliggende = vMouse.x - vOorsprong.x;
			float fOverstaande = vMouse.y - vOorsprong.y;

			if (fAanliggende > 0)
				fBeginhoek = PI / 2 - atan(fOverstaande / fAanliggende);
			else if (fAanliggende < 0)
				fBeginhoek = -PI / 2 - atan(fOverstaande / fAanliggende);
			else
				fBeginhoek = 0;
			clock.restart();
		}

		fHoek = bepaalHoek(fBeginhoek, fValversnelling, fLengte, clock.getElapsedTime().asSeconds());

		x = getX(fHoek, fLengte);
		y = getY(fHoek, fLengte);

		touw[1] = sf::Vertex({ x, y });
		massa.setPosition(x - fRadius, y - fRadius);

		window.clear();

		window.draw(touw, 2, sf::Lines);
		window.draw(massa);

		window.display();
	}

	return 0;
}