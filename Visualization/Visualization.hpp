#pragma once
#include "SFML/Graphics.hpp"
#include "StableFluids.hpp"
#include<algorithm>
namespace Visualization {
	class ScalarField
	{
	private:
		std::vector<std::vector<sf::RectangleShape>> fRects;
		float rectWidth = 800.0f / (float)width;
		float rectHeight = 600.0f / (float)width;
		sf::Font font;
		sf::Vector2f initialPos;
		sf::Vector2f finalPos;
		bool initialSet = false;
	public:
		ScalarField()
		{
			fRects.resize(width + 2);
			for (auto & vecRect : fRects)
			{
				vecRect.resize(width + 2, sf::RectangleShape(sf::Vector2f(rectWidth, rectHeight)));
			}
			for (int i = 1; i <= width; i++)
			{
				for (int j = 1; j <= width; j++) {

					fRects[i][j].setPosition(sf::Vector2f((i - 1)*rectWidth, (j - 1)*rectHeight));
				}
			}
			InitializeField();
		}
		void ProcessLMB(sf::RenderWindow & window)
		{
			sf::Vector2i vec = sf::Mouse::getPosition(window);
			int i = vec.x / rectWidth;
			int j = vec.y / rectHeight;
			int with = 4;
			for (int k = i - with; k <= i + with; k++)
			{
				for (int l = j - with; l <= j + with; l++)
				{
					if ((k <= width && l <= width) && (k >= 0 && l >= 0))
						s1[k][l].val = 10;
				}
			}

		}
		void ProcessRMB(sf::RenderWindow & window, sf::Event & event)
		{
			if (event.type == sf::Event::KeyPressed && (!initialSet))
			{
				if (event.key.code == sf::Keyboard::L) {
					initialPos = sf::Vector2f(sf::Mouse::getPosition(window));
					initialSet = true;
				}
			}
			else if ((event.type == sf::Event::KeyReleased) && initialSet)
			{
				if (event.key.code == sf::Keyboard::L)
				{
					finalPos = sf::Vector2f(sf::Mouse::getPosition(window));
					sf::Vector2f del = finalPos - initialPos;
					sf::Vector2f t = del;
					del.x = 200.0*(del.x / sqrt((t.x * t.x + t.y * t.y)));
					del.y = 200.0 * (del.y / sqrt((t.x * t.x + t.y *t.y)));
					int i1 = initialPos.x / rectWidth;
					int j1 = initialPos.y / rectHeight;
					int i2 = finalPos.x / rectWidth;
					int j2 = finalPos.y / rectHeight;
					int imin = std::min<int>(i1, i2);
					int imax = std::max<int>(i1, i2);
					int jmin = std::min<int>(j1, j2);
					int jmax = std::max<int>(j1, j2);
					int step = 1;
					if ((i2 - i1) != 0)
					{
						double slope = double(j2 - j1) / double(i2 - i1);
						int c = int(double(j1) - double(i1) * slope);


						for (int i = imin; i <= imax; i += step)
						{
							int j = i * slope + c;
							if ((j >= 0 && i >= 0) && (j < width&&i < width)) {
								v1[i][j].x += del.x;
								v1[i][j].y += del.y;
							}
						}
					}
					initialSet = false;
				}

			}
		}
		void ProcessMouse(sf::RenderWindow & window, sf::Event & event)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				ProcessLMB(window);
			}
			ProcessRMB(window, event);


		}
		void Update()
		{
			Vel_Step();
			Dens_Step();
			float r = 0.0;
			for (int i = 1; i <= width; i++)
			{
				for (int j = 1; j <= width; j++)
				{

					r = s1[i][j].val * 40;
					r = std::clamp<float>(r, 0, 255);
					fRects[i][j].setFillColor(sf::Color(r, 100, 100));
				}
			}
		}
		void Draw(sf::RenderWindow & window)
		{
			for (int i = 1; i <= width; i++)
			{
				for (int j = 1; j <= width; j++) {
					window.draw(fRects[i][j]);
				}
			}
		}
	};
}
