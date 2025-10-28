#include "Particle.h"

Particle::Particle(int id, float lifetime, sf::Vector2f velocity, sf::Color color)
	: id(id), lifetime(lifetime), velocity(velocity), color(color), has_been_updated(false)
{
}

void Particle::update()
{
}
