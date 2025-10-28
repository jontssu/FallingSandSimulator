#pragma once

#include "stdafx.h"
#include "Particle.h"

enum MaterialID
{
	MAT_ID_EMPTY,
	MAT_ID_SAND,
	MAT_ID_WATER
};

class Renderer
{
	public:
	    Renderer();
	    ~Renderer() {};

		Particle &get_p(int x, int y);

		void updateSand(int x, int y);
		void updateWater(int x, int y);

		void update();
		void render(sf::RenderTarget& target);
	private:
		std::vector<std::vector<Particle>>	particles;
		int									frame_count = 0;
};
