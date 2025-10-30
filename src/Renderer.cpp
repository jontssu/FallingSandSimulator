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
		// Try to fall as far as velocity allows (default to 5 if velocity is 0)
		int maxFallDistance = static_cast<int>(sand.getVelocity().y);
		if (maxFallDistance <= 0)
			maxFallDistance = 1;

		int fallDistance = 0;
		for (int i = 1; i <= maxFallDistance && y + i < WindowHeight; ++i)
		{
			Particle& below = get_p(x, y + i);
			// Only fall through empty spaces, STOP at water or sand
			if (below.getId() == MAT_ID_EMPTY)
				fallDistance = i;
			else
				break;  // Hit an obstacle
		}

		// If we can fall straight down, do it
		if (fallDistance > 0)
		{
			std::swap(sand, get_p(x, y + fallDistance));
			return;
		}

		Particle& below = get_p(x, y + 1);
		if (below.getId() == MAT_ID_WATER)
		{
			std::swap(sand, below);
			return;
		}

		// Can't fall straight, try diagonal
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
		// Try to fall as far as velocity allows (default to 5 if velocity is 0)
		int maxFallDistance = static_cast<int>(water.getVelocity().y);
		if (maxFallDistance <= 0)
			maxFallDistance = 1;

		int fallDistance = 0;
		for (int i = 1; i <= maxFallDistance && y + i < WindowHeight; ++i)
		{
			Particle& below = get_p(x, y + i);
			// Only fall through empty spaces, STOP at water or sand
			if (below.getId() == MAT_ID_EMPTY)
				fallDistance = i;
			else
				break;  // Hit an obstacle
		}

		// If we can fall straight down, do it
		if (fallDistance > 0)
		{
			std::swap(water, get_p(x, y + fallDistance));
			return;
		}

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
	
	// Only spread horizontally if we couldn't move down
	int dispersityRate = water.getDispersityRate();
	if (x - dispersityRate > 0 && x + dispersityRate + 1 < WindowWidth)
	{
		if (rand() % 2 == 0)
		{
			for (int i = 1; i <= dispersityRate; ++i)
			{
				Particle& left = get_p(x - i, y);
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
			for (int i = 1; i <= dispersityRate; ++i)
			{
				Particle& right = get_p(x + i, y);
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

void Renderer::updateWood(float dt, int x, int y)
{
	Particle& wood = get_p(x, y);
	if (wood.HasBeenUpdated())
		return;
	wood.setHasBeenUpdated(true);

	if (wood.getIsOnFire())
	{
		if (y - 1 >= 0)
		{
			Particle& above = get_p(x, y - 1);
			if (above.getId() == MAT_ID_EMPTY && rand() % 5 == 0)
				above.setId(MAT_ID_SMOKE);
		}
		if (wood.burn(dt))
		{
			wood.setId(MAT_ID_FIRE);
			wood.setLifetime(MAT_FIRE_LIFETIME);
		}
		if (y + 1 < WindowHeight && rand() % 4 == 0)
		{
			Particle& below = get_p(x, y + 1);
			if (below.getId() == MAT_ID_WOOD)
			{
				below.setId(MAT_ID_WOODFIRE);
				below.setIsOnFire(true);
			}
		}
		if (x - 1 >= 0 && rand() % 4 == 0)
		{
			Particle& left = get_p(x - 1, y);
			if (left.getId() == MAT_ID_WOOD)
			{
				left.setId(MAT_ID_WOODFIRE);
				left.setIsOnFire(true);
			}
		}
		if (x + 1 < WindowWidth && rand() % 4 == 0)
		{
			Particle& right = get_p(x + 1, y);
			if (right.getId() == MAT_ID_WOOD)
			{
				right.setId(MAT_ID_WOODFIRE);
				right.setIsOnFire(true);
			}
		}
		if (y - 1 >= 0 && rand() % 4 == 0)
		{
			Particle& above = get_p(x, y - 1);
			if (above.getId() == MAT_ID_WOOD)
			{
				above.setId(MAT_ID_WOODFIRE);
				above.setIsOnFire(true);
			}
		}
	}
}

void Renderer::updateStone(int x, int y) 
{
}

void Renderer::updateOil(int x, int y)
{
}

void Renderer::updateFire(float dt, int x, int y)
{
	Particle& fire = get_p(x, y);
	if (fire.HasBeenUpdated())
		return;
	fire.setHasBeenUpdated(true);
	int hasSpread = false;

	if (fire.burn(dt))
	{
		fire.setId(MAT_ID_EMPTY);
		return;
	}

	Particle& below = get_p(x, y + 1);
	if (y + 1 < WindowHeight)
	{
		int id = below.getId();
		if (id == MAT_ID_WOOD || id == MAT_ID_OIL)
		{
			if (id == MAT_ID_WOOD)
				below.setId(MAT_ID_WOODFIRE);
			below.setIsOnFire(true);
			hasSpread = true;
		}
	}
	if (x - 1 >= 0)
	{
		Particle& left = get_p(x - 1, y);
		int id = left.getId();
		if (id == MAT_ID_WOOD || id == MAT_ID_OIL)
		{
			if (id == MAT_ID_WOOD)
				left.setId(MAT_ID_WOODFIRE);
			left.setIsOnFire(true);
			hasSpread = true;
		}
	}
	if (x + 1 < WindowWidth)
	{
		Particle& right = get_p(x + 1, y);
		int id = right.getId();
		if (id == MAT_ID_WOOD || id == MAT_ID_OIL)
		{
			if (id == MAT_ID_WOOD)
				right.setId(MAT_ID_WOODFIRE);
			right.setIsOnFire(true);
			hasSpread = true;
		}
	}
	if (y - 1 >= 0)
	{
		Particle& above = get_p(x, y - 1);
		int id = above.getId();
		if (id == MAT_ID_WOOD || id == MAT_ID_OIL)
		{
			if (id == MAT_ID_WOOD)
				above.setId(MAT_ID_WOODFIRE);
			above.setIsOnFire(true);
			hasSpread = true;
		}
	}
	if (hasSpread)
	{
		fire.setId(MAT_ID_EMPTY);
		return;
	}
	if (below.getId() == MAT_ID_EMPTY)
	{
		std::swap(fire, below);
		return;	
	}
	Particle& belowLeft = get_p(x - 1, y + 1);
	if (belowLeft.getId() == MAT_ID_EMPTY)
	{
		std::swap(fire, belowLeft);
		return;
	}
	Particle& belowRight = get_p(x + 1, y + 1);
	if (belowRight.getId() == MAT_ID_EMPTY)
	{
		std::swap(fire, belowRight);
		return;
	}
}

void Renderer::updateSmoke(float dt, int x, int y)
{
	Particle& smoke = get_p(x, y);
	if (smoke.HasBeenUpdated())
		return;
	smoke.setHasBeenUpdated(true);

	if (smoke.burn(dt))
	{
		smoke.setId(MAT_ID_EMPTY);
		smoke.setLifetime(10.f);
	}

	Particle& above = get_p(x, y - 1);
	if (above.getId() == MAT_ID_EMPTY)
	{
		std::swap(smoke, above);
		return;	
	}
	Particle& aboveLeft = get_p(x - 1, y - 1);
	if (aboveLeft.getId() == MAT_ID_EMPTY)
	{
		std::swap(smoke, aboveLeft);
		return;
	}
	Particle& aboveRight = get_p(x + 1, y - 1);
	if (aboveRight.getId() == MAT_ID_EMPTY)
	{
		std::swap(smoke, aboveRight);
		return;
	}
	Particle& left = get_p(x - 1, y);
	if (left.getId() == MAT_ID_EMPTY)
	{
		std::swap(smoke, left);
		return;
	}	
	Particle& right = get_p(x + 1, y);
	if (right.getId() == MAT_ID_EMPTY)
	{
		std::swap(smoke, right);
		return;
	}
}


void Renderer::update(float dt)
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
					case MAT_ID_WOOD:
						updateWood(dt, x, y);
						break;
					case MAT_ID_STONE:
						updateStone(x, y);
						break;
					case MAT_ID_OIL:
						updateOil(x, y);
						break;
					case MAT_ID_FIRE:
						updateFire(dt, x, y);
						break;
					case MAT_ID_WOODFIRE:
						updateWood(dt, x, y);
						break;
					case MAT_ID_SMOKE:
						updateSmoke(dt, x, y);
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
					case MAT_ID_WOOD:
						updateWood(dt, x, y);
						break;
					case MAT_ID_STONE:
						updateStone(x, y);
						break;
					case MAT_ID_OIL:
						updateOil(x, y);
						break;
					case MAT_ID_FIRE:
						updateFire(dt, x, y);
						break;
					case MAT_ID_WOODFIRE:
						updateWood(dt, x, y);
						break;
					case MAT_ID_SMOKE:
						updateSmoke(dt, x, y);
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
					rectangle.setFillColor(sf::Color(0, 105, 148)); // Ocean blue
					target.draw(rectangle);
					break;
				}
				case MAT_ID_WOOD:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color(70, 50, 30)); // Dark Brown color
					target.draw(rectangle);
					break;
				}
				case MAT_ID_STONE:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color(128, 128, 128)); // Gray color
					target.draw(rectangle);
					break;
				}
				case MAT_ID_OIL:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color(0, 0, 0)); // Black color
					target.draw(rectangle);
					break;
				}
				case MAT_ID_FIRE:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					if (rand() % 2 == 0)
						rectangle.setFillColor(sf::Color::Yellow); // Yellow color
					else
						rectangle.setFillColor(sf::Color::Red); // Red color
					target.draw(rectangle);
					break;
				}
				case MAT_ID_WOODFIRE:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					if (rand() % 2 == 0)
						rectangle.setFillColor(sf::Color::Yellow); // Yellow color
					else
						rectangle.setFillColor(sf::Color::Red); // Red color
					target.draw(rectangle);
					break;
				}
				case MAT_ID_SMOKE:
				{
					sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
					rectangle.setPosition({static_cast<float>(x), static_cast<float>(y)});
					rectangle.setFillColor(sf::Color(0, 0, 0)); // Black color
					target.draw(rectangle);
					break;
				}
				default:
					break;
			}
		}
	}
}
