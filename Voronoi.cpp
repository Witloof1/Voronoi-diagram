#include <iostream>
#include <SFML/Graphics.hpp>
#include <limits>
#include <vector>
const int WIDTH = 800;
const int HEIGHT = 600;
const int POINT_COUNT = 100;

float dist(float x1, float y1, float x2, float y2) { return (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2); }

sf::Uint8 *pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

struct Point
{
	float x, y;
	uint8_t ID; 
	Point() { x = rand() % WIDTH; y = rand() % HEIGHT; }
	void draw(sf::RenderWindow &window)
	{
		sf::CircleShape circle(2, 10);
		circle.setOutlineThickness(1);
		circle.setOutlineColor(sf::Color::Black);
		circle.setPosition(x - 2, y - 2);
		window.draw(circle);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Voronoi-diagram");

	sf::Texture texture;
	texture.create(WIDTH, HEIGHT);
	sf::Sprite sprite(texture);

	srand(std::time(NULL));
	
	Point points[POINT_COUNT];
	for (int i = 0; i < POINT_COUNT; i++)
		points[i].ID = i;

	int y = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased)
			{
				y = 0;
				for (Point &p : points)
				{
					p.x = rand() % WIDTH; 
					p.y = rand() % HEIGHT;
				}
			}
		}

		window.clear(sf::Color(50, 50, 50));
		
		if (y < HEIGHT)
		{				
			for (int x = 0; x < WIDTH; x++)
			{
				uint8_t VoronoiCell; float fRecord = std::numeric_limits<float>::max();
				for (Point &p : points)
				{
					float fDist = dist(p.x, p.y, x, y);
					if (fDist < fRecord)
					{
						VoronoiCell = p.ID;
						fRecord = fDist;
					}
				}	
				srand(VoronoiCell);
				pixels[(x + y * WIDTH) * 4 + 0] = rand() % 255;
				pixels[(x + y * WIDTH) * 4 + 1] = rand() % 255;
				pixels[(x + y * WIDTH) * 4 + 2] = rand() % 255;			
				pixels[(x + y * WIDTH) * 4 + 3] = 255;
			}
			y++;
		}		

		texture.update(pixels);
		window.draw(sprite);

		for (Point &p : points)
			p.draw(window);
	
		window.display();
	}
}
