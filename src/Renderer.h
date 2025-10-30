#pragma once

#include "stdafx.h"
#include "Particle.h"


class Renderer
{
	public:
	    Renderer();
	    ~Renderer() {};

		Particle &get_p(int x, int y);

		void updateSand(int x, int y);
		void updateWater(int x, int y);
		void updateWood(float dt, int x, int y);
		void updateStone(int x, int y);
		void updateOil(int x, int y);
		void updateFire(float dt, int x, int y);
		void updateSmoke(float dt, int x, int y);

	    void update(float deltaTime);
	    void render(sf::RenderTarget &target);

	  private:
		std::vector<std::vector<Particle>>	particles;
		int									frame_count = 0;
		sf::Vector2f						gravity = {0.f, 1.f};
};
