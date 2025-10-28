#pragma once

#include "stdafx.h"

class Particle
{
	public:
	    Particle() = default;
		Particle(int id, float lifetime, sf::Vector2f velocity, sf::Color color);
	    ~Particle() {}

		inline const int getId() const { return id; }

		void update();

	private:
		int				id = 0;
		float			lifetime = 0.f;
		sf::Vector2f	velocity = {0.f, 0.f};
		sf::Color		color = sf::Color::White;
		bool			has_been_updated = false;


};