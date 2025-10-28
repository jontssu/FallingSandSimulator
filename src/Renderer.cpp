#include "Renderer.h"

Renderer::Renderer()
{
	particles.resize(WindowWidth);
	for (auto &col : particles)
		col.resize(WindowHeight);
}

Particle &Renderer::get_p(int x, int y)
{
	return particles[x][y];
}

void Renderer::updateSand(int x, int y)
{
	Particle &sand = get_p(x, y);
	if (y + 1 < WindowHeight)
	{
		Particle& below = get_p(x, y + 1);

		if (below.getId() == MAT_ID_EMPTY)
			std::swap(sand, below);
		else if (below.getId() == MAT_ID_WATER)
			std::swap(sand, below);

		Particle& belowLeft = get_p(x - 1, y + 1);
		Particle& belowRight = get_p(x + 1, y + 1);

		if ((belowLeft.getId() == MAT_ID_EMPTY && belowRight.getId() == MAT_ID_EMPTY)
			|| (belowLeft.getId() == MAT_ID_WATER && belowRight.getId() == MAT_ID_WATER))
		{
			if (rand() % 2 == 0)
				std::swap(sand, belowLeft);
			else
				std::swap(sand, belowRight);
		}
		else if (belowLeft.getId() == MAT_ID_EMPTY
			|| belowLeft.getId() == MAT_ID_WATER)
			std::swap(sand, belowLeft);
		else if (belowRight.getId() == MAT_ID_EMPTY
			|| belowRight.getId() == MAT_ID_WATER)
			std::swap(sand, belowRight);
	}
}

void Renderer::updateWater(int x, int y)
{
	Particle& water = get_p(x, y);
	if (y + 1 < WindowHeight)
	{
		Particle& below = get_p(x, y + 1);

		if (below.getId() == MAT_ID_EMPTY)
			std::swap(water, below);

		Particle& belowLeft = get_p(x - 1, y + 1);
		Particle& belowRight = get_p(x + 1, y + 1);
		if (belowLeft.getId() == MAT_ID_EMPTY && belowRight.getId() == MAT_ID_EMPTY)
		{
			if (rand() % 2 == 0)
				std::swap(water, belowLeft);
			else
				std::swap(water, belowRight);
		}
		else if (belowLeft.getId() == MAT_ID_EMPTY)
			std::swap(water, belowLeft);
		else if (belowRight.getId() == MAT_ID_EMPTY)
			std::swap(water, belowRight);

		Particle& left = get_p(x - 1, y);
		Particle& right = get_p(x + 1, y);
		if (left.getId() == MAT_ID_EMPTY && right.getId() == MAT_ID_EMPTY)
		{
			if (rand() % 2 == 0)
				std::swap(water, left);
			else
				std::swap(water, right);
		}
		else if (left.getId() == MAT_ID_EMPTY)
			std::swap(water, left);
		else if (right.getId() == MAT_ID_EMPTY)
			std::swap(water, right);
	}
}

// void Renderer::update()
// {
// 	for (int y = GRID_HEIGHT - 1; y > 0; --y)
// 	{
// 		for (int x = 0; x < GRID_WIDTH; ++x)
// 		{
// 			int mat_id = get_p(x, y).getId();
// 			switch (mat_id)
// 			{
// 				case MAT_ID_EMPTY:
// 					break;
// 				case MAT_ID_SAND:
// 					updateSand(x, y);
// 					break;
// 				case MAT_ID_WATER:
// 					updateWater(x, y);
// 					break;
// 			}
// 		}
// 	}
// }

void Renderer::update()
{
	frame_count++;
	for (int y = GRID_HEIGHT - 1; y > 0; --y)
	{
		if (frame_count % 2 == 0)
		{
			for (int x = 0; x < GRID_WIDTH; ++x)
			{
				int mat_id = get_p(x, y).getId();
				switch (mat_id)
				{
					case MAT_ID_EMPTY:
						break;
					case MAT_ID_SAND:
						updateSand(x, y);
						break;
					case MAT_ID_WATER:
						updateWater(x, y);
						break;
				}
			}
		}
		else
		{
			for (int x = GRID_WIDTH - 1; x >= 0; --x)
			{
				int mat_id = get_p(x, y).getId();
				switch (mat_id)
				{
					case MAT_ID_EMPTY:
						break;
					case MAT_ID_SAND:
						updateSand(x, y);
						break;
					case MAT_ID_WATER:
						updateWater(x, y);
						break;
				}
			}
		}
	}
}

void Renderer::render(sf::RenderTarget &target)
{
	for (int y = GRID_HEIGHT - 1; y > 0; --y)
	{
		for (int x = 0; x < GRID_WIDTH; ++x)
		{
			int mat_id = get_p(x, y).getId();
			switch (mat_id)
			{
				case MAT_ID_EMPTY:
					break;
				case MAT_ID_SAND:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color::Yellow);
					target.draw(rectangle);
					break;
				}
				case MAT_ID_WATER:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color::Cyan);
					target.draw(rectangle);
					break;
				}
			}
		}
	}
}
