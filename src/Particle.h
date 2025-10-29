#pragma once

#include "stdafx.h"

class Particle
{
	public:
	    Particle() = default;
		Particle(int id, float lifetime, sf::Vector2f velocity, sf::Color color);
	    ~Particle() {}

		void setHasBeenUpdated(bool updated) { has_been_updated = updated; }
		inline void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
		inline void setIsSideMoving(int val) { isSideMoving = val; }

		inline const int getId() const { return id; }
		inline const bool HasBeenUpdated() const { return has_been_updated; }
		inline const sf::Vector2f& getVelocity() const { return velocity; }
		inline const int getIsSideMoving() const { return isSideMoving; }

		void update();

	private:
		int				id = 0;
		float			lifetime = 0.f;
		sf::Vector2f	velocity;
		sf::Color		color = sf::Color::White;
		bool			has_been_updated = false;
		int				isSideMoving = 0;

};