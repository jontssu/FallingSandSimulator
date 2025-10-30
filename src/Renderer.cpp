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
	Particle& sand = get_p(x, y);
	if (sand.HasBeenUpdated())
		return;
	sand.setHasBeenUpdated(true);
	if (y + 1 < WindowHeight)
	{
		Particle& below = get_p(x, y + 1);
		if (below.getId() == MAT_ID_EMPTY
			|| below.getId() == MAT_ID_WATER)
		{
			std::swap(sand, below);
			return;
		}

		Particle& belowLeft = get_p(x - 1, y + 1);
		if (belowLeft.getId() == MAT_ID_EMPTY
			|| belowLeft.getId() == MAT_ID_WATER)
		{
			std::swap(sand, belowLeft);
			return;
		}

		Particle& belowRight = get_p(x + 1, y + 1);
		if (belowRight.getId() == MAT_ID_EMPTY
			|| belowRight.getId() == MAT_ID_WATER)
		{
			std::swap(sand, belowRight);
			return;
		}
	}
}


void Renderer::updateWater(int x, int y)

{
	Particle& water = get_p(x, y);
	if (water.HasBeenUpdated())
		return;
	water.setHasBeenUpdated(true);

	if (y + 1 < WindowHeight)
	{
		Particle& below = get_p(x, y + 1);
		if (below.getId() == MAT_ID_EMPTY)
		{
			std::swap(water, below);
			return;
		}
		// Try diagonal moves with random direction
		if (rand() % 2 == 0)
		{
			// Try left first
			if (x > 0)
			{
				Particle& belowLeft = get_p(x - 1, y + 1);
				if (belowLeft.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, belowLeft);
					return;
				}
			}
			// Then try right
			if (x + 1 < WindowWidth)
			{
				Particle& belowRight = get_p(x + 1, y + 1);
				if (belowRight.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, belowRight);
					return;
				}
			}
		}
		else
		{
			if (x + 1 < WindowWidth)
			{
				Particle& belowRight = get_p(x + 1, y + 1);
				if (belowRight.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, belowRight);
					return;
				}
			}
			if (x > 0)
			{
				Particle& belowLeft = get_p(x - 1, y + 1);
				if (belowLeft.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, belowLeft);
					return;
				}
			}
		}
	}
	int dispersityRate = water.getDispersityRate();
	if (x - dispersityRate > 0 && x + dispersityRate + 1 < WindowWidth)
	{
		if (rand() % 2 == 0)
		{
			for (int i = 0; i < dispersityRate; i++)
			{
				Particle& left = get_p(x - i - 1, y);
				if (left.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, left);
					return;
				}
				if (left.getId() == MAT_ID_SAND)
					return;
			}
		}
		else
		{
			for (int i = 0; i < dispersityRate; i++)
			{
				Particle& right = get_p(x + i + 1, y);
				if (right.getId() == MAT_ID_EMPTY)
				{
					std::swap(water, right);
					return;
				}
				if (right.getId() == MAT_ID_SAND)
					return;
			}
		}
	}
}


void Renderer::update()
{
	for (int y = 0; y < GRID_HEIGHT; ++y)
	{
		for (int x = 0; x < GRID_WIDTH; ++x)
		{
			Particle& p = get_p(x, y);
			p.setHasBeenUpdated(false);
		}
	}

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
